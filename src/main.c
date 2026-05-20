/*****************************************************************************
 * @file     main.c
 * @brief    IAP (In-Application Programming) sample for ML63Q2537
 *
 * Receives a user firmware image over UART using XMODEM-CRC, programs it
 * into the user flash area (0x10000000–0x1003BFFF), then remaps + resets
 * to boot the new firmware.
 *
 * UART: P32 (RX) / P33 (TX), 115200 8N1.
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "ML63Q25x7.h"
#include "mcu.h"
#include "rdwr_reg.h"
#include "clock.h"
#include "irq.h"
#include "tbc.h"
#include "wdt.h"
#include "smpl_common.h"
#include "smpl_common_led.h"
#include "FlashPrg.h"

#include "device.h"
#include "uartf0_i.h"
#include "xmodem.h"
#include "main.h"
#include "debug_log.h"

#define UARTF_PARAM_MODE        ( UARTF_LG_8BIT | UARTF_STP_1BIT | UARTF_PT_NON | \
                                  UARTF_BC_DIS  | UARTF_DLAB_RBR_THR | \
                                  UARTF_RFR_KEEP | UARTF_TFR_KEEP | UARTF_FTL_2BYTE )
#define UARTF_PARAM_CAJ         ( UARTF_RMV_ENA | 0x0019U )
#define UARTF_PARAM_DLR         ( 0x0019U )            /* 115200 bps @ SYSCLK 48 MHz */

#define FLAG_SET                ( 1 )
#define FLAG_CLR                ( 0 )

#define IAP_AREA_START_ADDR     ( 0x1003E000UL )
#define ISP_AREA2_START_ADDR    ( 0x1003C000UL )
#define USER_AREA_ADDR          ( 0x10000000UL )

#define FLASH_SECTOR_SIZE       ( 2048U )

uint8_t  s_flag;
uint32_t s_rwSize;
uint16_t s_cbfErrStat;

uint8_t XmodemBuf[XMODEM_BLOCK_SIZE];

static const uint8_t msgIapProg[] = { 'I', 'A', 'P', '\r', '\n' };
static const uint8_t msgError[]   = { '\r', '\n', 'E', 'r', 'r', 'o', 'r', '\r', '\n' };

static void s_initIap( void );
static void s_procUartfWrite( uint32_t size, uint16_t errStatus );
static int  write_verify( uint32_t *WriteAddr, uint8_t *WriteData, uint16_t data_size );
static void error_proc( void );
static void s_loadRemapEnd( void );

extern void Remap_End( void );

/* Section bounds for .remap_end (LMA in FLASH2, VMA in RAM). The
 * newlib crt0 only initializes .data, so we have to copy this one
 * ourselves before Remap_End() can be called from RAM. */
extern uint32_t __remap_end_start__[];
extern uint32_t __remap_end_end__[];
extern uint32_t __remap_end_rom[];

static void s_loadRemapEnd( void )
{
    uint32_t *src = __remap_end_rom;
    uint32_t *dst = __remap_end_start__;
    while ( dst < __remap_end_end__ ) {
        *dst++ = *src++;
    }
}

static void s_procUartfWrite( uint32_t size, uint16_t errStatus )
{
    s_flag       = FLAG_SET;
    s_rwSize     = size;
    s_cbfErrStat = errStatus;
}

/* IAP-specific peripheral bring-up. device_initialize() has already set up
 * WDT, 48 MHz PLL, and USR_PERI; we add TBC (XMODEM tick), UART pin mux,
 * UART driver, LED, and interrupts. */
static void s_initIap( void )
{
    /* dbg_log_init() not called here — crt0 already zeroes the .bss-resident
     * log state, and we want zero debug_log calls on the pre-banner path
     * while diagnosing the no-boot issue. */

    __disable_irq();
    irq_uaf0_dis();

    /* TBC: long-tick base @ 128 Hz drives XMODEM timeout counting. */
    tbc_clrLTBR();
    tbc_setLTI0S( TBC_LTINS_T128HZ );

    /* P32 = UARTF0_RX, P33 = UARTF0_TX. P3MOD0 uses 8 bits per pin;
     * P32 occupies bits 23:16, P33 bits 31:24. */
    uint32_t p3mod0 = read_reg32( PORT3->P3MOD0 );
    p3mod0 &= ~((0x3FUL << 16) | (0x3FUL << 24));
    p3mod0 |=  ((0x21UL << 16) | (0x23UL << 24));
    write_reg32( PORT3->P3MOD0, p3mod0 );
    set_bit( PORT3->P3DO, (1U << 3) );

    uartf0_init( (uint16_t)UARTF_PARAM_MODE, (uint16_t)UARTF_PARAM_CAJ, (uint16_t)UARTF_PARAM_DLR );

    smpl_initLED1( LED_INACTIVE );

    irq_uaf0_clearIRQ();
    irq_uaf0_ena();
    __enable_irq();
}

static void error_proc( void )
{
    s_flag   = FLAG_CLR;
    s_rwSize = sizeof(msgError);
    uartf0_write( (uint8_t *)msgError, s_rwSize, s_procUartfWrite );
    while ( s_flag == FLAG_CLR ) {
        wdt_clear();
    }
    irq_uaf0_dis();

    smpl_onLED1();

    for (;;) {
        wdt_clear();
    }
}

/* Write `data_size` bytes (128 for SOH blocks, 1024 for STX blocks) starting
 * at *WriteAddr. Erases a 2 KB sector at the boundary, then writes 32-bit
 * words. Verifies each word against memory after write. Refuses to write
 * past the IAP code area (>= ISP_AREA2_START_ADDR) to protect the bootloader
 * itself. */
static int write_verify( uint32_t *WriteAddr, uint8_t *WriteData, uint16_t data_size )
{
    uint16_t write_cnt;
    uint32_t writeData;

    if ( (*WriteAddr + data_size - 1) >= ISP_AREA2_START_ADDR ) {
        return M_NG;
    }

    /* flash_open / flash_controlSelfProg / flash_close manage driver state and
     * the FLASHSLF register; they are not per-word HW requirements. The per-word
     * HW unlock is the FLASHACP accept sequence, emitted by
     * flash_writeProgramMemory itself. Hoisting open/close out of the inner loop
     * removes ~3x register accesses per word. */
    if ( ((*WriteAddr) % FLASH_SECTOR_SIZE) == 0 ) {
        dbg_log_evt( DBG_EVT_ERASE_BEGIN, 0, *WriteAddr );
        flash_open();
        flash_controlSelfProg( flash_getFselfEn() );

        flash_eraseSectorProgramMemory( (void *)(*WriteAddr), flash_getFacp1(), flash_getFacp2() );
        while ( flash_checkFlashAccess() != FLASH_MEMORY_ACCESSIBLE ) {}

        flash_controlSelfProg( flash_getFselfDis() );
        flash_close();
        dbg_log_evt( DBG_EVT_ERASE_END, 0, *WriteAddr );
    }

    dbg_log_evt( DBG_EVT_WRITE_BEGIN, data_size, *WriteAddr );
    flash_open();
    flash_controlSelfProg( flash_getFselfEn() );

    for ( write_cnt = 0; write_cnt < data_size; write_cnt += 4 ) {
        writeData  =  (uint32_t)*(WriteData + write_cnt)     & 0x000000FFU;
        writeData |= ((uint32_t)*(WriteData + write_cnt + 1) <<  8);
        writeData |= ((uint32_t)*(WriteData + write_cnt + 2) << 16);
        writeData |= ((uint32_t)*(WriteData + write_cnt + 3) << 24);

        flash_writeProgramMemory( (void *)(*WriteAddr), writeData, flash_getFacp1(), flash_getFacp2() );
        while ( flash_checkFlashAccess() != FLASH_MEMORY_ACCESSIBLE ) {}

        if ( *((uint32_t *)(*WriteAddr)) != writeData ) {
            dbg_log_evt( DBG_EVT_VERIFY_FAIL, 0, *WriteAddr );
            flash_controlSelfProg( flash_getFselfDis() );
            flash_close();
            return M_NG;
        }
        *WriteAddr += 4;
    }

    flash_controlSelfProg( flash_getFselfDis() );
    flash_close();
    dbg_log_evt( DBG_EVT_WRITE_END, data_size, *WriteAddr );

    return M_OK;
}

int main( void )
{
    uint32_t write_addr;

    if ( device_initialize() != 0 ) {
        return -1;
    }
    s_loadRemapEnd();
    s_initIap();
    flash_init();

    write_addr = USER_AREA_ADDR;
    Xmodem_Init( XmodemBuf );

    /* Send "IAP\r\n" banner to announce we are alive. */
    s_flag   = FLAG_CLR;
    s_rwSize = sizeof(msgIapProg);
    uartf0_write( (uint8_t *)msgIapProg, s_rwSize, s_procUartfWrite );
    while ( s_flag == FLAG_CLR ) {
        wdt_clear();
    }

    /* Kick off XMODEM-CRC by sending 'C'. */
    Xmodem_SendByte( 'C' );

    for (;;) {
        wdt_clear();

        switch ( Xmodem_ReadStatus() ) {
        case RECV_END:
            if ( write_verify( &write_addr, XmodemBuf + 3, Xmodem_GetLastDataSize() ) != M_OK ) {
                error_proc();   /* never returns */
            } else {
                Xmodem_SendByte( ACK );
            }
            break;

        case EOT_END:
            /* Wait > 62.5 ms (two T16Hz ticks) so the final ACK actually
             * leaves the UART before we yank the address space out. */
            {
                uint8_t cnt;
                for ( cnt = 0; cnt < 2; cnt++ ) {
                    write_reg32( LTBC->LTBR, 0x00000000 );
                    while ( get_bit( LTBC->LTBR, 0x00000008 ) == 0 ) {}
                }
            }
            Remap_End();    /* writes REMAPCON, system reset, no return */
            break;

        case TIMEOUT_ERR:
        case RETRY_ERR:
        case SEND_ERR:
            error_proc();   /* never returns */
            break;

        default:
            break;
        }
    }
}
