/*****************************************************************************
 timer_common.h

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
 * @file    timer_common.h
 *
 * This module is 'Timer' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Timer
 *  @{
 *
 *  @brief Timers register related
 *
 *  @file
 */

#ifndef TIMER_COMMON_H__
#define TIMER_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* TMnCON */
#define TMnCON_TnCS0        ( 0x0001 )       /**< TMnCON Register TnCS0 bit  */
#define TMnCON_TnCS1        ( 0x0002 )       /**< TMnCON Register TnCS1 bit  */
#define TMnCON_TnDIV0       ( 0x0010 )       /**< TMnCON Register TnDIV0 bit */
#define TMnCON_TnDIV1       ( 0x0020 )       /**< TMnCON Register TnDIV1 bit */
#define TMnCON_TnDIV2       ( 0x0040 )       /**< TMnCON Register TnDIV2 bit */
#define TMnCON_TmM32        ( 0x1000 )       /**< TMnCON Register Tn1M32 bit */
#define TMnCON_TnOST        ( 0x0100 )       /**< TMnCON Register TnOST bit  */

/*=== API parameters value ===*/
/* Initial configuration parameters for timerX_init() */
#define TM_CS_LSCLK         (               0 |               0 )                      /**< Clock source   :LSCLK         */
#define TM_CS_OSCLK         (               0 |    TMnCON_TnCS0 )                      /**< Clock source   :OSCLK         */
#define TM_CS_EXTCLK        (    TMnCON_TnCS1 |    TMnCON_TnCS0 )                      /**< Clock source   :EXTCLK(TMCKI) */
#define TM_DIV1             (               0 |               0 |               0 )    /**< Division ratio :Not divide    */
#define TM_DIV2             (               0 |               0 |   TMnCON_TnDIV0 )    /**< Division ratio :1/2           */
#define TM_DIV4             (               0 |   TMnCON_TnDIV1 |               0 )    /**< Division ratio :1/4           */
#define TM_DIV8             (               0 |   TMnCON_TnDIV1 |   TMnCON_TnDIV0 )    /**< Division ratio :1/8           */
#define TM_DIV16            (   TMnCON_TnDIV2 |               0 |               0 )    /**< Division ratio :1/16          */
#define TM_DIV32            (   TMnCON_TnDIV2 |               0 |   TMnCON_TnDIV0 )    /**< Division ratio :1/32          */
#define TM_DIV64            (   TMnCON_TnDIV2 |   TMnCON_TnDIV1 |               0 )    /**< Division ratio :1/64          */
#define TM_MODE_16BIT       (               0 )                                        /**< Timer mode     :16-bit timer  */
#define TM_MODE_32BIT       (    TMnCON_TmM32 )                                        /**< Timer mode     :32-bit timer  */
#define TM_OST_RELOAD       (               0 )                                        /**< Mode           :auto reload   */
#define TM_OST_ONESHOT      (    TMnCON_TnOST )                                        /**< Mode           :one shot      */

#endif /*TIMER_COMMON_H__*/

/** @} */
/** @} */
