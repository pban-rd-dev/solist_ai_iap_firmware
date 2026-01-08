/*****************************************************************************
 timer2_3.h

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
 * @file    timer2_3.h
 *
 * This module is 'Timer2_3' drivers.
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

#ifndef TIMER2_3_H__
#define TIMER2_3_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "timer_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize timer2
 *
 * @param[in]   tmncon  A value of setting TM2CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : TM_MODE_16BIT, or TM_MODE_32BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer2_init( tmncon )   write_reg32( TIMER->TM2CON, tmncon )
/**
 * Initialize timer3
 *
 * @param[in]   tmncon  A value of setting TM3CON register. <br>
 *                      Specify the logical sum of the following items.
 * @arg                     Clock source      : TM_CS_LSCLK, or TM_CS_OSCLK, or TM_CS_EXTCLK
 * @arg                     Division ratio    : TM_DIV1, or TM_DIV4, or TM_DIV8, or TM_DIV16, or TM_DIV32, or TM_DIV64
 * @arg                     Timer             : Only TM_MODE_8BIT
 * @arg                     Mode              : TM_OST_RELOAD, or TM_OST_ONESHOT
 * @return      None
 */
#define         timer3_init( tmncon )   write_reg32( TIMER->TM3CON, tmncon )
/**
 * Start timer2
 *
 * @param       -
 * @return      None
 */
#define         timer2_start()          set_bit( TIMER->TMSTR, (1 << 2) )
/**
 * Start timer3
 *
 * @param       -
 * @return      None
 */
#define         timer3_start()          set_bit( TIMER->TMSTR, (1 << 3) )
/**
 * Stop timer2
 *
 * @param       -
 * @return      None
 */
#define         timer2_stop()           set_bit( TIMER->TMSTP, (1 << 2) )
/**
 * Stop timer3
 *
 * @param       -
 * @return      None
 */
#define         timer3_stop()           set_bit( TIMER->TMSTP, (1 << 3) )
/**
 * Clear counter of timer2
 *
 * @param       -
 * @return      None
 */
#define         timer2_clrCnt()         write_reg32( TIMER->TM2C, 0x00 )
/**
 * Clear counter of timer3
 *
 * @param       -
 * @return      None
 */
#define         timer3_clrCnt()         write_reg32( TIMER->TM3C, 0x00 )
/**
 * Get counter value of timer2
 *
 * @param       -
 * @return      The value of timer2 counter
 */
#define         timer2_getCnt()         read_reg32( TIMER->TM2C )
/**
 * Get counter value of timer3
 *
 * @param       -
 * @return      The value of timer3 counter
 */
#define         timer3_getCnt()         read_reg32( TIMER->TM3C )
/**
 * Get the status of timer2
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer2_getStatus()      get_bit( TIMER->TMSTAT, (1 << 2) )
/**
 * Get the status of timer3
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Counting
 */
#define         timer3_getStatus()      get_bit( TIMER->TMSTAT, (1 << 3) )

void            timer2_setCnt( uint32_t cnt );
void            timer3_setCnt( uint32_t cnt );

#endif /*_TIMER2_3_H__*/

/** @} */
/** @} */
