/*****************************************************************************
 * @file    uartf0_i.c
 * @brief   Interrupt-driven UARTF (channel 0) implementation
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample).
 *****************************************************************************/

#include "mcu.h"
#include "rdwr_reg.h"
#include "uartf0_i.h"

#define UARTFn_WR_FIFO_MAX  ( 4 )

static uartfCtrlParam_t s_writeCtrlParam;
static uartfCtrlParam_t s_readCtrlParam;

__INLINE static void s_writeSingleData( uartfCtrlParam_t *param )
{
    uartf0_putc( *param->data );
    param->data++;
    param->cnt++;
}

__INLINE static void s_writeFifo( uartfCtrlParam_t *param )
{
    uint8_t wr_size = param->blockSize;
    if ( (param->cnt + wr_size) > param->size ) {
        wr_size = (uint8_t)(param->size - param->cnt);
    }
    while ( uartf0_checkWriteBusy() != 0 ) {}
    for ( ; wr_size != 0; wr_size-- ) {
        s_writeSingleData( param );
    }
}

__INLINE static uint16_t s_readSingleData( uartfCtrlParam_t *param )
{
    uint16_t status = (uint16_t)uartf0_getStatus();
    *param->data = (uint8_t)uartf0_getc();
    param->data++;
    param->cnt++;
    return status;
}

__INLINE static uint16_t s_readFifo( uartfCtrlParam_t *param )
{
    uint16_t status = 0;
    uint8_t rd_size = param->blockSize;
    if ( (param->cnt + rd_size) > param->size ) {
        rd_size = (uint8_t)(param->size - param->cnt);
    }
    for ( ; rd_size != 0; rd_size-- ) {
        if ( uartf0_checkReadReady() == 1 ) {
            status |= s_readSingleData( param );
        }
    }
    return status;
}

void uartf0_init( uint16_t uafnmod, uint16_t uafncaj, uint16_t brDivisorLatch )
{
    set_bit( UARTF0->UAF0MOD, (1 << 7) );
    write_reg32( UARTF0->UAF0BUF, brDivisorLatch );
    clear_bit( UARTF0->UAF0MOD, (1 << 7) );
    write_reg32( UARTF0->UAF0CAJ, uafncaj );

    uafnmod |= UAFnMOD_UFnFEN;
    write_reg32( UARTF0->UAF0MOD, uafnmod );
    write_reg32( UARTF0->UAF0IER, (uint16_t)(UARTF_ERBFI_DIS | UARTF_ETBEI_DIS | UARTF_ELSI_DIS) );
    s_writeCtrlParam.blockSize = (uint8_t)UARTFn_WR_FIFO_MAX;
    s_readCtrlParam.blockSize  = (uint8_t)( (read_reg32( UARTF0->UAF0MOD ) >> 12) & 0x3 );
    s_readCtrlParam.blockSize++;

    uartf0_clearReadFifo();
    uartf0_clearWriteFifo();
    uartf0_getStatus();
    uartf0_getIntCause();
}

void uartf0_write( uint8_t *data, uint32_t size, cbfUartF_t func )
{
    s_writeCtrlParam.data     = data;
    s_writeCtrlParam.size     = size;
    s_writeCtrlParam.cnt      = 0;
    s_writeCtrlParam.callBack = func;
    s_writeCtrlParam.errStat  = 0;

    s_writeFifo( &s_writeCtrlParam );
    set_reg32( UARTF0->UAF0IER, UARTF_ETBEI_ENA );
}

#ifdef UART_BYTE_CALLBACK
void uartf0_read( uint8_t *data, uint32_t size, cbfUartFByte_t func1Byte, cbfUartF_t func )
#else
void uartf0_read( uint8_t *data, uint32_t size, cbfUartF_t func )
#endif
{
    uartf0_getStatus();

    s_readCtrlParam.data         = data;
    s_readCtrlParam.size         = size;
    s_readCtrlParam.cnt          = 0;
    s_readCtrlParam.callBack     = func;
#ifdef UART_BYTE_CALLBACK
    s_readCtrlParam.callBackByte = func1Byte;
#endif
    s_readCtrlParam.errStat      = 0;

    set_reg32( UARTF0->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
}

int32_t uartf0_continueWrite( uint16_t intStatus )
{
    int32_t ret = UARTF_R_TRANS_CONT_OK;

    intStatus = intStatus & UARTF_IRID_MASK;
    if ( (intStatus != UARTF_IRID_WRITE_REQ) && (intStatus != UARTF_IRID_TRANS_COMP) ) {
        return UARTF_R_TRANS_CONT_OK;
    }
    if ( s_writeCtrlParam.size > s_writeCtrlParam.cnt ) {
        s_writeFifo( &s_writeCtrlParam );
        ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
    } else {
        if ( intStatus == UARTF_IRID_WRITE_REQ ) {
            clear_reg32( UARTF0->UAF0IER, UARTF_ETBEI_ENA );
            set_reg32(   UARTF0->UAF0IER, UARTF_TEMTI_ENA );
            ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
        } else {
            clear_reg32( UARTF0->UAF0IER, (UARTF_ETBEI_ENA | UARTF_TEMTI_ENA) );
            if ( s_writeCtrlParam.callBack != (void *)0 ) {
                s_writeCtrlParam.callBack( s_writeCtrlParam.cnt, s_writeCtrlParam.errStat );
            }
            ret = (int32_t)( UARTF_R_TRANS_FIN );
        }
    }
    return ret;
}

int32_t uartf0_continueRead( void )
{
    int32_t  ret;
    uint16_t status;
    uint8_t  flgReadEnd = 0;

    status = (uint16_t)uartf0_getStatus();
    if ( s_readCtrlParam.size > s_readCtrlParam.cnt ) {
        status |= s_readFifo( &s_readCtrlParam );
        s_readCtrlParam.errStat |= status & (uint16_t)(UAFnLSR_UFnFER|UAFnLSR_UFnOER|UAFnLSR_UFnPER|UAFnLSR_UFnBI);

#ifdef UART_BYTE_CALLBACK
        if ( s_readCtrlParam.callBackByte != (void *)0 ) {
            if ( s_readCtrlParam.callBackByte( *(s_readCtrlParam.data - 1), s_readCtrlParam.errStat ) == UART_R_STOP ) {
                clear_reg32( UARTF0->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
                return (int32_t)( UARTF_R_TRANS_FIN );
            }
        }
#endif
        if ( s_readCtrlParam.size <= s_readCtrlParam.cnt ) {
            flgReadEnd = 1;
        } else if ( ( status & ( uint16_t )( UAFnLSR_UFnFER ) ) != 0 ) {
            flgReadEnd = 1;
        }
    } else {
        s_readCtrlParam.errStat |= status;
        uartf0_clearReadFifo();
        flgReadEnd = 1;
    }

    ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
    if ( flgReadEnd != 0 ) {
        clear_reg32( UARTF0->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
        ret = (int32_t)( UARTF_R_TRANS_FIN );
        if ( s_readCtrlParam.callBack != (void *)0 ) {
            s_readCtrlParam.callBack( s_readCtrlParam.cnt, s_readCtrlParam.errStat );
        }
    }
    return ret;
}

void uartf0_stopWrite( void )
{
    clear_reg32( UARTF0->UAF0IER, (UARTF_ETBEI_ENA | UARTF_TEMTI_ENA) );
    uartf0_clearWriteFifo();
    s_writeCtrlParam.data     = (void *)0;
    s_writeCtrlParam.size     = 0;
    s_writeCtrlParam.cnt      = 0;
    s_writeCtrlParam.callBack = (void *)0;
    s_writeCtrlParam.errStat  = 0;
}

uint32_t uartf0_getReadCount( void )
{
    return s_readCtrlParam.cnt;
}

void uartf0_stopRead( void )
{
    clear_reg32( UARTF0->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
    uartf0_clearReadFifo();
    s_readCtrlParam.data         = (void *)0;
    s_readCtrlParam.size         = 0;
    s_readCtrlParam.cnt          = 0;
    s_readCtrlParam.callBack     = (void *)0;
#ifdef UART_BYTE_CALLBACK
    s_readCtrlParam.callBackByte = (void *)0;
#endif
    s_readCtrlParam.errStat      = 0;
    uartf0_getStatus();
}
