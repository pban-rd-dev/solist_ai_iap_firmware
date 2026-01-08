/*****************************************************************************
 wdt.h

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
 * @file    wdt.h
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

#ifndef WDT_H__
#define WDT_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*--- bit field of registers ---*/
/* WDTMOD */
#define WDTMOD_WDTP0        ( 0x01 )        /**< WDTMOD Register WDTP[0] bit     */
#define WDTMOD_WDTP1        ( 0x02 )        /**< WDTMOD Register WDTP[1] bit     */

#define WDT_125MS           ( 0x00 )        /**< Overflow period : 125 ms        */
#define WDT_500MS           ( WDTMOD_WDTP0 )/**< Overflow period : 500 ms        */
#define WDT_2S              ( WDTMOD_WDTP1 )/**< Overflow period : 2 sec         */
#define WDT_8S              ( WDTMOD_WDTP1 | WDTMOD_WDTP0 ) /**< Overflow period : 8 sec         */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize WDT
 *
 * @param[in]   wdtmod  Overflow period (WDT_125MS,WDT_500MS,WDT_2S,WDT_8S)
 * @return      None
 */
#define         wdt_init( wdtmod )          write_reg32( WDT->WDTMOD, wdtmod )

void            wdt_clear( void );

#endif /*WDT_H__*/

/** @} */
/** @} */
