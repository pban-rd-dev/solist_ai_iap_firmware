/*****************************************************************************
 rtc0.c

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
 * @file    rtc0.c
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "rtc0.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*--- Register bit. ---*/
/* RTCCON */
#define RTC0CON_RIN			( RTCCON_RIN1 | RTCCON_RIN0 )	/**< RTCCON register RIN0-1 bit */
/*--- Setting value. ---*/
#define RTC_MIN_INVALID		( 0x7Fu )	/**< Alarm disable value : min */
#define RTC_HOUR_INVALID	( 0x3Fu )	/**< Alarm disable value : hour */
#define RTC_DAY_INVALID		( 0x3Fu )	/**< Alarm disable value : day */
#define RTC_MON_INVALID		( 0x1Fu )	/**< Alarm disable value : month */
#define RTC_WEEK_INVALID	( 0xFFu )	/**< Alarm disable value : month */
#define RTC_WEEK_DISABLE	( 0x00u )	/**< Alarm disable value : week */

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
/*--- Table. ---*/
/**
 * For calculating the week. 
 * - [0][n] : common year
 * - [1][n] : leap year
 */
static const uint8_t	tblOutOfWeekday[2][12] = {
	{0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5},
	{0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6}
};
/**
 * For calculating the week. 
 * - [0][n] : common year
 * - [1][n] : leap year
 */
static const uint8_t	tblWeekdayNo[2][7] = {
	{RTC_WEEK_DAY_SUN, RTC_WEEK_DAY_MON, RTC_WEEK_DAY_TUE, RTC_WEEK_DAY_WED, RTC_WEEK_DAY_THU, RTC_WEEK_DAY_FRI, RTC_WEEK_DAY_SAT},
	{RTC_WEEK_DAY_SAT, RTC_WEEK_DAY_SUN, RTC_WEEK_DAY_MON, RTC_WEEK_DAY_TUE, RTC_WEEK_DAY_WED, RTC_WEEK_DAY_THU, RTC_WEEK_DAY_FRI}
};

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/
static uint8_t bcd2dec( uint8_t bcd );
static int8_t checkBCD( uint8_t bcd );

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Set the date and clock.
 *
 * @param[in]   *prm    Pointer to configuration parameter.
 *
 * @retval      RTC_R_OK(=0) : Setting success.
 * @retval      RTC_R_ERR_SEC(=-1) : Parameter sec is outside the range.
 * @retval      RTC_R_ERR_MIN(=-2) : Parameter min is outside the range.
 * @retval      RTC_R_ERR_HOUR(=-3) : Parameter hour is outside the range.
 * @retval      RTC_R_ERR_WEEK(=-4) : Parameter week is outside the range.
 * @retval      RTC_R_ERR_DAY(=-5) : Parameter day is outside the range.
 * @retval      RTC_R_ERR_MON(=-6) : Parameter mon is outside the range.
 * @retval      RTC_R_ERR_YEAR(=-7) : Parameter year is outside the range.
 *
 * @note        If SYSCLK is HSCLK, then stop RTC before this function is called.
 */
int32_t rtc0_setTime( tRtcTime *prm )
{
	uint8_t	decYear;
	uint8_t	maxDay;
	uint8_t	regBak;
	
	/*=== parameter check. ===*/
	/*--- check [year](00-99). ---*/
	if( ( prm->year  > (uint8_t)0x99 ) || ( checkBCD(prm->year) < 0 ) ) {
		return ( RTC_R_ERR_YEAR );
	}
	
	/*--- check [mon](01-12). ---*/
	if( ( prm->mon < (uint8_t)0x1 )  ||
	    ( prm->mon > (uint8_t)0x12 ) ||
	    ( checkBCD(prm->mon) < 0 ) ) {
		return ( RTC_R_ERR_MON );
	}
	
	/*--- check [day]. ---*/
	/*- month : 2 => day : 28 or 29 -*/
	if( prm->mon == (uint8_t)0x2 ) {
		decYear = bcd2dec( prm->year );
		if( ( decYear & (uint8_t)0x03 ) == (uint8_t)0 ) {
			maxDay = (uint8_t)0x29;
		}
		else {
			maxDay = (uint8_t)0x28;
		}
	}
	/*- month : 4 or 6 or 9 or 11 => day : 30 -*/
	else if( ( prm->mon == (uint8_t)0x4 ) ||
	         ( prm->mon == (uint8_t)0x6 ) ||
	         ( prm->mon == (uint8_t)0x9 ) ||
	         ( prm->mon == (uint8_t)0x11 ) ) {
		maxDay = (uint8_t)0x30;
	}
	/*- month : 1 or 3 or 5 or 7 or 8 or 10 or 12 => day : 31 -*/
	else {
		maxDay = (uint8_t)0x31;
	}
	
	if( ( prm->day < (uint8_t)0x1 ) ||
	    ( prm->day > maxDay ) ||
	    ( checkBCD(prm->day) < 0 )) {
		return ( RTC_R_ERR_DAY );
	}
	
	/*--- check [week](01-07). ---*/
	if( ( prm->week < (uint8_t)0x1 ) ||
	    ( prm->week > (uint8_t)0x7 ) ) {
		return ( RTC_R_ERR_WEEK );
	}
	
	/*--- check [hour](00-23). ---*/
	if( ( prm->hour > (uint8_t)0x23 ) || ( checkBCD(prm->hour) < 0 ) ) {
		return ( RTC_R_ERR_HOUR );
	}
	
	/*--- check [min](00-59). ---*/
	if( ( prm->min > (uint8_t)0x59 ) || ( checkBCD(prm->min) < 0 ) ) {
		return ( RTC_R_ERR_MIN );
	}
	
	/*--- check [sec](00-59). ---*/
	if( ( prm->sec > (uint8_t)0x59 ) || ( checkBCD(prm->sec) < 0 ) ) {
		return ( RTC_R_ERR_SEC );
	}
	
	/*=== register setting. ===*/
	/*--- Stop RTC.  ---*/
	regBak = rtc0_checkRunning();
	rtc0_stop();
	/*--- Set time. ---*/
	write_reg32(RTC->RTCYEAR, prm->year);
	write_reg32(RTC->RTCMON, prm->mon);
	write_reg32(RTC->RTCDAY, prm->day);
	write_reg32(RTC->RTCWEEK, prm->week);
	write_reg32(RTC->RTCHOUR, prm->hour);
	write_reg32(RTC->RTCMIN, prm->min);
	write_reg32(RTC->RTCSEC, prm->sec);
	/*--- Start RTC.  ---*/
	if(regBak == 1) {
		rtc0_start();
	}
	return ( RTC_R_OK );
}

/**
 * Get the date and clock.
 *
 * @param[out]  *prm Pointer to area where setting is stored.
 *
 * @retval      RTC_R_OK(=0) : Success.
 * @retval      RTC_R_ERR_GETTIME(=-10) : Failure.
 */
int32_t rtc0_getTime( tRtcTime *prm )
{
	uint32_t hms1, hms2, ymdw1, ymdw2;
	
	hms1 = read_reg32(RTC->RTCHMS);
	hms2 = read_reg32(RTC->RTCHMS);
	ymdw1 = read_reg32(RTC->RTCYMDW);
	ymdw2 = read_reg32(RTC->RTCYMDW);
	
	if( (hms1 != hms2) || (ymdw1 != ymdw2) ) {
		return ( RTC_R_ERR_GETTIME );
	}

	prm->sec  = (uint8_t)(hms1 & 0x000000FF);
	prm->min  = (uint8_t)((hms1 >> 8) & 0x000000FF);
	prm->hour = (uint8_t)((hms1 >> 16 ) & 0x000000FF);
	prm->week = (uint8_t)(ymdw1 & 0x000000FF);
	prm->day  = (uint8_t)((ymdw1 >> 8) & 0x000000FF);
	prm->mon  = (uint8_t)((ymdw1 >> 16) & 0x000000FF);
	prm->year = (uint8_t)((ymdw1 >> 24) & 0x000000FF);

	return ( RTC_R_OK );
}

/**
 * Setting the regular interrupt.
 *
 * @param[in]   rin   regular interrupt setting parameter.
 * @arg             RTC_RIN_DISABLE(=0) : Disable to regular interrupt.
 * @arg             RTC_RIN_0_5_SEC(=1) : Enable to 0.5 sec interrupt.
 * @arg             RTC_RIN_1_0_SEC(=2) : Enable to 1.0 sec interrupt.
 * @arg             RTC_RIN_1_0_MIN(=3) : Enable to 1.0 min interrupt.
 *
 * @retval      RTC_R_OK(=0) : Success.
 * @retval      RTC_R_ERR_RIN(=-9) : Parameter rin is outside the range.
 */
int32_t rtc0_setRegularInt( uint8_t rin )
{
	uint32_t	reg;
	
	/*=== parameter check. ===*/
	/*--- check [rin] .---*/
	if( rin > (uint8_t)RTC_RIN_1_0_MIN ) {
		return ( RTC_R_ERR_RIN);
	}
	
	/*=== register setting. ===*/
	reg  = read_reg32(RTC->RTCCON);
	reg &= ~(uint32_t)RTC0CON_RIN;
	reg |= (uint32_t)rin << 1 ;
	write_reg32(RTC->RTCCON, reg);
	return ( RTC_R_OK );
}

/**
 * Setting the alarm0.
 *
 * @param[in]   alen    alarm setting parameter.
 * @arg            RTC_ALEN_DIS(=0) : Enable alarm 0.
 * @arg            RTC_ALEN_ENA(=1) : Disable ararm 0.
 *
 * @param[in]   *prm    Pointer to configuration parameter.                                         <br>
 *                      [*]prm->day = 0 : A day of the day is not used for the comparison.          <br>
 *                      [*]prm->mon = 0 : A day of the month is not used for the comparison.        <br>
 *
 * @retval      RTC_R_OK(=0) : Success.
 * @retval      RTC_R_ERR_MIN(=-2) : Parameter min is outside the range.
 * @retval      RTC_R_ERR_HOUR(=-3) : Parameter hour is outside the range.
 * @retval      RTC_R_ERR_WEEK(=-4) : Parameter week is outside the range.
 * @retval      RTC_R_ERR_ALEN(=-8) : Parameter alen is outside the range.
 */
int32_t rtc0_setAlarm0( uint8_t alen, tRtcAlarm0* prm )
{
	/*=== parameter check. ===*/
	/*--- check [alen]. ---*/
	if( alen > (uint8_t)RTC_ALEN_ENA ) {
		return ( RTC_R_ERR_ALEN );
	}
	if( alen == (uint8_t)RTC_ALEN_ENA ) {
		/*--- check [week](01-07). ---*/
		if( prm->week > (uint8_t)0x7 ) {
			return ( RTC_R_ERR_WEEK );
		}
		/*--- check [hour](00-23). ---*/
		if( ( prm->hour > (uint8_t)0x23 ) || ( checkBCD(prm->hour) < 0 ) ) {
			return ( RTC_R_ERR_HOUR );
		}
		/*--- check [min](00-59). ---*/
		if( ( prm->min > (uint8_t)0x59 ) || ( checkBCD(prm->min) < 0 ) ) {
			return ( RTC_R_ERR_MIN );
		}
	}
	/*=== register setting. ===*/
	/*--- Disable alarm0. ---*/
	write_reg32(RTC->AL0MIN, RTC_MIN_INVALID);
	write_reg32(RTC->AL0HOUR, RTC_HOUR_INVALID);
	write_reg32(RTC->AL0WEEK, RTC_WEEK_INVALID);
	/*--- Enable alarm0. ---*/
	if( alen == (uint8_t)RTC_ALEN_ENA ) {
		write_reg32(RTC->AL0MIN, prm->min);
		write_reg32(RTC->AL0HOUR, prm->hour);
		write_reg32(RTC->AL0WEEK, prm->week);
	}
	return ( RTC_R_OK );
}

/**
 * Setting the alarm1.
 *
 * @param[in]   alen    alarm setting parameter.
 * @arg            RTC_ALEN_DIS(=0) : Enable alarm 0.
 * @arg            RTC_ALEN_ENA(=1) : Disable ararm 0.
 *
 * @param[in]   *prm    Pointer to configuration parameter.                                         <br>
 *                      [*]prm->day = 0 : A day of the day is not used for the comparison.          <br>
 *                      [*]prm->mon = 0 : A day of the month is not used for the comparison.        <br>
 *
 * @retval      RTC_R_OK(=0) : Success.
 * @retval      RTC_R_ERR_MIN(=-2) : Parameter min is outside the range.
 * @retval      RTC_R_ERR_HOUR(=-3) : Parameter hour is outside the range.
 * @retval      RTC_R_ERR_DAY(=-5) : Parameter day is outside the range.
 * @retval      RTC_R_ERR_MON(=-6) : Parameter mon is outside the range.
 * @retval      RTC_R_ERR_ALEN(=-8) : Parameter alen is outside the range.
 */
int32_t rtc0_setAlarm1( uint8_t alen, tRtcAlarm1* prm )
{
	uint8_t	maxDay;
	
	/*=== parameter check. ===*/
	/*--- check [alen]. ---*/
	if( alen > (uint8_t)RTC_ALEN_ENA ) {
		return ( RTC_R_ERR_ALEN );
	}
	if( alen == (uint8_t)RTC_ALEN_ENA ) {
		if( ( prm->mon > (uint8_t)0x12 ) || ( checkBCD(prm->mon) < 0 ) ) {
			return ( RTC_R_ERR_MON );
		}
		if( prm->mon == (uint8_t)0x2 ) {
			maxDay = (uint8_t)0x29;
		}
		/*- month : 4 or 6 or 9 or 11 => day : 30 -*/
		else if( ( prm->mon == (uint8_t)0x4 ) ||
		         ( prm->mon == (uint8_t)0x6 ) ||
		         ( prm->mon == (uint8_t)0x9 ) ||
		         ( prm->mon == (uint8_t)0x11 ) ) {
			maxDay = (uint8_t)0x30;
		}
		/*- month : 1 or 3 or 5 or 7 or 8 or 10 or 12 => day : 31 -*/
		else {
			maxDay = (uint8_t)0x31;
		}
		if( ( prm->day > maxDay ) || ( checkBCD(prm->day) < 0 ) ) {
			return ( RTC_R_ERR_DAY );
		}
		/*--- check [hour](00-23). ---*/
		if( ( prm->hour > (uint8_t)0x23 ) || ( checkBCD(prm->hour) < 0 ) ) {
			return ( RTC_R_ERR_HOUR );
		}
		/*--- check [min](00-59). ---*/
		if( ( prm->min > (uint8_t)0x59 ) || ( checkBCD(prm->min) < 0 ) ) {
			return ( RTC_R_ERR_MIN );
		}
	}
	
	/*=== register setting. ===*/
	/*--- Disable alarm1. ---*/
	write_reg32(RTC->AL1MIN, (uint8_t)RTC_MIN_INVALID);
	write_reg32(RTC->AL1HOUR, (uint8_t)RTC_HOUR_INVALID);
	write_reg32(RTC->AL1DAY, (uint8_t)RTC_DAY_INVALID);
	write_reg32(RTC->AL1MON, (uint8_t)RTC_MON_INVALID);
	/*--- Enable alarm1. ---*/
	if( alen == (uint8_t)RTC_ALEN_ENA ) {
		write_reg32(RTC->AL1MON, prm->mon);
		write_reg32(RTC->AL1DAY, prm->day);
		write_reg32(RTC->AL1HOUR, prm->hour);
		write_reg32(RTC->AL1MIN, prm->min);
	}
	
	return ( RTC_R_OK );
}

/**
 * Get the alarm0 setting.
 *
 * @param[out]  prm : Pointer to area where setting is stored.
 *
 * @retval      None
 */
void rtc0_getAlarm0( tRtcAlarm0 *prm )
{
	/*=== register read. ===*/
	prm->min  = (uint8_t)read_reg32(RTC->AL0MIN);
	prm->hour = (uint8_t)read_reg32(RTC->AL0HOUR);
	prm->week  = (uint8_t)read_reg32(RTC->AL0WEEK);
}

/**
 * Get the alarm1 setting.
 *
 * @param[out]  prm : Pointer to area where setting is stored.
 *
 * @retval      None
 */
void rtc0_getAlarm1( tRtcAlarm1 *prm )
{
	/*=== register read. ===*/
	prm->min  = (uint8_t)read_reg32(RTC->AL1MIN);
	prm->hour = (uint8_t)read_reg32(RTC->AL1HOUR);
	prm->day  = (uint8_t)read_reg32(RTC->AL1DAY);
	prm->mon  = (uint8_t)read_reg32(RTC->AL1MON);
}

/**
 * Calculating the week.
 *
 * @param[in,out]   *prm : Pointer to area where setting is stored. <br>
 *                  prm->week : the calculation result of weekday.
 *
 * @retval      RTC_R_OK(=0) : Success.
 * @retval      RTC_R_ERR_DAY(=-5) : Parameter day is outside the range.
 * @retval      RTC_R_ERR_MON(=-6) : Parameter mon is outside the range.
 * @retval      RTC_R_ERR_YEAR(=-7) : Parameter year is outside the range.
 *
 * @note The calcWeekday function that calculates day of week works correctly when year is between 2000 and 2099.
 */
int32_t rtc0_calcWeekday( tRtcTime *prm )
{
	uint8_t	dayOfWeek;
	uint8_t	leapyear=0;
	uint8_t	maxDay;
	uint8_t	decDay, decMonth, decYear;

	/*=== parameter check. ===*/
	/*--- check [year](00-99). ---*/
	if( ( prm->year  > (uint8_t)0x99 ) || ( checkBCD(prm->year) < 0 ) ) {
		return ( RTC_R_ERR_YEAR );
	}
	/*--- check [mon](01-12). ---*/
	if( ( prm->mon < (uint8_t)0x1 )  ||
	    ( prm->mon > (uint8_t)0x12 ) ||
	    ( checkBCD(prm->mon) < 0 ) ) {
		return ( RTC_R_ERR_MON );
	}

	/* check leapyear. */
	decYear = bcd2dec( prm->year );
	if( ( decYear & (uint8_t)0x03 ) == (uint8_t)0 ) {
		leapyear = 1;							/* Setting leapyear flag .*/
	}

	/*--- check [day]. ---*/
	/*- month : 2 => day : 28 or 29 -*/
	if( prm->mon == (uint8_t)0x2 ) {
		/* If the number of year is leap year, then max number of day is 29. */
		if( leapyear != (uint8_t)0 ) {
			maxDay = (uint8_t)0x29;
		}
		else {
			maxDay = (uint8_t)0x28;
		}
	}
	/*- month : 4 or 6 or 9 or 11 => day : 30 -*/
	else if( ( prm->mon == (uint8_t)0x4 ) ||
	         ( prm->mon == (uint8_t)0x6 ) ||
	         ( prm->mon == (uint8_t)0x9 ) ||
	         ( prm->mon == (uint8_t)0x11 ) ) {
		maxDay = (uint8_t)0x30;
	}
	/*- month : 1 or 3 or 5 or 7 or 8 or 10 or 12 => day : 31 -*/
	else {
		maxDay = (uint8_t)0x31;
	}
	if( ( prm->day < (uint8_t)0x1 ) ||
	    ( prm->day > maxDay ) ||
	    ( checkBCD(prm->day) < 0 ) ) {
		return ( RTC_R_ERR_DAY );
	}

	/*=== Calculation. ===*/
	/* Convert bcd to dec.*/
	decDay    = bcd2dec( prm->day );
	decMonth  = bcd2dec( prm->mon );

	/* Calculation day of week (0:Sunday,1:Monday, ..., 6:Saturday). */
	dayOfWeek = decYear + (decYear>>2) + tblOutOfWeekday[leapyear][decMonth-1] + decDay + 6;
	dayOfWeek %= 7;

	/* Convert week No. */
	prm->week = tblWeekdayNo[leapyear][dayOfWeek];

	return ( RTC_R_OK );
}

/*############################################################################*/
/*#                              Subroutine                                  #*/
/*############################################################################*/
/**
 * BCD to DEC
 *
 * @param[in]   bcd : bcd data
 *
 * @retval      dec
 * @retval      None
 */
static uint8_t bcd2dec( uint8_t bcd )
{
	return ( (uint8_t)( ( ( bcd >> 4 ) * 10 ) + ( bcd & 0x0F ) ) );
}

/**
 * Check BCD format
 *
 * @param[in]   bcd     Checking value
 * @retval      0       Valid
 * @retval      -1      Invalid
 */
static int8_t checkBCD( uint8_t bcd )
{
	if (((bcd & 0xFu) >= 0xAu) || ((bcd & 0xF0u) >= 0xA0u)) {
		return (-1);
	}
	return ( 0 );
}

/** @} */
/** @} */
