/*****************************************************************************
 rst.c

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
 * @file    rst.c
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
 *  @brief Reset Function register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "rst.h"

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
 * Execute CPU reset
 *
 * @param       -
 * @return      None
 */
__attribute__((__noreturn__))
void rst_cpuReset( void )
{
	__DSB();
	
	/* CPU reset ON */
	write_reg32( RESET->CPURST, CPURST_ON );

	__DSB();

	/* wait until reset */
	for(;;) {
	  __NOP();
	}
}

/** @} */
/** @} */
