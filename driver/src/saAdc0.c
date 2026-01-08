/*****************************************************************************
 saAdc0.c

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
 * @file    saAdc0.c
 *
 * This module is 'SA-ADC0' drivers.
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "saAdc0.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Initialize SA-ADC
 *
 * @param[in]   *st_initAdc     Configuration of SA-ADC operation
 * @arg                             discharge
 *                                    - SAADC_SAINIT_NO_DISCHARGE
 *                                    - SAADC_SAINIT_DISCHARGE
 * @arg                             holdTime
 *                                    - 0x00U-0x1FU
 * @arg                             clock
 *                                    - SAADC_SACK_OSCLK_DIV1
 *                                    - SAADC_SACK_OSCLK_DIV2
 *                                    - SAADC_SACK_OSCLK_DIV4
 *                                    - SAADC_SACK_OSCLK_DIV8
 *                                    - SAADC_SACK_OSCLK_DIV16
 *                                    - SAADC_SACK_OSCLK_DIV3
 *                                    - SAADC_SACK_OSCLK_DIV6
 *                                    - SAADC_SACK_LSCLK_DIV1
 * @arg                             mode
 *                                    - SAADC_SALP_ONESHOT
 *                                    - SAADC_SALP_CONTINUOUS  
 * @arg                             limitInterrupt
 *                                    - SAADC_SALMD_INSIDE_LIMIT
 *                                    - SAADC_SALMD_UPPER_LIMIT
 *                                    - SAADC_SALMD_LOWER_LIMIT
 *                                    - SAADC_SALMD_OUTSIDE_LIMIT
 * @arg                             limitMode
 *                                    - SAADC_SALEN_DISABLE
 *                                    - SAADC_SALEN_ENABLE
 * @arg                             ampStabilityTime
 *                                    - 0x00U-0x0FU
 * @arg                             interruptMode
 *                                    - SAADC_SADIMD0_ALL_CH
 *                                    - SAADC_SADIMD0_EACH_CH
 * @arg                             interruptLimitMode
 *                                    - SAADC_SADIMD1_LIMIT_MATCH
 *                                    - SAADC_SADIMD1_LIMIT_REGARDLESS
 * @arg                             interval
 *                                    - 0x0000U-0xFFFFU
 * @arg                             channelSync
 *                                    - SAADC_SYNC_NORMAL
 *                                    - SAADC_SYNC_SYNCHRONOUS
 *                                    .
 *                                    note: "channelSync" is used only for SA-ADC1.
 * @arg                             adAdjustment
 *                                    - SAADC_SABC_VDD_OVER_2_7
 *                                    - SAADC_SABC_VDD_UNDER_2_7
 * @return      None
 */
void saAdc0_init( initAdc_t *st_initAdc )
{
	uint32_t setVal;

	/* stop adc */
	write_reg32( SADC0->SADCON, 0UL );

	/* disable test mode */
	write_reg32( SADC0->SADTMOD, 0UL );

	/* clear status */
	saAdc0_clrAllLimitStatus();

	/* set mode */
	setVal = ((uint32_t)st_initAdc->ampStabilityTime << 24U);
	setVal |= ((uint32_t)st_initAdc->discharge << 1U);
	setVal |= ((uint32_t)st_initAdc->holdTime << 16U);
	setVal |= ((uint32_t)st_initAdc->adAdjustment << 12U);
	setVal |= ((uint32_t)st_initAdc->clock << 8U);
	setVal |= ((uint32_t)st_initAdc->mode);
	write_reg32( SADC0->SADMOD, setVal );

	/* interval count */
	write_reg32( SADC0->SADSTM, st_initAdc->interval );

	/* set limit control */
	setVal = ((uint32_t)st_initAdc->limitInterrupt << 8U) | ((uint32_t)st_initAdc->limitMode);
	write_reg32( SADC0->SADLMOD, setVal );

	/* set interrupt */
	setVal = ((uint32_t)st_initAdc->interruptLimitMode << 1U) | ((uint32_t)st_initAdc->interruptMode);
	write_reg32( SADC0->SADIMOD, setVal );
}

/**
 * Setting conversion channels
 *
 * @param[in]   *st_enableChannel   Configuration of the conversion channels <br>
 *                                  st_enableChannel.chN : SAADC_OFF or SAADC_RUN
 * @return      None
 */
void saAdc0_setEnableChannel( enableAdcChannel_t *st_enableChannel )
{
	uint32_t setVal;

	setVal = (uint32_t)(read_reg32( SADC0->SADEN ) & (0x1U << 17U));
	setVal |= (uint32_t)(st_enableChannel->ch0);
	setVal |= (uint32_t)(st_enableChannel->ch1 << 1U);
	setVal |= (uint32_t)(st_enableChannel->ch2 << 2U);
	setVal |= (uint32_t)(st_enableChannel->ch3 << 3U);
	setVal |= (uint32_t)(st_enableChannel->ch4 << 4U);
	setVal |= (uint32_t)(st_enableChannel->ch5 << 5U);
	setVal |= (uint32_t)(st_enableChannel->ch6 << 6U);
	setVal |= (uint32_t)(st_enableChannel->ch7 << 7U);
	setVal |= (uint32_t)(st_enableChannel->ch8 << 8U);
	setVal |= (uint32_t)(st_enableChannel->ch9 << 9U);
	setVal |= (uint32_t)(st_enableChannel->ch10 << 10U);
	setVal |= (uint32_t)(st_enableChannel->ch11 << 11U);
	
	write_reg32(SADC0->SADEN, setVal);
}

/** @} */
/** @} */
