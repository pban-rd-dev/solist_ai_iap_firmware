/*****************************************************************************
 uartf2.h

 Copyright (C) 2024 ROHM Co., Ltd.
 All rights reserved.

 This software is provided "as is" and any expressed or implied
 warranties, including, but not limited to, the implied warranties of
 merchantability and fitness for a particular purpose are disclaimed.
 ROHM shall not be liable for any direct, indirect, consequential or
 incidental damages arising from using or modifying this software.
 You (customer) can modify and use this software in whole or part on
 your own responsibility, only for the purpose of developing the software
 for use with microcontroller manufactured by ROHM.

 History
    2024.07.31 Ver 1.0.0

******************************************************************************/
/**
 * @file    uartf2.h
 *
 * This module is 'UARTF2' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup UARTF
 *  @{
 *
 *  @brief UART with FIFO register related
 *
 *  @file
 */

#ifndef UARTF2_H__
#define UARTF2_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "uartf_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Getting reception data
 *
 * @param       -
 * @return      A reception data <br>
 *              (A value of UAF0BUF register)
 */
#define         uartf2_getc()           read_reg32( UARTF2->UAF0BUF )

/**
 * Sending transmission of data
 *
 * @param[in]   data    A transmission data
 *              (Setting value of UAF0BUF register)
 * @return  None
 */
#define         uartf2_putc( data )     write_reg32( UARTF2->UAF0BUF, data )

/**
 * Checking UARTF(Writing) busy status
 *
 * @param       -
 * @retval      0                busy status : Normal(able to write)
 * @retval      "other than 0"   busy status : Busy
 */
#define         uartf2_checkWriteBusy() ( ( ! get_bit( UARTF2->UAF0LSR, (1 << 5) )) )

/**
 * Checking UARTF(Reading) ready status
 *
 * @param       -
 * @retval      0                ready status : Normal
 * @retval      "other than 0"   ready status : Ready(able to read, and reception data is valid)
 */
#define         uartf2_checkReadReady() ( get_bit( UARTF2->UAF0LSR, (1 << 0) ) )

/**
 * Getting interrupt cause
 *
 * Get causes which UARTF interrupt is requested.
 *
 * @param       -
 * @return      Interrupt causes <br>
 *              (A value of UAF0IIR register)
 */
#define         uartf2_getIntCause()    read_reg32( UARTF2->UAF0IIR )

/**
 * Getting UARTF status
 *
 *
 * @param       -
 * @return      UARTF line status <br>
 *              (A value of UAF0LSR register)
 */
#define         uartf2_getStatus()      read_reg32( UARTF2->UAF0LSR )

/**
 * Clear RBR interrupt cause
 *
 * Read RBR to clear interrupt cause. <br>
 * ex. Character timeout is occurred.
 *
 * @param       -
 * @return      RBR stored data(Dummy read) <br>
 *              (A value of UAF0BUFL register)
 */
#define         uartf2_clearRbrIntCause() uartf2_getc()

/**
 * Crear Write FIFO
 *
 * @param       -
 * @return      None
 */
 #define        uartf2_clearWriteFifo()   (set_bit( UARTF2->UAF0MOD, (1 << 10) ))

/**
 * Crear Read FIFO
 *
 * @param       -
 * @return      None
 */
 #define        uartf2_clearReadFifo()    (set_bit( UARTF2->UAF0MOD, (1 << 9) ))

void            uartf2_init( uint16_t uafnmod, uint16_t uafncaj, uint16_t brDivisorLatch );
void            uartf2_write( uint8_t *data, uint32_t size, cbfUartF_t func );
void            uartf2_read( uint8_t *data, uint32_t size, cbfUartF_t func );
int32_t         uartf2_continueWrite( uint16_t intStatus );
int32_t         uartf2_continueRead( void );
void            uartf2_stopWrite( void );
void            uartf2_stopRead( void );

#endif /*UARTF2_H__*/

/** @} */
/** @} */
