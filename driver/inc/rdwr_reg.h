/*****************************************************************************
 rdwr_reg.h

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
 * @file    rdwr_reg.h
 *
 * This module is 'Common' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Common
 *  @{
 *
 *  @brief The common definition to read/write SFR registers.
 *
 *  @file
 */

#ifndef RDWR_REG_H__
#define RDWR_REG_H__

#include <stdint.h>

#define read_reg8(a)        ((uint8_t)(a))                             /**< 1Byte read  */
#define read_reg16(a)       ((uint16_t)(a))                            /**< 2Byte read  */
#define read_reg32(a)       ((uint32_t)(a))                            /**< 4Byte read  */
#define write_reg8(a,d)     ((a)=(uint8_t)(d))                         /**< 1Byte write */
#define write_reg16(a,d)    ((a)=(uint16_t)(d))                        /**< 2Byte write */
#define write_reg32(a,d)    ((a)=(uint32_t)(d))                        /**< 4Byte write */
#define set_reg8(a,m)       ((a)|=(uint8_t)(m))                        /**< 1Byte(any bit) set   */
#define set_reg16(a,m)      ((a)|=(uint16_t)(m))                       /**< 2Byte(any bit) set   */
#define set_reg32(a,m)      ((a)|=(uint32_t)(m))                       /**< 4Byte(any bit) set   */
#define clear_reg8(a,m)     ((a)&=~(uint8_t)(m))                       /**< 1Byte(any bit) clear */
#define clear_reg16(a,m)    ((a)&=~(uint16_t)(m))                      /**< 2Byte(any bit) clear */
#define clear_reg32(a,m)    ((a)&=~(uint32_t)(m))                      /**< 4Byte(any bit) clear */

#define set_bit(n,c)        (set_reg32  (n,c))                                                   /**< byte bit set   */
#define clear_bit(n,c)      (clear_reg32(n,c))                                                   /**< byte bit clear */
#define get_bit(n,c)        (((read_reg32(n)) & (c)) != 0)                                       /**< byte bit read  */
#define write_bit(n,c,d)    do{ clear_bit(n,c);set_bit(n,d); }while(0)                           /**< byte bit write */

#endif /* RDWR_REG_H__ */

/** @} */
/** @} */
