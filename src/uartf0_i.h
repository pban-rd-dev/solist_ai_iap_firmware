/*****************************************************************************
 * @file    uartf0_i.h
 * @brief   Interrupt-driven UARTF (channel 0) API
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample).
 * Wraps the UARTF0 peripheral with callback-based async read/write that the
 * XMODEM state machine drives. Different from driver/inc/uartf0.h, which is
 * the lower-level register interface.
 *****************************************************************************/

#ifndef UARTF0_I_H__
#define UARTF0_I_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "uartf_common_i.h"

#define uartf0_getc()             read_reg32( UARTF0->UAF0BUF )
#define uartf0_putc( data )       write_reg32( UARTF0->UAF0BUF, data )
#define uartf0_checkWriteBusy()   ( ( ! get_bit( UARTF0->UAF0LSR, (1 << 5) )) )
#define uartf0_checkReadReady()   ( get_bit( UARTF0->UAF0LSR, (1 << 0) ) )
#define uartf0_getIntCause()      read_reg32( UARTF0->UAF0IIR )
#define uartf0_getStatus()        read_reg32( UARTF0->UAF0LSR )
#define uartf0_clearRbrIntCause() uartf0_getc()
#define uartf0_clearWriteFifo()   ( set_bit( UARTF0->UAF0MOD, (1 << 10) ) )
#define uartf0_clearReadFifo()    ( set_bit( UARTF0->UAF0MOD, (1 <<  9) ) )

void    uartf0_init( uint16_t uafnmod, uint16_t uafncaj, uint16_t brDivisorLatch );
void    uartf0_write( uint8_t *data, uint32_t size, cbfUartF_t func );
#ifdef UART_BYTE_CALLBACK
void    uartf0_read( uint8_t *data, uint32_t size, cbfUartFByte_t func1Byte, cbfUartF_t func );
#else
void    uartf0_read( uint8_t *data, uint32_t size, cbfUartF_t func );
#endif
int32_t  uartf0_continueWrite( uint16_t intStatus );
int32_t  uartf0_continueRead( void );
void     uartf0_stopWrite( void );
void     uartf0_stopRead( void );

/* Bytes received in the current uartf0_read() session. Reset to 0 on every
 * uartf0_read() call. Callers that need to act on a byte-count boundary
 * (e.g. variable-size XMODEM blocks) must use this rather than counting
 * per-byte callback invocations, which fire once per IRQ — not once per byte. */
uint32_t uartf0_getReadCount( void );

#endif /* UARTF0_I_H__ */
