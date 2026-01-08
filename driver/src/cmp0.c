/*****************************************************************************
 cmp0.c

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
 * @file    cmp0.c
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "cmp_common.h"
#include "cmp0.h"

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
 * Get result of CMP0
 *
 * @param       -
 * @return      Result of Comparator
 * @retval      CMP_M_HIGH : The CMP0M terminal is larger than the CMP0P terminal
 * @retval      CMP_P_HIGH : The CMP0P terminal is larger than the CMP0M terminal
 *
 */
int8_t cmp0_getResult( void )
{
	int8_t ret = 0U;
	
	/* Get result of CMP  */
	if( (read_reg32( CMP->CMPCON ) & CMPCON_CMP0D) != 0 ) {
		ret = (int8_t)CMP_P_HIGH;
	}
	else {
		ret = (int8_t)CMP_M_HIGH;
	}
	
	return ret;
}

/**
 * Get interrupt status of CMP0
 *
 * @return      Result of Comparator
 * @retval      0              : No Interrupt or clear(initial value)
 * @retval      "other than 0" : Interrupt occurred
 *
 */
int8_t cmp0_getIntStatus( void )
{
	int8_t ret = 0U;
	
	/* Get interrupt status of CMP  */
	if( (read_reg32( CMP->CMPINTS ) & CMPINTS_CMP0IS) == 0 ) {
		ret = (int8_t)0;
	}
	else {
		ret = (int8_t)1;
	}
	
	return ret;
}

/** @} */
/** @} */
