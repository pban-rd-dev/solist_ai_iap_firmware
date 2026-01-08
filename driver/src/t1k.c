/*****************************************************************************
 t1k.c

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
 * @file    t1k.c
 *
 * This module is '1kHz Timer' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Timer1kHz
 *  @{
 *
 *  @brief 1kHz Timer register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "t1k.h"

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
 * T1KCR Register Clear
 *
 * @param       -
 * @return      None
 */
void t1k_clrT1KCR( void )
{
	/*---	1kHz timer Stop	---*/
	t1k_stop();

	/*---	T1KCR Register Clear	---*/
	write_reg32(TIMER1K->T1KCR, 0x00000000UL);
}

/** @} */
/** @} */
