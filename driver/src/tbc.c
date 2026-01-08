/*****************************************************************************
 tbc.c

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
 * @file    tbc.c
 *
 * This module is 'TBC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup TBC
 *  @{
 *
 *  @brief Time Base Counter register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "tbc.h"
#include "irq.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/
static void s_setLTInS( uint8_t bitNo, uint8_t ltins );

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Set low-speed time base clock (LTBINTn)
 *
 * @param[in]   bitNo   bit-number of LTInS
 * @param[in]   ltins   assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE )
 * @return      None
 */
static void s_setLTInS( uint8_t bitNo, uint8_t ltins )
{
	uint32_t buf;
	uint32_t reg;

	buf = NVIC_GetEnableIRQ( LTBC_IRQn );
	irq_tbc0_dis();

	reg = read_reg32( LTBC->LTBINT );
	reg &= ~((uint32_t)0xFU << bitNo);
	reg |= ((uint32_t)ltins) << bitNo;
	write_reg32( LTBC->LTBINT, reg );
	__NOP();
	
	tbc_clearIntStat(TBC_INTST_ALL_CLEAR);
	irq_tbc0_clearIRQ();
	
	if ( buf == 1U ) {
		irq_tbc0_ena();
	}
}

/**
 * Set low-speed time base clock (LTBINT0)
 *
 * @param[in]   lti0s   assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @return      None
 */
void tbc_setLTI0S( uint8_t lti0s )
{
	s_setLTInS( 0U, lti0s );
}

/**
 * Set low-speed time base clock (LTBINT1)
 *
 * @param[in]   lti1s   assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @return      None
 */
void tbc_setLTI1S( uint8_t lti1s )
{
	s_setLTInS( 4U, lti1s );
}

/**
 * Set low-speed time base clock (LTBINT2)
 *
 * @param[in]   lti2s   assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @return      None
 */
void tbc_setLTI2S( uint8_t lti2s )
{
	s_setLTInS( 8U, lti2s );
}

/**
 * Set low-speed time base clock (LTBINT0-2) and output signal
 *
 * @param[in]   lti0s   LTBINT0 assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @param[in]   lti1s   LTBINT1 assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @param[in]   lti2s   LTBINT2 assigned clock (TBC_LTINS_T128HZ~TBC_LTINS_T1HZ,TBC_LTINS_NONE)
 * @param[in]   ltco    output signal (TBC_LTCO_T1HZ,TBC_LTCO_T2HZ )
 * @return      None
 */
void tbc_setLTBINT( uint8_t lti0s, uint8_t lti1s, uint8_t lti2s, uint8_t ltco )
{
	uint32_t buf;
	uint32_t reg;

	buf = NVIC_GetEnableIRQ( LTBC_IRQn );
	irq_tbc0_dis();

	reg = read_reg32( LTBC->LTBINT );
	reg &= ~0x00010FFFUL;
	reg |= ((uint32_t)lti2s << 8U) | ((uint32_t)lti1s << 4U) | ((uint32_t)lti0s << 0U);
	reg |= (uint32_t)ltco << 16U;
	write_reg32( LTBC->LTBINT, reg );
	__NOP();
	
	tbc_clearIntStat(TBC_INTST_ALL_CLEAR);
	irq_tbc0_clearIRQ();
	
	if ( buf == 1U ) {
		irq_tbc0_ena();
	}
}

/**
 * Clear low-speed time base counter register (LTBR)
 *
 * @param       -
 * @return      None
 * 
 * @note        Call this function with SYSCLK set to LSCLK.
 */
void tbc_clrLTBR( void )
{
	uint32_t buf;

	buf = NVIC_GetEnableIRQ( LTBC_IRQn );
	irq_tbc0_dis();

	write_reg32( LTBC->LTBR, 0x00000000UL );
	__NOP();
	
	tbc_clearIntStat(TBC_INTST_ALL_CLEAR);
	irq_tbc0_clearIRQ();
	
	if ( buf == 1U ) {
		irq_tbc0_ena();
	}
}

/**
 * Set output signal of low-speed time base clock
 *
 * @param[in]   select  output signal (TBC_LTCO_T1HZ,TBC_LTCO_T2HZ )
 * @return      None
 */
void tbc_setOutclk( uint8_t select )
{
	uint32_t reg;

	reg = read_reg32( LTBC->LTBINT );
	reg &= ~0x00010000UL;
	reg |= (uint32_t)select << 16U;
	write_reg32( LTBC->LTBINT, reg );
}

/** @} */
/** @} */
