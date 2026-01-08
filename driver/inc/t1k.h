/*****************************************************************************
 t1k.h

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
 * @file    t1k.h
 *
 * This module is '1kHz Timer' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Timer1kHz
 *  @{
 *
 *  @brief 1kHz Timer register related
 *
 *  @file
 */

#ifndef T1K_H__
#define T1K_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*--- bit field of registers ---*/
#define T1KCON_T1KRUN		( 0x01 )			/**< T1KCON Register T1KRUN bit    */
#define T1KCON_T1KSEL0		( 0x02 )			/**< T1KCON Register T1KSEL[0] bit */
#define T1KCON_T1KSEL1		( 0x04 )			/**< T1KCON Register T1KSEL[1] bit */
#define T1KCON_T1KSEL2		( 0x08 )			/**< T1KCON Register T1KSEL[2] bit */

/*=== API parameters value ===*/
/*---	1kHz timer Interrupt mode	---*/
#define T1KSEL_10HZ			( 0x00 )			/**< interrupt period : 10Hz	*/
#define T1KSEL_20HZ			( T1KCON_T1KSEL0 )	/**< interrupt period : 20Hz	*/
#define T1KSEL_40HZ			( T1KCON_T1KSEL2 )	/**< interrupt period : 40Hz	*/
#define T1KSEL_60HZ			( T1KCON_T1KSEL2 | T1KCON_T1KSEL0 )	/**< interrupt period : 60Hz	*/
#define T1KSEL_80HZ			( T1KCON_T1KSEL2 | T1KCON_T1KSEL1 )	/**< interrupt period : 80Hz	*/
#define T1KSEL_1HZ			( T1KCON_T1KSEL2 | T1KCON_T1KSEL1 | T1KCON_T1KSEL0 )	/**< interrupt period : 1Hz	*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * 1kHz timer initialize
 *
 * @param[in]   mode        interrupt mode : T1KSEL_10HZ to T1KSEL_1HZ
 * @return      None
 */
#define         t1k_init( mode )     do {                                                    \
                                        clear_bit( TIMER1K->T1KCON, (1 << 0) );                       \
                                        write_reg32( TIMER1K->T1KCON, ((mode) & 0x0E) );  \
                                     } while (0)

/**
 * Start 1kHz timer
 *
 * @param[in]   -
 * @return      None
 */
#define         t1k_start()        set_bit( TIMER1K->T1KCON, (1 << 0) )

/**
 * Stop 1kHz timer
 *
 * @param[in]   -
 * @return      None
 */
#define         t1k_stop()        clear_bit( TIMER1K->T1KCON, (1 << 0) )

/**
 * Get counter value of 1kHz timer
 *
 * @param       -
 * @return      The value of 1kHz timer counter <br>
 *              (A value of T1KCR register)
 */
#define         t1k_getT1KCR()      read_reg32( TIMER1K->T1KCR )

void            t1k_clrT1KCR( void );

#endif /*T1K_H__*/

/** @} */
/** @} */
