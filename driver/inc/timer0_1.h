/*****************************************************************************
 timer0_1.h

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
 * @file    timer0_1.h
 *
 * This module is 'Timer0_1' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Timer
 *  @{
 *
 *  @brief Timers register related
 *
 *  @file
 */

#ifndef TIMER0_1_H__
#define TIMER0_1_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "timer_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize timer0
 *
 * @param[in]   tmncon  A value of setting TM0CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : TM_MODE_16BIT, or TM_MODE_32BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer0_init( tmncon )   write_reg32( TIMER->TM0CON, tmncon )
/**
 * Initialize timer1
 *
 * @param[in]   tmncon  A value of setting TM1CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : Only TM_MODE_16BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer1_init( tmncon )   write_reg32( TIMER->TM1CON, tmncon )
/**
 * Start timer0
 *
 * @param       -
 * @return      None
 */
#define         timer0_start()          set_bit( TIMER->TMSTR, (1 << 0) )
/**
 * Start timer1
 *
 * @param       -
 * @return      None
 */
#define         timer1_start()          set_bit( TIMER->TMSTR, (1 << 1) )
/**
 * Stop timer0
 *
 * @param       -
 * @return      None
 */
#define         timer0_stop()           set_bit( TIMER->TMSTP, (1 << 0) )
/**
 * Stop timer1
 *
 * @param       -
 * @return      None
 */
#define         timer1_stop()           set_bit( TIMER->TMSTP, (1 << 1) )
/**
 * Clear counter of timer0
 *
 * @param       -
 * @return      None
 */
#define         timer0_clrCnt()         write_reg32( TIMER->TM0C, 0x00 )
/**
 * Clear counter of timer1
 *
 * @param       -
 * @return      None
 */
#define         timer1_clrCnt()         write_reg32( TIMER->TM1C, 0x00 )
/**
 * Get counter value of timer0
 *
 * @param       -
 * @return      The value of timer0 counter
 */
#define         timer0_getCnt()         read_reg32( TIMER->TM0C )
/**
 * Get counter value of timer1
 *
 * @param       -
 * @return      The value of timer1 counter
 */
#define         timer1_getCnt()         read_reg32( TIMER->TM1C )
/**
 * Get the status of timer0
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer0_getStatus()      get_bit( TIMER->TMSTAT, (1 << 0) )
/**
 * Get the status of timer1
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer1_getStatus()      get_bit( TIMER->TMSTAT, (1 << 1) )

void            timer0_setCnt( uint32_t cnt );
void            timer1_setCnt( uint32_t cnt );

#endif /*TIMER0_1_H__*/

/** @} */
/** @} */
