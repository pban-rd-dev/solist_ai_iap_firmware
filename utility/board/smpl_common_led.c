/*****************************************************************************
 smpl_common_led.c

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
 * @file    smpl_common_led.c
 *
 * This module is commmon process for using LED.
 *
 */
#include "mcu.h"
#include "smpl_common_led.h"

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
/*#                              Subroutine                                  #*/
/*############################################################################*/
/**
 * LED1 initialize
 *
 * @param[in]   defVal      The dafault logical value of LED
 * @arg         value : LED_INACTIVE or LED_ACTIVE
 * @return      None
 */
void smpl_initLED1( uint8_t defVal )
{
    if(defVal == LED_ACTIVE) {
        smpl_onLED1();
    }
    else {
        smpl_offLED1();
    }
    write_bit( PORT5->P5MOD0, (0x3FU << 0U), (0x0AU << 0U));    /* General-purpose mode */
}

/**
 * LED2 initialize
 *
 * @param[in]   defVal      The dafault logical value of LED
 * @arg         value : LED_INACTIVE or LED_ACTIVE
 * @return      None
 */
void smpl_initLED2( uint8_t defVal )
{
    if(defVal == LED_ACTIVE) {
        smpl_onLED2();
    }
    else {
        smpl_offLED2();
    }
    write_bit( PORT5->P5MOD0, (0x3FU << 8U), (0x0AU << 8U));    /* General-purpose mode */
}

/**
 * LED3 initialize
 *
 * @param[in]   defVal      The dafault logical value of LED
 * @arg         value : LED_INACTIVE or LED_ACTIVE
 * @return      None
 */
void smpl_initLED3( uint8_t defVal )
{
    if(defVal == LED_ACTIVE) {
        smpl_onLED3();
    }
    else {
        smpl_offLED3();
    }
    write_bit( PORT5->P5MOD0, (0x3FU << 16U), (0x0AU << 16U));    /* General-purpose mode */
}
