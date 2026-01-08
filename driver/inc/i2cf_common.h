/*****************************************************************************
 i2cf_common.h

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
 * @file    i2cf_common.h
 *
 * This module is 'I2CF' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup I2CF
 *  @{
 *
 *  @brief I2C Bus Interface with FIFO register related
 *
 *  @file
 */

#ifndef _I2CF_COMMON_H_
#define _I2CF_COMMON_H_

#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* I2FnCTL */
#define I2FnCTL_I2FnMD0             ( 0x0001 )                    /**< I2FnCTL Register I2FnMD0 bit                             */
#define I2FnCTL_I2FnMD1             ( 0x0002 )                    /**< I2FnCTL Register I2FnMD1 bit                             */
#define I2FnCTL_I2FnASIE            ( 0x0040 )                    /**< I2FnCTL Register I2FnASIE bit                            */

#define I2FnCTL_I2FnCFIE            ( 0x0200 )                    /**< I2FnCTL Register I2FnCFIE bit                            */
#define I2FnCTL_I2FnSTPI            ( 0x0800 )                    /**< I2FnCTL Register I2FnSTPI bit                            */
#define I2FnCTL_I2FnDLEN            ( 0x1000 )                    /**< I2FnCTL Register I2FnDLEN bit                            */

/* I2FnBFR */
#define I2FnBFR_I2FnBSL0            ( 0x0001 )                    /**< I2FnBFR Register I2FnBSL0 bit                            */
#define I2FnBFR_I2FnBSL1            ( 0x0002 )                    /**< I2FnBFR Register I2FnBSL1 bit                            */ 
#define I2FnBFR_I2FnBMRW            ( 0x0008 )                    /**< I2FnBFR Register I2FnBMRW bit                            */

/* I2FnBMK */
#define I2FnBMK_I2FnBFIE            ( 0x0001 )                    /**< I2FnBMK Register I2FnBFIE bit                            */
#define I2FnBMK_I2FnBNIE            ( 0x0004 )                    /**< I2FnBMK Register I2FnBNIE bit                            */
#define I2FnBMK_I2FnBTIE            ( 0x0008 )                    /**< I2FnBMK Register I2FnBTIE bit                            */
#define I2FnBMK_I2FnBSIE            ( 0x0010 )                    /**< I2FnBMK Register I2FnBSIE bit                            */
#define I2FnBMK_I2FnBAIE            ( 0x0020 )                    /**< I2FnBMK Register I2FnBAIE bit                            */
#define I2FnBMK_I2FnBDIE            ( 0x0040 )                    /**< I2FnBMK Register I2FnBDIE bit                            */

/*=== API parameters value ===*/
/* Initial configuration parameters for i2cf*_init() */
#define I2F_MOD_STD                 ( 0 )                         /**< Transmission mode : Standard mode(100kHz)                */
#define I2F_MOD_FST                 ( I2FnCTL_I2FnMD0 )           /**< Transmission mode : Fast mode(400kHz)                    */

#define I2F_MAAS_DIS                ( 0 )                         /**< Disables MAAS interrupt                                  */
#define I2F_MAAS_ENA                ( I2FnCTL_I2FnASIE )          /**< Enables MAAS interrupt                                   */
#define I2F_MCF_DIS                 ( 0 )                         /**< Disables MCF interrupt                                   */
#define I2F_MCF_ENA                 ( I2FnCTL_I2FnCFIE )          /**< Enables MCF interrupt                                    */
#define I2F_STP_DIS                 ( 0 )                         /**< Disables STP interrupt                                   */
#define I2F_STP_ENA                 ( I2FnCTL_I2FnSTPI )          /**< Enables STP interrupt                                    */
#define I2F_DRLD_DIS                ( 0 )                         /**< Disables DR_LD interrupt                                 */
#define I2F_DRLD_ENA                ( I2FnCTL_I2FnDLEN )          /**< Enables DR_LD interrupt                                  */

#define I2F_BMFT_DIS                ( 0 )                         /**< Disables I2FnBMFT interrupt                              */
#define I2F_BMFT_ENA                ( I2FnBMK_I2FnBFIE )          /**< Enables I2FnBMFT interrupt                               */
#define I2F_BNIE_DIS                ( 0 )                         /**< Disables I2FnBMNA interrupt                              */
#define I2F_BNIE_ENA                ( I2FnBMK_I2FnBNIE )          /**< Enables I2FnBMNA interrupt                               */
#define I2F_BTIE_DIS                ( 0 )                         /**< Disables I2FnBMTO interrupt                              */
#define I2F_BTIE_ENA                ( I2FnBMK_I2FnBTIE )          /**< Enables I2FnBMTO interrupt                               */
#define I2F_BSIE_DIS                ( 0 )                         /**< Disables I2FnBMIS interrupt                              */
#define I2F_BSIE_ENA                ( I2FnBMK_I2FnBSIE )          /**< Enables I2FnBMIS interrupt                               */
#define I2F_BAIE_DIS                ( 0 )                         /**< Disables I2FnBMAG interrupt                              */
#define I2F_BAIE_ENA                ( I2FnBMK_I2FnBAIE )          /**< Enables I2FnBMAG interrupt                               */
#define I2F_BDIE_DIS                ( 0 )                         /**< Disables I2FnBMDZ interrupt                              */
#define I2F_BDIE_ENA                ( I2FnBMK_I2FnBDIE )          /**< Enables I2FnBMDZ interrupt                               */

#define I2F_SUB_NOT                 ( 0 )                                           /**< Not sent sub address                   */
#define I2F_SUB_SENT0               (                0 | I2FnBFR_I2FnBSL0 )         /**< Sent sub address 0                     */
#define I2F_SUB_SENT10              ( I2FnBFR_I2FnBSL1 | 0                )         /**< Sent sub address 1 and 0               */

#define I2F_BUF_DIR_RECV            ( I2FnBFR_I2FnBMRW )                            /**< Data transfer direction : reception    */
#define I2F_BUF_DIR_TRANS           ( 0 )                                           /**< Data transfer direction : transmission */

#define I2F_TR_RECV                 ( 0 )                         /**< Received by master                                       */
#define I2F_TR_TRANS                ( 1 )                         /**< Transmitted by master                                    */

/*=== API return value ===*/
#define I2F_R_OK                    ( 0 )                         /**< Status of OK                                                        */
#define I2F_R_TRANS_FIN             ( 1 )                         /**< Communication of writing/reading data is finished (success/failure) */
#define I2F_R_TRANS_CONT_OK         ( 0 )                         /**< Communication of writing/reading data is continued (success)        */
#define I2F_R_INTERRUPT_NG          ( -1 )                        /**< Abnormal interruption                                               */

/*=== Error value ===*/
#define I2F_ERR_ACR                 ( 1 )                         /**< Error : Nack reception                                              */
#define I2F_ERR_SEND_ERR            ( 2 )                         /**< Error : Transmit error                                              */
#define I2F_ERR_NACK                ( -1 )                        /**< Error : Nack Receiption                                             */
#define I2F_ERR_BMDZ                ( -2 )                        /**< Error : Transferred bytes is 0 of transmission                      */
#define I2F_ERR_BMAG                ( -3 )                        /**< Error : Capacity number of mismatch of transfer bytes and buffer    */
#define I2F_ERR_BMIS                ( -4 )                        /**< Error : Unexpected stop condition                                   */
#define I2F_ERR_BMTO                ( -5 )                        /**< Error : Timeout                                                     */
#define I2F_ERR_BMNA                ( -6 )                        /**< Error : NACK Receiption and terminated transfer                     */

#define I2F_ERR_SLVINIT             ( 7 )                         /**< Error : Slave initial error                    */
#define I2F_ERR_SLVSTP              ( 8 )                         /**< Error : Slave stop condition error             */

/*=== callback function ===*/
/**
 * I2C callback function
 *
 * @param[in]   size        size of reading/writing data
 * @param[in]   errStat     status of I2C operation result      <br>
 *                          0                : Success          <br>
 *                          I2F_ERR_ACR      : Nack reception   <br>
 *                          I2F_ERR_SEND_ERR : Transmit error
 * @return      None
 */
typedef void (*cbfI2f_t)( uint32_t size, uint8_t errStat );

#endif /*_I2CF_COMMON_H_*/

/** @} */
/** @} */
