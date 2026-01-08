/*****************************************************************************
 crc.c

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
 * @file    crc.c
 *
 * This module is 'CRC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup CRC
 *  @{
 *
 *  @brief CRC Calculator register related
 *
 *  @file
 */

#include "crc.h"

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
 * Initialize CRC value
 *
 * @param[in]       mode  CRC mode                                                      <br>
 *                 (Setting value of CRCMOD register)                                   <br>
 *                   Specify the logical sum of the following items.
 * @arg              Direction  :   CRC_DIRECTION_LSB_FIRST or CRC_DIRECTION_MSB_FIRST
 * @arg              Size       :   CRC_BITWIDTH_8BIT or CRC_BITWIDTH_32BIT
 * @arg              Mode       :   CRC_MODE_CRC16 or CRC_MODE_CRC32
 * 
 * @param[in]       initialVal  CRC calculation initial value                           <br>
 *                 (Setting value of CRCRES register)
 * 
 * @return         None
 */
void crc_Init(uint8_t mode, uint32_t initialVal)
{
	write_reg32( CRC->CRCMOD, mode );

	write_reg32( CRC->CRCRES, initialVal );
}


/**
 * Start CRC calculation
 *
 * @param[in]       *addr  pointer to area where the send data is stored
 * @param[in]       size   size of data (unit is 8bit or 32bit)
 * 
 * @return         The value of calculation result
 */
uint32_t crc_startCalc( void *addr, uint32_t size )
{
	uint32_t i;
	uint32_t *ptr32=(uint32_t *)addr;
	uint8_t *ptr8=(uint8_t *)addr;
	
	if(get_bit(CRC->CRCMOD,0x00000004) != 0){
		for(i=0;i<size;i++) {
			crc_setCalcData( ptr32[i] );
		}
	} else {
		for(i=0;i<size;i++) {
			crc_setCalcData( ptr8[i] );
		}
	}
		
	return crc_getCalcResult();
}

/** @} */
/** @} */
