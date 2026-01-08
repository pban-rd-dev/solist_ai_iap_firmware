/*****************************************************************************
 vls.c

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
 * @file    vls.c
 *
 * This module is 'VLS' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup VLS
 *  @{
 *
 *  @brief Voltage Level Supervisor register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "vls.h"

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
 * Initialize VLS
 *
 * @param[in]   vlslv       Selection of VLS voltage (VLS_LV_235 ~ VLS_LV_399)
 *
 * @param[in]   vlsmod      Configuration of "VLS mode" and "VLS reset and interrupt" <br>
 *                          Specify the logical sum of the following items.
 * @arg                         VLS mode                : VLS_AMD_SINGLE_1, VLS_AMD_SINGLE_2, VLS_AMD_SUPERVISOR
 * @arg                         VLS reset and interrupt : VLS_RST_DIS_INT_DIS, VLS_RST_ENA_INT_DIS1, VLS_RST_DIS_INT_ENA, VLS_RST_ENA_INT_DIS2
 *
 * @param[in]   vlssmp      Configuration of VLS sampling                  <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Sampling       : VLS_SAMPLING_DIS or VLS_SAMPLING_ENA
 * @arg                         Sampling clock : VLS_DIV_DIV1 ~ VLS_DIV_DIV8
 *
 * @return      None
 */
void vls_init( uint8_t vlslv, uint8_t vlsmod, uint8_t vlssmp )
{
	/* Configuration of VLS  */
	write_reg32( VLS->VLS0LV, vlslv );
	write_reg32( VLS->VLS0SMP, vlssmp );
	write_reg32( VLS->VLS0MOD, vlsmod );
}

/** @} */
/** @} */
