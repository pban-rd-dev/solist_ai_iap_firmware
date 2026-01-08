/*****************************************************************************
 timer0_1.c

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
 * @file    timer0_1.c
 *
 * This module is 'Timer0_1' drivers.
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
#include "timer0_1.h"
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
 * Initialize the value to be compared with the timer0 counter and an 16-bit/32-bit binary counter
 * @note Set TMnD when the timer stops.
 *
 * @param[in]   cnt     the value of TM0D register
 * @return      None
 */
void timer0_setCnt( uint32_t cnt )
{
	write_reg32( TIMER->TM0C, 0x00 );                /* counter clear */
	write_reg32( TIMER->TM0D, cnt );                 /* compare value set */
}

/**
 * Initialize the value to be compared with the timer1 counter and an 16-bit binary counter
 * @note Set TMnD when the timer stops.
 *
 * @param[in]   cnt     the value of TM1D register
 * @return      None
 */
void timer1_setCnt( uint32_t cnt )
{
	/*--- Only 16bit timer mode ---*/
	write_reg32( TIMER->TM1C, 0x00 );                /* counter clear */
	write_reg32( TIMER->TM1D, cnt );                 /* compare value set */
}

/** @} */
/** @} */
