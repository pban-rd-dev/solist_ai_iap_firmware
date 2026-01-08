/*****************************************************************************
 wdt.c

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
 * @file    wdt.c
 *
 * This module is 'WDT' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup WDT
 *  @{
 *
 *  @brief Watchdog Timer register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "wdt.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*--- bit field of registers ---*/
/* WDTCON */
#define WDTCON_WDP          ( 0x01 )        /**< WDTCON Register WDP bit      */

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
 * Clear WDT
 *
 * @param       -
 * @return      None
 */
void wdt_clear( void )
{
	do {
		write_reg32( WDT->WDTCON, 0x5A );
	} while ( (read_reg32( WDT->WDTCON ) & WDTCON_WDP) == 0 );
	write_reg32( WDT->WDTCON, 0xA5 );
}

/** @} */
/** @} */
