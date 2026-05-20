/*****************************************************************************
 * @file    xmodem.c
 * @brief   XMODEM-CRC state machine
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample). The only change
 * from the original is the renamed CRC header (crc.h -> xmodem_crc.h) so it
 * does not collide with driver/inc/crc.h.
 *****************************************************************************/

#include "mcu.h"
#include "irq.h"
#include "uartf0_i.h"
#include "xmodem_crc.h"
#include "xmodem.h"
#include "main.h"

static uint8_t  XmodemStatus;
static uint8_t  XmodemRetryCnt;
static uint8_t  XmodemBlockNum;
static uint8_t  XmodemSendData;
static uint16_t XmodemTimeoutCnt;
static uint16_t XmodemTimeoutRetryCnt;

static uint8_t *XmodemBuf;

/* Per-block state for the dual-size receive path. Reset on init and on every
 * block boundary (success or error). Actual received-byte count comes from
 * the UART driver via uartf0_getReadCount() — the per-byte callback fires
 * once per IRQ (not once per byte), so we cannot count it ourselves. */
static uint16_t s_block_size = 0;       /* total bytes in current block: 133 (SOH) or 1029 (STX) */
static uint16_t s_last_data_size = 0;   /* data bytes (128 or 1024) of last block, for main */

#define TIMEOUT_CNT     (1280)    /* 128 Hz tick * 1280 = 10 s */
#define RETRY_CNT       (10)

static int32_t Xmodem_CheckBlockData( uint8_t *RecvData, uint8_t block_num, uint16_t data_size );
static void    Xmodem_StartTimeOut( void );
static void    Xmodem_StopTimeOut( void );
static void    Xmodem_SendByteComplete( uint32_t size, uint16_t errStat );
static void    Xmodem_RecvBlockComplete( uint32_t size, uint16_t errStat );
static int8_t  Xmodem_RecvChar( uint8_t data, uint16_t errStat );
static void    Xmodem_RetryProc( void );

void Xmodem_Init( uint8_t *RecvBuf )
{
    XmodemBuf = (uint8_t *)RecvBuf;
    XmodemStatus = INIT_STATE;
    XmodemRetryCnt = 0U;
    XmodemBlockNum = 1U;
    XmodemSendData = 0U;
    XmodemTimeoutCnt = 0U;
    XmodemTimeoutRetryCnt = 0U;
    s_block_size = 0;
    s_last_data_size = 0;
}

uint16_t Xmodem_GetLastDataSize( void )
{
    return s_last_data_size;
}

static int32_t Xmodem_CheckBlockData( uint8_t *RecvData, uint8_t BlockNum, uint16_t data_size )
{
    uint16_t crc;
    uint16_t loopcnt;
    uint16_t tmp;

    if ( RecvData[0] + RecvData[1] != 0xff ) {
        return -1;
    }
    crc = 0;
    for ( loopcnt = 0; loopcnt < data_size; loopcnt++ ) {
        crc = UpdCRC16( (uint16_t)RecvData[loopcnt + 2], crc );
    }
    tmp  = (uint16_t)RecvData[data_size + 2] << 8;
    tmp |= (uint16_t)RecvData[data_size + 3];
    if ( crc != tmp ) {
        return -1;
    }
    if ( RecvData[0] != BlockNum ) {
        return -1;
    }
    return 0;
}

static void Xmodem_StartTimeOut( void )
{
    irq_tbc0_clearIRQ();
    irq_tbc0_ena();
    XmodemTimeoutCnt = 0;
}

static void Xmodem_StopTimeOut( void )
{
    irq_tbc0_dis();
}

extern uint8_t s_flag;

static void Xmodem_SendByteComplete( uint32_t Size, uint16_t ErrStat )
{
    (void)Size;
    (void)ErrStat;

    switch ( XmodemSendData ) {
    case 'C':
    case ACK:
    case NAK:
        if ( XmodemStatus != RECV_EOT ) {
            uartf0_read( XmodemBuf, XMODEM_BLOCK_SIZE, Xmodem_RecvChar, Xmodem_RecvBlockComplete );
            irq_uaf0_ena();
            Xmodem_StartTimeOut();
        } else {
            XmodemStatus = EOT_END;
        }
        break;
    case CAN:
        XmodemStatus = SEND_ERR;
        break;
    default:
        irq_uaf0_dis();
        XmodemStatus = SEND_ERR;
        Xmodem_SendByte( CAN );
        break;
    }
}

void Xmodem_SendByte( uint8_t send_data )
{
    XmodemSendData = send_data;
    if ( XmodemStatus != RECV_EOT ) {
        if ( (send_data == 'C') || (send_data == ACK) ) {
            XmodemStatus = RECV_STATE;
        }
    }
    uartf0_write( &XmodemSendData, 1, Xmodem_SendByteComplete );
    irq_uaf0_ena();
}

/* Dead in the new flow — Xmodem_RecvChar returns UART_R_STOP at the end of
 * every block, which prevents the driver from invoking this complete
 * callback. Kept as a no-op so the uartf0_read call site doesn't need to
 * special-case a NULL pointer. */
static void Xmodem_RecvBlockComplete( uint32_t Size, uint16_t ErrStat )
{
    (void)Size;
    (void)ErrStat;
}

/* Per-IRQ receive callback (the UART driver fires this once per IRQ, not
 * once per byte). On the first IRQ of a new block, XmodemBuf[0] holds the
 * header byte — we use it to decide whether the block is SOH (133 bytes)
 * or STX (1029 bytes). On subsequent IRQs we compare uartf0_getReadCount()
 * against the chosen block size; when reached, we validate and respond
 * inline and return UART_R_STOP so the driver tears down the read. */
static int8_t Xmodem_RecvChar( uint8_t Data, uint16_t ErrStat )
{
    uint32_t cnt;

    (void)Data;

    XmodemTimeoutCnt = 0;
    XmodemTimeoutRetryCnt = 0;

    if ( ErrStat != 0 ) {
        s_block_size = 0;
        Xmodem_StopTimeOut();
        Xmodem_RetryProc();
        return UART_R_STOP;
    }

    if ( s_block_size == 0 ) {
        /* First IRQ of this block — header has just arrived in XmodemBuf[0]. */
        switch ( XmodemBuf[0] ) {
        case EOT:
            XmodemStatus = RECV_EOT;
            Xmodem_SendByte( ACK );
            XmodemRetryCnt = 0;
            return UART_R_STOP;
        case SOH:
            s_block_size = 5 + 128;     /* hdr + blk# + ~blk# + 128 data + CRC16 */
            XmodemRetryCnt = 0;
            break;
        case STX:
            s_block_size = 5 + 1024;    /* hdr + blk# + ~blk# + 1024 data + CRC16 */
            XmodemRetryCnt = 0;
            break;
        default:
            irq_uaf0_dis();
            Xmodem_SendByte( CAN );
            Xmodem_RetryProc();
            return UART_R_STOP;
        }
    }

    cnt = uartf0_getReadCount();
    if ( cnt < s_block_size ) {
        return 0;       /* more IRQs / bytes to come */
    }

    /* Block complete — validate and respond inline. */
    uint16_t data_size = (uint16_t)(s_block_size - 5);
    s_block_size = 0;

    Xmodem_StopTimeOut();

    if ( Xmodem_CheckBlockData( XmodemBuf + 1, XmodemBlockNum, data_size ) != 0 ) {
        XmodemRetryCnt++;
        if ( XmodemRetryCnt > RETRY_CNT ) {
            Xmodem_SendByte( CAN );
        } else {
            if ( XmodemBlockNum == 1 ) {
                Xmodem_SendByte( 'C' );
            } else {
                Xmodem_SendByte( NAK );
            }
        }
    } else {
        s_last_data_size = data_size;
        XmodemStatus = RECV_END;
        XmodemBlockNum++;
        XmodemRetryCnt = 0;
    }

    return UART_R_STOP;
}

uint8_t Xmodem_ReadStatus( void )
{
    return XmodemStatus;
}

void Xmodem_CountTimeOut( void )
{
    XmodemTimeoutCnt++;
    if ( XmodemTimeoutCnt > TIMEOUT_CNT ) {
        XmodemTimeoutRetryCnt++;
        irq_uaf0_dis();

        if ( XmodemTimeoutRetryCnt > RETRY_CNT ) {
            Xmodem_StopTimeOut();
            XmodemTimeoutCnt = TIMEOUT_CNT;
            XmodemStatus = TIMEOUT_ERR;
            Xmodem_SendByte( CAN );
        } else {
            Xmodem_SendByte( XmodemSendData );
        }
    }
}

static void Xmodem_RetryProc( void )
{
    irq_uaf0_dis();

    XmodemRetryCnt++;
    if ( XmodemRetryCnt > RETRY_CNT ) {
        XmodemStatus = RETRY_ERR;
        Xmodem_SendByte( CAN );
    } else {
        Xmodem_SendByte( NAK );
    }
}
