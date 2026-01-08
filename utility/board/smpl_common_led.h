/*****************************************************************************
 smpl_common_led.h

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
 * @file    smpl_common_led.h
 *
 * This file is common definition for using LED.
 *
 */
#ifndef SMPL_COMMON_LED_H__
#define SMPL_COMMON_LED_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== LED ===*/
#define         LED1_PORT                   ( PORT5->P5DO )                         /**< The register of Port data for LED         */
#define         LED2_PORT                   ( PORT5->P5DO )                         /**< The register of Port data for LED         */
#define         LED3_PORT                   ( PORT5->P5DO )                         /**< The register of Port data for LED         */
#define         LED_ACTIVE                  ( 1U )                                  /**< The logical value of LED activate         */
#define         LED_INACTIVE                ( 0U )                                  /**< The logical value of LED inactivate       */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * LED1 ON
 *
 * @param       -
 * @return      None
 */
#define         smpl_onLED1()               ( clear_bit( LED1_PORT, (LED_ACTIVE << 0U) ) )

/**
 * LED1 OFF
 *
 * @param       -
 * @return      None
 */
#define         smpl_offLED1()              ( set_bit( LED1_PORT, (LED_ACTIVE << 0U) ) )

/**
 * LED1 blink
 *
 * @param       -
 * @return      None
 */
#define         smpl_blinkLED1()            do {                                                        \
                                                if( get_bit( LED1_PORT, (1U << 0U) ) != 0 ) {           \
                                                    smpl_onLED1();                                      \
                                                } else {                                                \
                                                    smpl_offLED1();                                     \
                                                }                                                       \
                                            } while(0)

/**
 * LED2 ON
 *
 * @param       -
 * @return      None
 */
#define         smpl_onLED2()               ( clear_bit( LED2_PORT, (LED_ACTIVE << 1U) ) )

/**
 * LED2 OFF
 *
 * @param       -
 * @return      None
 */
#define         smpl_offLED2()              ( set_bit( LED2_PORT, (LED_ACTIVE << 1U) ) )

/**
 * LED2 blink
 *
 * @param       -
 * @return      None
 */
#define         smpl_blinkLED2()            do {                                                        \
                                                if( get_bit( LED2_PORT, (1U << 1U) ) != 0 ) {           \
                                                    smpl_onLED2();                                      \
                                                } else {                                                \
                                                    smpl_offLED2();                                     \
                                                }                                                       \
                                            } while(0)

/**
 * LED3 ON
 *
 * @param       -
 * @return      None
 */
#define         smpl_onLED3()               ( clear_bit( LED3_PORT, (LED_ACTIVE << 2U) ) )

/**
 * LED3 OFF
 *
 * @param       -
 * @return      None
 */
#define         smpl_offLED3()              ( set_bit( LED3_PORT, (LED_ACTIVE << 2U) ) )

/**
 * LED3 blink
 *
 * @param       -
 * @return      None
 */
#define         smpl_blinkLED3()            do {                                                        \
                                                if( get_bit( LED3_PORT, (1U << 2U) ) != 0 ) {           \
                                                    smpl_onLED3();                                      \
                                                } else {                                                \
                                                    smpl_offLED3();                                     \
                                                }                                                       \
                                            } while(0)

/* Prototype */
void smpl_initLED1( uint8_t defVal );
void smpl_initLED2( uint8_t defVal );
void smpl_initLED3( uint8_t defVal );

#endif /* SMPL_COMMON_LED_H__ */

