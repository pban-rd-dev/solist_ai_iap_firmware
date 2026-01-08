/*****************************************************************************
 rtc0.h

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
 * @file    rtc0.h
 *
 * This module is 'TRC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup RTC
 *  @{
 *
 *  @brief Real Time Clock register related
 *
 *  @file
 */

#ifndef RTC0_H__
#define RTC0_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "rtc_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/**
 * Start RTC.
 *
 * @param       -
 * @return      -
 */
#define         rtc0_start()                 set_bit( RTC->RTCCON, (1 << 0) )

/**
 * Stop RTC.
 *
 * @param       -
 * @return      -
 */
#define         rtc0_stop()                  clear_bit( RTC->RTCCON, (1 << 0) )

/**
 * Clear interrupt status
 *
 * @param[in]   intst   Interrupt status <br>
 *                      Specify the logical sum of the following items, or RTC_INT_ALL.
 * @arg                     RTC_RTC_INT
 * @arg                     RTC_AL0_INT
 * @arg                     RTC_AL1_INT
 * @return      None
 */
#define         rtc0_clearStat( intst )      write_reg32( RTC->RTCINTST, (intst) )

/**
 * Get interrupt status
 *
 * @param       -
 * @return      Interrupt status <br>
 *              The logical sum of the following items
 * @arg                     RTC_RTC_INT
 * @arg                     RTC_AL0_INT
 * @arg                     RTC_AL1_INT
 */
#define         rtc0_getIntStat()            read_reg32( RTC->RTCINTST )

/**
 * Get operation status
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Running
 */
#define         rtc0_checkRunning()          get_bit( RTC->RTCCON, (1 << 0) )

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
int32_t	rtc0_setTime			( tRtcTime *prm );
int32_t	rtc0_getTime			( tRtcTime *prm );
int32_t	rtc0_setRegularInt		( uint8_t rin );
int32_t	rtc0_setAlarm0			( uint8_t alen, tRtcAlarm0* prm );
void	rtc0_getAlarm0			( tRtcAlarm0 *prm );
int32_t	rtc0_setAlarm1			( uint8_t alen, tRtcAlarm1* prm );
void	rtc0_getAlarm1			( tRtcAlarm1 *prm );
int32_t	rtc0_calcWeekday		( tRtcTime *prm );

#endif /*RTC0_H__*/

/** @} */
/** @} */
