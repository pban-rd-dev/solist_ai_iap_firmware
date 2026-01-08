/*****************************************************************************
 debug_uart.c

 UART Debug Interface Implementation for ML63Q2537
 - Uses UARTF0 peripheral
 - Non-blocking operation
 - Supports printf-style formatting

******************************************************************************/

#include "debug_uart.h"
#include "mcu.h"
#include "uartf0.h"
#include "rdwr_reg.h"
#include "irq.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*############################################################################*/
/*#                                 Defines                                  #*/
/*############################################################################*/

/* Buffer for printf formatting */
#define PRINTF_BUFFER_SIZE      256

/* UART configuration parameters - matching sample code */
/* For 115200 bps at SYSCLK 48MHz */
#define UARTF_PARAM_MODE        (UARTF_LG_8BIT | UARTF_STP_1BIT | UARTF_PT_NON | \
                                 UARTF_BC_DIS | UARTF_DLAB_RBR_THR | UARTF_FEN_ENA | \
                                 UARTF_RFR_KEEP | UARTF_TFR_KEEP | UARTF_FTL_2BYTE)
#define UARTF_PARAM_CAJ         (UARTF_RMV_ENA | 0x0019U)
#define UARTF_PARAM_DLR         (0x0019U)  /* baudrate : 115200bps at SYSCLK48MHz */

/* #define UARTF_PARAM_CAJ         (UARTF_RMV_ENA | 0x000DU) */
/* #define UARTF_PARAM_DLR         (0x000AU)  /\* baudrate : 115200bps at SYSCLK20MHz *\/ */

/*############################################################################*/
/*#                                Variables                                 #*/
/*############################################################################*/

static bool s_uart_initialized = false;
static char s_printf_buffer[PRINTF_BUFFER_SIZE];

/*############################################################################*/
/*#                            Private Functions                             #*/
/*############################################################################*/

/**
 * Configure GPIO pins for UART
 */
static void s_configure_gpio(void)
{
    /* Configure P33 (bit 3) and P32 (bit 2) for UART0 */
    /* P3MOD0 register controls pins P30-P33 */
    /* Each pin uses 6 bits: [5:0] for mode configuration */
    /* P30: bits [5:0], P31: bits [11:6], P32: bits [17:12], P33: bits [23:18] */
    
    /* Read current P3MOD0 value */
    uint32_t p3mod0 = read_reg32(PORT3->P3MOD0);
    
    /* Clear bits for P32 (bits 23:16) and P33 (bits 31:24) */
    p3mod0 &= ~((0x3F << 16) | (0x3F << 24));
    
    /* Set P32 as UARTF0_RX */
    p3mod0 |= (0x21 << 16);
    
    /* Set P33 as UARTF0_TX */
    p3mod0 |= (0x23 << 24);
    
    /* Write back to P3MOD0 */
    write_reg32(PORT3->P3MOD0, p3mod0);
    
    /* Set P33 output high (idle state for UART TX) */
    set_bit(PORT3->P3DO, (1 << 3));
}

/* /\** */
/*  * Simple strlen implementation */
/*  *\/ */
/* static uint32_t s_strlen(const char *str) */
/* { */
/*     uint32_t len = 0; */
/*     while (*str++) { */
/*         len++; */
/*     } */
/*     return len; */
/* } */

/**
 * Simple itoa for hex conversion
 */
static void s_hex_to_str(uint8_t value, char *str)
{
    const char hex_chars[] = "0123456789ABCDEF";
    str[0] = hex_chars[(value >> 4) & 0x0F];
    str[1] = hex_chars[value & 0x0F];
    str[2] = '\0';
}

/*############################################################################*/
/*#                              Public Functions                            #*/
/*############################################################################*/

/**
 * Initialize debug UART
 */
bool debug_uart_init(void)
{
    if (s_uart_initialized) {
        return true;
    }
    
    /* Configure GPIO pins */
    s_configure_gpio();
    
    /* Initialize UARTF0 */
    uartf0_init((uint16_t)UARTF_PARAM_MODE,(uint16_t)UARTF_PARAM_CAJ, (uint16_t)UARTF_PARAM_DLR);
    s_uart_initialized = true;
    /* enable interrupt */
    irq_uaf0_clearIRQ();
    irq_uaf0_ena();
    
    /* Send initialization message */
    debug_uart_puts("\r\n=== Debug UART Initialized ===\r\n");
    debug_uart_printf("UART0: %d baud, 8N1\r\n", DEBUG_UART_BAUD_RATE);
    debug_uart_printf("Pins: TX=P33, RX=P32\r\n");
    
    return true;
}

/**
 * Deinitialize debug UART
 */
void debug_uart_deinit(void)
{
    if (!s_uart_initialized) {
        return;
    }
    
    /* Wait for any pending transmission */
    debug_uart_flush();
    
    /* Reset GPIO pins to default state (GPIO input) */
    uint32_t p3mod0 = read_reg32(PORT3->P3MOD0);
    
    /* Clear bits for P32 (bits 17:12) and P33 (bits 23:18) */
    p3mod0 &= ~((0x3F << 12) | (0x3F << 18));
    
    /* Set both pins as GPIO input (0x0A) */
    p3mod0 |= (0x0A << 12) | (0x0A << 18);
    
    /* Write back to P3MOD0 */
    write_reg32(PORT3->P3MOD0, p3mod0);
    
    s_uart_initialized = false;
}

/**
 * Send a single character
 */
void debug_uart_putc(char c)
{
    if (!s_uart_initialized) {
        return;
    }
    
    /* Wait until transmit buffer is ready */
    while (uartf0_checkWriteBusy()) {
        /* Busy wait */
    }
    
    /* Send character */
    uartf0_putc((uint8_t)c);
}

/**
 * Send a string
 */
void debug_uart_puts(const char *str)
{
    if (!s_uart_initialized || !str) {
        return;
    }
    
    while (*str) {
        debug_uart_putc(*str++);
    }
}

/**
 * Send formatted string
 */
int debug_uart_printf(const char *format, ...)
{
    if (!s_uart_initialized || !format) {
        return 0;
    }
    
    va_list args;
    int len;
    
    /* Format string into buffer */
    va_start(args, format);
    len = vsnprintf(s_printf_buffer, PRINTF_BUFFER_SIZE, format, args);
    va_end(args);
    
    /* Ensure null termination */
    if (len >= PRINTF_BUFFER_SIZE) {
        len = PRINTF_BUFFER_SIZE - 1;
    }
    s_printf_buffer[len] = '\0';
    
    /* Send formatted string */
    debug_uart_puts(s_printf_buffer);
    
    return len;
}

/**
 * Send hex dump of data
 */
void debug_uart_hex_dump(const uint8_t *data, uint32_t len, const char *prefix)
{
    if (!s_uart_initialized || !data || len == 0) {
      DEBUG_INFO("hex dump returns soon");
        return;
    }
    
    char hex_str[3];
    uint32_t i;
    
    if (prefix) {
        debug_uart_puts(prefix);
        debug_uart_puts(": ");
    }

    for (i = 0; i < len; i++) {
        if (i > 0 && (i % 16) == 0) {
            debug_uart_puts("\r\n");
            if (prefix) {
                debug_uart_puts(prefix);
                debug_uart_puts(": ");
            }
        } else if (i > 0 && (i % 8) == 0) {
            debug_uart_puts("  ");
        } else if (i > 0) {
            debug_uart_putc(' ');
        }
        
        s_hex_to_str(data[i], hex_str);
        debug_uart_puts(hex_str);
    }
    
    debug_uart_puts("\r\n");
}

/**
 * Check if UART is ready
 */
bool debug_uart_is_ready(void)
{
    if (!s_uart_initialized) {
        return false;
    }
    
    return !uartf0_checkWriteBusy();
}

/**
 * Flush UART transmit buffer
 */
void debug_uart_flush(void)
{
    if (!s_uart_initialized) {
        return;
    }
    
    /* Wait until transmit complete */
    while (uartf0_checkWriteBusy()) {
        /* Busy wait */
    }
    
    /* Wait for UART transmit empty flag */
    while (!(uartf0_getStatus() & UAFnLSR_UFnTEMT)) {
        /* Busy wait */
    }
}

/**
 * UARTF interrupt routine
 *
 * @param           -
 * @return          None
 */
void debug_procUartfInt( void )
{
	uint32_t intStat;

	intStat = uartf0_getIntCause() & UARTF_IRID_MASK;
	switch( intStat ) {
	case UARTF_IRID_READ_REQ:
	case UARTF_IRID_CHAR_TIMEOUT:
	case UARTF_IRID_DATA_ERR:
		uartf0_continueRead();
		break;
	case UARTF_IRID_WRITE_REQ:
	case UARTF_IRID_TRANS_COMP:
		uartf0_continueWrite( (uint16_t)intStat );
		break;
	default:
		break;
	}
}
