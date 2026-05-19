/*****************************************************************************
 * @file    app_irq.c
 * @brief   Application-level IRQ handlers for the IAP sample
 *****************************************************************************/

#include "mcu.h"
#include "uartf0_i.h"
#include "tbc.h"
#include "xmodem.h"

void EXI_IRQHandler( void );
void NMI_Handler( void );
void UAF0_IRQHandler( void );
void LTBC_IRQHandler( void );

void EXI_IRQHandler( void )
{
}

void NMI_Handler( void )
{
}

void UAF0_IRQHandler( void )
{
    uint32_t intStat = uartf0_getIntCause() & UARTF_IRID_MASK;
    switch ( intStat ) {
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

void LTBC_IRQHandler( void )
{
    Xmodem_CountTimeOut();
    tbc_clearIntStat( TBC_INTST_LTBINT0 );
}
