/*****************************************************************************
 crc.h

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
 * @file    crc.h
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

#ifndef CRC_H__
#define CRC_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/


#define CRC_DIRECTION_LSB_FIRST		( 0x00000000UL )					/**<   CRC Direction: LSB First */
#define CRC_DIRECTION_MSB_FIRST		( 0x00000002UL )					/**<   CRC Direction: MSB First */
#define CRC_BITWIDTH_8BIT			( 0x00000000UL )					/**<   CRC Direction: BIT Width 8bit */
#define CRC_BITWIDTH_32BIT			( 0x00000004UL )					/**<   CRC Direction: BIT Width 32bit */
#define CRC_MODE_CRC16				( 0x00000000UL )					/**<   CRC Direction: CRC Calc Mode CRC16/CCITT */
#define CRC_MODE_CRC32				( 0x00000008UL )					/**<   CRC Direction: CRC Calc Mode CRC32 */


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Set CRC calculation data
 *
 * @param[in]   crcdata   CRC calculation data                                                    <br>
 *                      (Setting value of CRCDATA register)                                       <br>
 * @return      None
 */
#define		crc_setCalcData( crcdata )		write_reg32( CRC->CRCDATA, crcdata )


/**
 * Get CRC calculation result
 *
 * @param       -
 * @return      The value of CRC Calculation Result                                               <br>
 *              (A value of CRCRES register)
 */
#define		crc_getCalcResult()				read_reg32( CRC->CRCRES )

/**
 * Set CRC calculation result
 *
 * @param[in]   crcvalue   CRC calculation result                                                 <br>
 *                      (Setting value of CRCRES register)                                        <br>
 * @return      None
 */
#define		crc_setCalcResult( crcvalue )	write_reg32( CRC->CRCRES, crcvalue )


void crc_Init(uint8_t mode, uint32_t initialVal);
uint32_t crc_startCalc( void *addr, uint32_t size );


#endif /* CRC_H__ */

/** @} */
/** @} */
