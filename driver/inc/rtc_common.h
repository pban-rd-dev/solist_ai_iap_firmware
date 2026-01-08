/*****************************************************************************
 rtc_common.h

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
 * @file    rst.h
 *
 * This module is 'Reset' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Reset
 *  @{
 *
 *  @brief Real Time Clock register related
 *
 *  @file
 */

#ifndef RTC_COMMON_H__
#define RTC_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*--- bit field of registers ---*/
#define RTCCON_RTCEN			0x00000001                           /**< RTCCON Register RTCEN bit                              */
#define RTCCON_RIN0				0x00000002                           /**< RTCCON Register RIN[0] bit                             */
#define RTCCON_RIN1				0x00000004                           /**< RTCCON Register RIN[1] bit                             */
#define RTCINTST_RTCINT			0x00000001                           /**< RTCINTST Register RTCINT bit                           */
#define RTCINTST_AL0INT			0x00000002                           /**< RTCINTST Register AL0INT bit                           */
#define RTCINTST_AL1INT			0x00000004                           /**< RTCINTST Register AL1INT bit                           */

/*=== API return value ===*/
/*--- General ---*/
#define RTC_R_OK				(   0 )                              /**< Status of OK                                           */
#define RTC_R_ERR_SEC			(  -1 )                              /**< Noticed error : second                                 */
#define RTC_R_ERR_MIN			(  -2 )                              /**< Noticed error : minute                                 */
#define RTC_R_ERR_HOUR			(  -3 )                              /**< Noticed error : hour                                   */
#define RTC_R_ERR_WEEK			(  -4 )                              /**< Noticed error : week                                   */
#define RTC_R_ERR_DAY			(  -5 )                              /**< Noticed error : day                                    */
#define RTC_R_ERR_MON			(  -6 )                              /**< Noticed error : month                                  */
#define RTC_R_ERR_YEAR			(  -7 )                              /**< Noticed error : year                                   */
#define RTC_R_ERR_ALEN			(  -8 )                              /**< Noticed error : alarm n                                */
#define RTC_R_ERR_RIN			(  -9 )                              /**< Noticed error : regular int                            */
#define RTC_R_ERR_GETTIME		( -10 )                              /**< Noticed error : get time                               */

/*=== API parameters value ===*/
/*--- rtc_setRegularInt() ---*/
/* rin */
#define RTC_RIN_DISABLE			( 0x00u )                            /**< Regular Int disable                                    */
#define RTC_RIN_0_5_SEC			( 0x01u )                            /**< Regular Int 0.5[sec]                                   */
#define RTC_RIN_1_0_SEC			( 0x02u )                            /**< Regular Int 1.0[sec]                                   */
#define RTC_RIN_1_0_MIN			( 0x03u )                            /**< Regular Int 1.0[min]                                   */
/* alen */
#define RTC_ALEN_DIS			( 0 )                                /**< Alarm N disable                                        */
#define RTC_ALEN_ENA			( 1 )                                /**< Alarm N enable                                         */

/*=== Week day value. ===*/
#define RTC_WEEK_DAY_SUN		( 0x01u )                            /**< Week day : sunday                                      */
#define RTC_WEEK_DAY_MON		( 0x02u )                            /**< Week day : monday                                      */
#define RTC_WEEK_DAY_TUE		( 0x03u )                            /**< Week day : tuesday                                     */
#define RTC_WEEK_DAY_WED		( 0x04u )                            /**< Week day : wednesday                                   */
#define RTC_WEEK_DAY_THU		( 0x05u )                            /**< Week day : thursday                                    */
#define RTC_WEEK_DAY_FRI		( 0x06u )                            /**< Week day : friday                                      */
#define RTC_WEEK_DAY_SAT		( 0x07u )                            /**< Week day : saturday                                    */

/*=== IRQ STAT value. ===*/
#define RTC_RTC_INT				( RTCINTST_RTCINT )                  /**< RTC INT                                                */
#define RTC_AL0_INT				( RTCINTST_AL0INT )                  /**< AL0 INT                                                */
#define RTC_AL1_INT				( RTCINTST_AL1INT )                  /**< AL1 INT                                                */
#define RTC_INT_ALL				( RTCINTST_AL1INT | RTCINTST_AL0INT | RTCINTST_RTCINT ) /**< All Interrupt                                          */

/*############################################################################*/
/*#                                Typedef                                   #*/
/*############################################################################*/
/**
 * @brief Date&Time setting parameter.
 */
typedef struct {
	uint8_t			sec;	/**< Number of seconds(00-59).*/
	uint8_t			min;	/**< Number of minutes(00-59).*/
	uint8_t			hour;	/**< Time from 0 o'clock(00-23).*/
	uint8_t			week;	/**< A day of the week(01-07).*/
	uint8_t			day;	/**< Date in month(01-31).*/
	uint8_t			mon;	/**< Month in year(01-12).*/
	uint8_t			year;	/**< Years(00-99).*/
} tRtcTime;

/**
 * @brief Alarm0 setting parameter.
 */
typedef struct {
	uint8_t			min;	/**< Number of minutes(00-59). */
	uint8_t			hour;	/**< Time from 0 o'clock(00-23). */
	uint8_t			week;	/**< Week(00-07). */
} tRtcAlarm0;

/**
 * @brief Alarm1 setting parameter.
 */
typedef struct {
	uint8_t			min;	/**< Number of minutes(00-59). */
	uint8_t			hour;	/**< Time from 0 o'clock(00-23). */
	uint8_t			day;	/**< Date in month(00-31). [*]0 : The day is not used for the comparison. */
	uint8_t			mon;	/**< Month in year(00-12). [*]0 : The month is not used for the comparison. */
} tRtcAlarm1;


#endif /*RTC_H__*/

/** @} */
/** @} */
