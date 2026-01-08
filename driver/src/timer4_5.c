/*****************************************************************************
 timer4_5.c

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
 * @file    timer4_5.c
 *
 * This module is 'Timer4_5' drivers.
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "timer4_5.h"
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
 * Initialize the value to be compared with the timer4 counter and an 16-bit/32-bit binary counter
 * @note Set TMnD when the timer stops.
 *
 * @param[in]   cnt     the value of TM4D register
 * @return      None
 */
void timer4_setCnt( uint32_t cnt )
{
	write_reg32( TIMER->TM4C, 0x00 );                /* counter clear */
	write_reg32( TIMER->TM4D, cnt );                 /* compare value set */
}

/**
 * Initialize the value to be compared with the timer5 counter and an 16-bit binary counter
 * @note Set TMnD when the timer stops.
 *
 * @param[in]   cnt     the value of TM5D register
 * @return      None
 */
void timer5_setCnt( uint32_t cnt )
{
	/*--- Only 16bit timer mode ---*/
	write_reg32( TIMER->TM5C, 0x00 );                /* counter clear */
	write_reg32( TIMER->TM5D, cnt );                 /* compare value set */
}

/** @} */
/** @} */
