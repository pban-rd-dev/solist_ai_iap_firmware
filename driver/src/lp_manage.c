/*****************************************************************************
 lp_manage.c

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
 * @file    lp_manage.c
 *
 * This module is 'System' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup System
 *  @{
 *
 *  @brief System Control Function register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "lp_manage.h"

/**
 * Setting STOP mode
 *
 * @param       -
 * @return      None
 */
void lp_setStopMode( void )
{
	/* The CPU mode is changed to the STOP mode. */
	/* When the mode switch to STOP mode at High speed oscillator is
	 * used, Frequency Status Register (FSTAT) HOSCS bit must be "0". */

	write_reg32( LSICNT->PMCON, 0 << 0);
	SCB->SCR |= 1UL << 2;
	__DSB();
	__WFI();
}

/**
 * Setting HALT mode
 *
 * @param       -
 * @return      None
 */
void lp_setHaltMode( void )
{
	/* The CPU mode is changed to the HALT mode. */
	write_reg32( LSICNT->SBYCON, 1 << 0);
	__WFI();
}

/** @} */
/** @} */
