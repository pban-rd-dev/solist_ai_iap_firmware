/*****************************************************************************
 timer4_5.h

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
 * @file    timer4_5.h
 *
 * This module is 'Timer4_5' drivers.
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

#ifndef TIMER4_5_H__
#define TIMER4_5_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "timer_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize timer4
 *
 * @param[in]   tmncon  A value of setting TM4CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : TM_MODE_16BIT, or TM_MODE_32BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer4_init( tmncon )   write_reg32( TIMER->TM4CON, tmncon )
/**
 * Initialize timer5
 *
 * @param[in]   tmncon  A value of setting TM5CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : Only TM_MODE_8BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer5_init( tmncon )   write_reg32( TIMER->TM5CON, tmncon )
/**
 * Start timer4
 *
 * @param       -
 * @return      None
 */
#define         timer4_start()          set_bit( TIMER->TMSTR, (1 << 4) )
/**
 * Start timer5
 *
 * @param       -
 * @return      None
 */
#define         timer5_start()          set_bit( TIMER->TMSTR, (1 << 5) )
/**
 * Stop timer4
 *
 * @param       -
 * @return      None
 */
#define         timer4_stop()           set_bit( TIMER->TMSTP, (1 << 4) )
/**
 * Stop timer5
 *
 * @param       -
 * @return      None
 */
#define         timer5_stop()           set_bit( TIMER->TMSTP, (1 << 5) )
/**
 * Clear counter of timer4
 *
 * @param       -
 * @return      None
 */
#define         timer4_clrCnt()         write_reg32( TIMER->TM4C, 0x00 )
/**
 * Clear counter of timer5
 *
 * @param       -
 * @return      None
 */
#define         timer5_clrCnt()         write_reg32( TIMER->TM5C, 0x00 )
/**
 * Get counter value of timer4
 *
 * @param       -
 * @return      The value of timer4 counter
 */
#define         timer4_getCnt()         read_reg32( TIMER->TM4C )
/**
 * Get counter value of timer5
 *
 * @param       -
 * @return      The value of timer5 counter
 */
#define         timer5_getCnt()         read_reg32( TIMER->TM5C )
/**
 * Get the status of timer4
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer4_getStatus()      get_bit( TIMER->TMSTAT, (1 << 4) )
/**
 * Get the status of timer5
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer5_getStatus()      get_bit( TIMER->TMSTAT, (1 << 5) )

void            timer4_setCnt( uint32_t cnt );
void            timer5_setCnt( uint32_t cnt );

#endif /*_TIMER4_5_H__*/

/** @} */
/** @} */
