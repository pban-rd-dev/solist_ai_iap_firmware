/*****************************************************************************
 tbc_common.h

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
 * @file    tbc_common.h
 *
 * This module is 'TBC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup TBC
 *  @{
 *
 *  @brief Common definition for Time Base Counter
 *
 *  @file
 */

#ifndef TBC_COMMON_H__
#define TBC_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== API parameters value ===*/
/* TBC output                                                                */
#define TBC_LTCO_T1HZ        ( 0x0U )           /**< output clock : T1HZ     */
#define TBC_LTCO_T2HZ        ( 0x1U )           /**< output clock : T2HZ     */

/* TBC interrupt                                                             */
#define TBC_LTINS_T128HZ     ( 0x0U )           /**< assigned clock : T128HZ */
#define TBC_LTINS_T64HZ      ( 0x1U )           /**< assigned clock : T64HZ  */
#define TBC_LTINS_T32HZ      ( 0x2U )           /**< assigned clock : T32HZ  */
#define TBC_LTINS_T16HZ      ( 0x3U )           /**< assigned clock : T16HZ  */
#define TBC_LTINS_T8HZ       ( 0x4U )           /**< assigned clock : T8HZ   */
#define TBC_LTINS_T4HZ       ( 0x5U )           /**< assigned clock : T4HZ   */
#define TBC_LTINS_T2HZ       ( 0x6U )           /**< assigned clock : T2HZ   */
#define TBC_LTINS_T1HZ       ( 0x7U )           /**< assigned clock : T1HZ   */
#define TBC_LTINS_NONE       ( 0x8U )           /**< assigned clock : Not assigned */

/* TBC interrupt status                                                      */
#define TBC_INTST_LTBINT0    ( 0x1U )           /**< interrupt status : LTBCINT0 */
#define TBC_INTST_LTBINT1    ( 0x2U )           /**< interrupt status : LTBCINT1 */
#define TBC_INTST_LTBINT2    ( 0x4U )           /**< interrupt status : LTBCINT2 */
#define TBC_INTST_ALL_CLEAR  ( TBC_INTST_LTBINT2 | TBC_INTST_LTBINT1 | TBC_INTST_LTBINT0 ) /**< interrupt status : LTBCINTST All clear */

/* LTBADJ register                                                           */
#define LADJ_MASK            ( 0x00000FFFUL )   /**< LTBADJ register mask    */
#define LADJ_SIGN_BIT        ( 0x00001000UL )   /**< LTBADJ register sign bit */


#endif /*TBC_COMMON_H__*/

/** @} */
/** @} */
