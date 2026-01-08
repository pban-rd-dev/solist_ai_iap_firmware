/*****************************************************************************
 cmp0.h

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
 * @file    cmp0.h
 *
 * This module is 'CMP0' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup CMP
 *  @{
 *
 *  @brief Analog Comparator register related
 *
 *  @file
 */

#ifndef CMP0_H__
#define CMP0_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "cmp_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize CMP0
 *
 * @param[in]   edge      A value of setting CMP0MOD register. <br>
 *                        Specify the logical sum of the following items.
 * @arg                     Interrupt Edge      : CMP_EDGE_DISABLE, or CMP_EDGE_FALLING or CMP_EDGE_RISING or CMP_EDGE_BOTH
 * @param[in]   sampling  A value of setting CMP0MOD register. <br>
 *                        Specify the logical sum of the following items.
 * @arg                     Sampling and Clock source      : CMP_SAMPLING_DIS, or CMP_SAMPLING_OSCLK or CMP_SAMPLING_LSCLK
 * @param[in]   div       A value of setting CMP0MOD register. <br>
 *                        Specify the logical sum of the following items.
 * @arg                     Sampling division ratio      : CMP_DIV_DIV1, or CMP_DIV_DIV2, ( ... ), or CMP_DIV_DIV128

 * @return      None
 */
#define         cmp0_init( edge, sampling, div )   write_reg32( CMP->CMP0MOD, ((edge) | (sampling) | (div)) )

/**
 * Activates CMP0 (Enable)
 *
 * @param       -
 * @return      None
 */
#define         cmp0_ena()                    set_bit( CMP->CMPCON, CMPCON_CMP0EN )

/**
 * Deactivates CMP0 (Disable)
 *
 * @param       -
 * @return      None
 */
#define         cmp0_dis()                   clear_bit( CMP->CMPCON, CMPCON_CMP0EN )

/**
 * Clear CMP0 interrupt status
 *
 * @param       -
 * @return      None
 */
#define         cmp0_clrIntStatus()          set_bit( CMP->CMPINTS, CMPINTS_CMP0IS )


int8_t          cmp0_getResult( void );
int8_t          cmp0_getIntStatus( void );

#endif /*CMP0_H__*/

/** @} */
/** @} */
