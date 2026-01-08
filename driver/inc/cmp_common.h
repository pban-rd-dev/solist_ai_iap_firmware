/*****************************************************************************
 cmp_common.h

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
 * @file    cmp_common.h
 *
 * This module is 'CMP' drivers.
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

#ifndef CMP_COMMON_H__
#define CMP_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
#define CMPCON_CMP0EN           ( 0x00000001UL )                    /**< CMPCON Register CMP0EN bit  */
#define CMPCON_CMP1EN           ( 0x00000002UL )                    /**< CMPCON Register CMP1EN bit  */
#define CMPCON_CMP2EN           ( 0x00000004UL )                    /**< CMPCON Register CMP2EN bit  */
#define CMPCON_CMP0D            ( 0x00000100UL )                    /**< CMPCON Register CMP0D bit   */
#define CMPCON_CMP1D            ( 0x00000200UL )                    /**< CMPCON Register CMP1D bit   */
#define CMPCON_CMP2D            ( 0x00000400UL )                    /**< CMPCON Register CMP2D bit   */
#define CMPINTS_CMP0IS          ( 0x00000001UL )                    /**< CMPINTS Register CMP0IS bit */
#define CMPINTS_CMP1IS          ( 0x00000002UL )                    /**< CMPINTS Register CMP1IS bit */
#define CMPINTS_CMP2IS          ( 0x00000004UL )                    /**< CMPINTS Register CMP2IS bit */

/*=== API parameters value ===*/
#define CMP_EDGE_DISABLE        ( 0x00000000UL )                    /**< Interrupt mode : Interrupt Prohibit      */
#define CMP_EDGE_FALLING        ( 0x00000001UL )                    /**< Interrupt mode : Falling Edge Interrupt  */
#define CMP_EDGE_RISING         ( 0x00000002UL )                    /**< Interrupt mode : Rising Edge Interrupt   */
#define CMP_EDGE_BOTH           ( 0x00000003UL )                    /**< Interrupt mode : Both Edge Interrupts    */

#define CMP_SAMPLING_DIS        ( 0x00000000UL )                    /**< Sampling mode : No Sampling              */
#define CMP_SAMPLING_OSCLK      ( 0x00000004UL )                    /**< Sampling mode : With OSCLK Sampling      */
#define CMP_SAMPLING_LSCLK      ( 0x00000008UL )                    /**< Sampling mode : With LSCLK Sampling      */

#define CMP_DIV_DIV1            ( 0x00000000UL )                    /**< Sampling divisioin ratio : No divider    */
#define CMP_DIV_DIV2            ( 0x00000010UL )                    /**< Sampling divisioin ratio : Divide by 2   */
#define CMP_DIV_DIV4            ( 0x00000020UL )                    /**< Sampling divisioin ratio : Divide by 4   */
#define CMP_DIV_DIV8            ( 0x00000030UL )                    /**< Sampling divisioin ratio : Divide by 8   */
#define CMP_DIV_DIV16           ( 0x00000040UL )                    /**< Sampling divisioin ratio : Divide by 16  */
#define CMP_DIV_DIV32           ( 0x00000050UL )                    /**< Sampling divisioin ratio : Divide by 32  */
#define CMP_DIV_DIV64           ( 0x00000060UL )                    /**< Sampling divisioin ratio : Divide by 64  */
#define CMP_DIV_DIV128          ( 0x00000070UL )                    /**< Sampling divisioin ratio : Divide by 128 */

/*=== API return value ===*/
#define CMP_M_HIGH              ( 0x00 )                            /**< The CMP M terminal is larger than the CMP P terminal. */
#define CMP_P_HIGH              ( 0x01 )                            /**< The CMP P terminal is larger than the CMP M terminal. */

#endif /*CMP_COMMON_H__*/

/** @} */
/** @} */
