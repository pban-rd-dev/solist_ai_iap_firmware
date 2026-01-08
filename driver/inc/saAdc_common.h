/*****************************************************************************
 saAdc_common.h

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
 * @file    saAdc_common.h
 *
 * This module is 'SA-ADC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup SA-ADC
 *  @{
 *
 *  @brief Successive Approximation Type A/D Converter register related
 *
 *  @file
 */

#ifndef SAADC_COMMON_H__
#define SAADC_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*=== API Parameters value. ===*/
/* Parameters for saAdc_init() */
#define SAADC_SAINIT_NO_DISCHARGE   ( 0x0U )    /**< Discharge control  : no discharge           */
#define SAADC_SAINIT_DISCHARGE      ( 0x1U )    /**< Discharge control  : discharge              */

#define SAADC_SACK_OSCLK_DIV1       ( 0x0U )    /**< SA-ADC clock       : OSCLK                  */
#define SAADC_SACK_OSCLK_DIV2       ( 0x1U )    /**< SA-ADC clock       : OSCLK/2                */
#define SAADC_SACK_OSCLK_DIV4       ( 0x2U )    /**< SA-ADC clock       : OSCLK/4                */
#define SAADC_SACK_OSCLK_DIV8       ( 0x3U )    /**< SA-ADC clock       : OSCLK/8                */
#define SAADC_SACK_OSCLK_DIV16      ( 0x4U )    /**< SA-ADC clock       : OSCLK/16               */
#define SAADC_SACK_OSCLK_DIV3       ( 0x5U )    /**< SA-ADC clock       : OSCLK/3                */
#define SAADC_SACK_OSCLK_DIV6       ( 0x6U )    /**< SA-ADC clock       : OSCLK/6                */
#define SAADC_SACK_LSCLK_DIV1       ( 0x7U )    /**< SA-ADC clock       : LSCLK                  */

#define SAADC_SALP_ONESHOT          ( 0x0U )    /**< SA-ADC mode        : one-shot               */
#define SAADC_SALP_CONTINUOUS       ( 0x1U )    /**< SA-ADC mode        : continuous             */

#define SAADC_SYNC_NORMAL           ( 0x00U)    /**< Channel synchronization       : normal      */
#define SAADC_SYNC_SYNCHRONOUS      ( 0x01U)    /**< Channel synchronization       : synchronous */

#define SAADC_SABC_VDD_OVER_2_7     ( 0x00U)    /**< A/D Characteristic Adjustment : Vdd >= 2.7V */
#define SAADC_SABC_VDD_UNDER_2_7    ( 0x04U)    /**< A/D Characteristic Adjustment : Vdd < 2.7V  */

#define SAADC_VREFP_VDD             ( 0x0U )    /**< SA-ADC Vref       : VDD                    */
#define SAADC_VREFP_VREFP           ( 0x1U )    /**< SA-ADC Vref       : VREFP                  */

#define SAADC_SADIMD1_LIMIT_MATCH      ( 0x0U ) /**< SA-ADC interrupt mode of limit  : match      */
#define SAADC_SADIMD1_LIMIT_REGARDLESS ( 0x1U ) /**< SA-ADC interrupt mode of limit  : regardless */

#define SAADC_SADIMD0_ALL_CH        ( 0x0U )    /**< SA-ADC interrupt mode   : all channel      */
#define SAADC_SADIMD0_EACH_CH       ( 0x1U )    /**< SA-ADC interrupt mode   : each channel     */

#define SAADC_SALMD_INSIDE_LIMIT    ( 0x0U )    /**< SA-ADC limit mode : interrupt at inside limit    */
#define SAADC_SALMD_UPPER_LIMIT     ( 0x1U )    /**< SA-ADC limit mode : interrupt at upper limit     */
#define SAADC_SALMD_LOWER_LIMIT     ( 0x2U )    /**< SA-ADC limit mode : interrupt at lower limit     */
#define SAADC_SALMD_OUTSIDE_LIMIT   ( 0x3U )    /**< SA-ADC limit mode : interrupt at outside limit   */

#define SAADC_SALEN_DISABLE         ( 0x0U )    /**< SA-ADC limit control : disable             */
#define SAADC_SALEN_ENABLE          ( 0x1U )    /**< SA-ADC limit control : enable              */


/* Parameters for saAdc_setTrigger() */
#define SAADC_SASTS_TM0INT          ( 0x0U )    /**< SA-ADC trigger source     : TM0INT           */
#define SAADC_SASTS_TM1INT          ( 0x1U )    /**< SA-ADC trigger source     : TM1INT           */
#define SAADC_SASTS_TM2INT          ( 0x2U )    /**< SA-ADC trigger source     : TM2INT           */
#define SAADC_SASTS_TM3INT          ( 0x3U )    /**< SA-ADC trigger source     : TM3INT           */
#define SAADC_SASTS_FTM0TGO         ( 0x8U )    /**< SA-ADC trigger source     : FTM0TGO          */
#define SAADC_SASTS_FTM1TGO         ( 0x9U )    /**< SA-ADC trigger source     : FTM1TGO          */
#define SAADC_SASTS_NTMS0TGO0       ( 0x10U )   /**< SA-ADC trigger source     : NTMS0TGO0        */
#define SAADC_SASTS_NTMS0TGO1       ( 0x11U )   /**< SA-ADC trigger source     : NTMS0TGO1        */
#define SAADC_SASTS_NTMS1TGO0       ( 0x12U )   /**< SA-ADC trigger source     : NTMS1TGO0        */
#define SAADC_SASTS_NTMS1TGO1       ( 0x13U )   /**< SA-ADC trigger source     : NTMS1TGO1        */
#define SAADC_SASTS_NTMS2TGO0       ( 0x14U )   /**< SA-ADC trigger source     : NTMS2TGO0        */
#define SAADC_SASTS_NTMS2TGO1       ( 0x15U )   /**< SA-ADC trigger source     : NTMS2TGO1        */


/* Parameters for saAdc_setEnableChannel() */
#define SAADC_OFF                   ( 0U )      /**< SA-ADC control          : off              */
#define SAADC_RUN                   ( 1U )      /**< SA-ADC control          : run              */

/* Parameters for saAdc_setTestmode() */
#define SAADC_SADTMOD_DISABLE       ( 0U )      /**< SA-ADC test mode      : disable            */
#define SAADC_SADTMOD_FULLSCALE     ( 1U )      /**< SA-ADC test mode      : full scale         */
#define SAADC_SADTMOD_ZEROSCALE     ( 2U )      /**< SA-ADC test mode      : zero scale         */
#define SAADC_SADTMOD_VREFSCALE     ( 3U )      /**< SA-ADC test mode      : vref scale         */

/* Parameters for saAdc_clrLimitStatus() */
#define SAADC_SAULC_ALL_CLR         ( 0xFFFUL )  /**< SA-ADC limit judgment result: all clear  */

/**
 * @brief Initial parameters for SA-ADC
 *
 */
typedef struct {
	uint8_t discharge;              /**< discharge control             */
	uint8_t holdTime;               /**< sample hold time              */
	uint8_t clock;                  /**< clock                         */
	uint8_t mode;                   /**< mode                          */
	uint8_t limitInterrupt;         /**< limit interrupt               */
	uint8_t limitMode;              /**< limit mode                    */
	uint8_t reserved;               /**< reserved                      */
	uint8_t ampStabilityTime;       /**< amp stability time            */
	uint8_t interruptMode;          /**< interrupt mode                */
	uint8_t interruptLimitMode;     /**< interrupt mode of limit       */
	uint16_t interval;              /**< interval count                */
	uint8_t channelSync;            /**< channel synchronization       */
	uint8_t adAdjustment;           /**< A/D Characteristic Adjustment */
} initAdc_t;

/**
 * @brief A/D conversion permission configuration
 *
 */
typedef struct {
	uint8_t ch0;                    /**< SA-ADC enable : ch0  */
	uint8_t ch1;                    /**< SA-ADC enable : ch1  */
	uint8_t ch2;                    /**< SA-ADC enable : ch2  */
	uint8_t ch3;                    /**< SA-ADC enable : ch3  */
	uint8_t ch4;                    /**< SA-ADC enable : ch4  */
	uint8_t ch5;                    /**< SA-ADC enable : ch5  */
	uint8_t ch6;                    /**< SA-ADC enable : ch6  */
	uint8_t ch7;                    /**< SA-ADC enable : ch7  */
	uint8_t ch8;                    /**< SA-ADC enable : ch8  */
	uint8_t ch9;                    /**< SA-ADC enable : ch9  */
	uint8_t ch10;                   /**< SA-ADC enable : ch10 */
	uint8_t ch11;                   /**< SA-ADC enable : ch11 */
} enableAdcChannel_t;

#endif /*SAADC_COMMON_H__*/

/** @} */
/** @} */
