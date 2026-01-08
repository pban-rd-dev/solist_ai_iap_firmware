/*****************************************************************************
 ftimer_common.h

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
 * @file    ftimer_common.h
 *
 * This module is 'FTimer' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup FTimer
 *  @{
 *
 *  @brief Functional Timer register related
 *
 *  @file
 */

#ifndef FTIMER_COMMON_H__
#define FTIMER_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* FTnMOD */
#define FTnMOD_FTnMD0       ( 0x0001 )      /**< FTnMOD Register FTnMD0 bit     */
#define FTnMOD_FTnMD1       ( 0x0002 )      /**< FTnMOD Register FTnMD1 bit     */
#define FTnMOD_FTnDTEN      ( 0x0040 )      /**< FTnMOD Register FTnDTEN        */
#define FTnMOD_FTnOST       ( 0x0080 )      /**< FTnMOD Register FTnOST bit     */
#define FTnMOD_FTnSTPO      ( 0x0100 )      /**< FTnMOD Register FTnSTPO bit    */

/* FTnCLK */
#define FTnCLK_FTnCK0       ( 0x0001 )      /**< FTnCLK Register FTnCK0 bit     */
#define FTnCLK_FTnCK1       ( 0x0002 )      /**< FTnCLK Register FTnCK1 bit     */
#define FTnCLK_FTnCKD0      ( 0x0010 )      /**< FTnCLK Register FTnCKD0 bit    */
#define FTnCLK_FTnCKD1      ( 0x0020 )      /**< FTnCLK Register FTnCKD1 bit    */
#define FTnCLK_FTnCKD2      ( 0x0040 )      /**< FTnCLK Register FTnCKD2 bit    */
#define FTnCLK_FTnXCK0      ( 0x0100 )      /**< FTnCLK Register FTnXCK0 bit    */
#define FTnCLK_FTnXCK1      ( 0x0200 )      /**< FTnCLK Register FTnXCK1 bit    */
#define FTnCLK_FTnXCK2      ( 0x0400 )      /**< FTnCLK Register FTnXCK2 bit    */

/* FTnTRG0 */
#define FTnTRG0_FTnST0      ( 0x0001 )      /**< FTnTRG0 Register FTnST0 bit    */
#define FTnTRG0_FTnST1      ( 0x0002 )      /**< FTnTRG0 Register FTnST1 bit    */
#define FTnTRG0_FTnEXCL     ( 0x0004 )      /**< FTnTRG0 Register FTnEXCL bit   */
#define FTnTRG0_FTnCST      ( 0x0008 )      /**< FTnTRG0 Register FTnCST bit    */
#define FTnTRG0_FTnSTS0     ( 0x0100 )      /**< FTnTRG0 Register FTnSTS0 bit   */
#define FTnTRG0_FTnSTS1     ( 0x0200 )      /**< FTnTRG0 Register FTnSTS1 bit   */
#define FTnTRG0_FTnSTS2     ( 0x0400 )      /**< FTnTRG0 Register FTnSTS2 bit   */
#define FTnTRG0_FTnSTS3     ( 0x0800 )      /**< FTnTRG0 Register FTnSTS3 bit   */
#define FTnTRG0_FTnSTSS     ( 0x8000 )      /**< FTnTRG0 Register FTnSTSS bit   */

/* FTnTRG1 */
#define FTnTRG1_FTnTRM0     ( 0x00010000 )      /**< FTnTRG1 Register FTnTRM0 bit   */
#define FTnTRG1_FTnTRM1     ( 0x00020000 )      /**< FTnTRG1 Register FTnTRM1 bit   */
#define FTnTRG1_FTnEST0     ( 0x00100000 )      /**< FTnTRG1 Register FTnTRM0 bit   */
#define FTnTRG1_FTnEST1     ( 0x00200000 )      /**< FTnTRG1 Register FTnTRM1 bit   */

/* FTnCON0 */
#define FTnCON0_FTnRUN      ( 0x00000001 )  /**< FTnCON0 Register FTnRUN bit    */
#define FTnCON0_FTnGTEN     ( 0x00000010 )  /**< FTnCON0 Register FTnGTEN bit   */
#define FTnCON0_FTnEMGEN    ( 0x00000100 )  /**< FTnCON0 Register FTnEMGEN bit  */
#define FTnCON0_FTnSDN      ( 0x00001000 )  /**< FTnCON0 Register FTnSDN bit    */

/* FTnCON1 */
#define FTnCON1_FTnUD       ( 0x00000001 )  /**< FTnCON Register FTnUD bit      */
#define FTnCON1_FLGA        ( 0x00000010 )  /**< FTnCON Register FTnFLGA bit    */
#define FTnCON1_FLGB        ( 0x00000020 )  /**< FTnCON Register FTnFLGB bit    */
#define FTnCON1_FLGC        ( 0x00000040 )  /**< FTnCON Register FTnFLGC bit    */
#define FTnCON1_FTnSTA      ( 0x00000080 )  /**< FTnCON Register FTnSTA bit     */

/* FTnINTE */
#define FTnINTE_FTnIEP      ( 0x0001 )      /**< FTnINTE Register FTnIEP bit    */
#define FTnINTE_FTnIEA      ( 0x0002 )      /**< FTnINTE Register FTnIEA bit    */
#define FTnINTE_FTnIEB      ( 0x0004 )      /**< FTnINTE Register FTnIEB bit    */
#define FTnINTE_FTnIETS     ( 0x0008 )      /**< FTnINTE Register FTnIETS bit   */
#define FTnINTE_FTnIETR     ( 0x0010 )      /**< FTnINTE Register FTnIETR bit   */
#define FTnINTE_FTnIOP      ( 0x0100 )      /**< FTnINTE Register FTnIOP bit    */
#define FTnINTE_FTnIOA      ( 0x0200 )      /**< FTnINTE Register FTnIOA bit    */
#define FTnINTE_FTnIOB      ( 0x0400 )      /**< FTnINTE Register FTnIOB bit    */

/* FTnINTC */
#define FTnINTC_FTnICP      ( FTnINTE_FTnIEP )  /**< FTnINTC Register FTnICP bit    */
#define FTnINTC_FTnICA      ( FTnINTE_FTnIEA )  /**< FTnINTC Register FTnICA bit    */
#define FTnINTC_FTnICB      ( FTnINTE_FTnIEB )  /**< FTnINTC Register FTnICB bit    */
#define FTnINTC_FTnICTS     ( FTnINTE_FTnIETS ) /**< FTnINTC Register FTnICTS bit   */
#define FTnINTC_FTnICTR     ( FTnINTE_FTnIETR ) /**< FTnINTC Register FTnICTR bit   */
#define FTnINTC_FTnICES     ( 0x0020 )          /**< FTnINTC Register FTnICES bit   */
#define FTnINTC_FTnIR       ( 0x8000 )          /**< FTnINTC Register FTnIR bit     */
#define FTnINTC_ALL_CLEAR   ( FTnINTC_FTnICP | FTnINTC_FTnICA | FTnINTC_FTnICB | FTnINTC_FTnICTS | FTnINTC_FTnICTR | FTnINTC_FTnICES ) /**< FTnINTC Register All clear */

/* FTOxSL                        */
/*   (Note:'x' is TMOUT number.) */
#define FTOxSL_FTOxS0        ( 0x01 )       /**< FTOxSL Register FTOxS0 bit     */
#define FTOxSL_FTOxS1        ( 0x02 )       /**< FTOxSL Register FTOxS1 bit     */
#define FTOxSL_FTOxS2        ( 0x04 )       /**< FTOxSL Register FTOxS2 bit     */
#define FTOxSL_FTOxSN        ( 0x10 )       /**< FTOxSL Register FTOxSN bit     */


/*=== API parameters value ===*/
/* Initial configuration parameters for ftimerX_init() */
#define FTM_MD_TIMER        ( 0 )                                               /**< Mode : Timer                                     */
#define FTM_MD_CAPTURE      ( FTnMOD_FTnMD0 )                                   /**< Mode : Capture                                   */
#define FTM_MD_PWM1         ( FTnMOD_FTnMD1 )                                   /**< Mode : PWM1                                      */
#define FTM_MD_PWM2         ( FTnMOD_FTnMD1 | FTnMOD_FTnMD0 )                   /**< Mode : PWM2                                      */

#define FTM_DTEN_DIS        ( 0 )                                               /**< Dead time : disable                              */
#define FTM_DTEN_ENA        ( FTnMOD_FTnDTEN )                                  /**< Dead time : enable                               */

#define FTM_OST_RELOAD      ( 0 )                                               /**< Reload mode (for Timer/PWM) : auto reload        */
#define FTM_OST_ONESHOT     ( FTnMOD_FTnOST )                                   /**< Reload mode (for Timer/PWM) : one shout          */
#define FTM_OST_CAP_AUTO    ( 0 )                                               /**< Capture mode (for Capture)  : auto               */
#define FTM_OST_CAP_SINGLE  ( FTnMOD_FTnOST )                                   /**< Capture mode (for Capture)  : single             */

#define FTM_STPO_LOW        ( 0 )                                               /**< Output signal when FTimer is stoped : low        */
#define FTM_STPO_KEEP       ( FTnMOD_FTnSTPO )                                  /**< Output signal when FTimer is stoped : keep       */

#define FTM_CK_LSCLK        ( 0 )                                               /**< Clock source : LSCLK                             */
#define FTM_CK_OSCLK        ( FTnCLK_FTnCK0 )                                   /**< Clock source : OSCLK                             */
#define FTM_CK_HSCLK        ( FTnCLK_FTnCK1 )                                   /**< Clock source : HSCLK                            */
#define FTM_CK_EXTCLK       ( FTnCLK_FTnCK1 | FTnCLK_FTnCK0 )                   /**< Clock source : EXTCLK                            */

#define FTM_CKD_DIV1        ( 0 )                                               /**< Division ratio :Not divide                       */
#define FTM_CKD_DIV2        ( FTnCLK_FTnCKD0 )                                  /**< Division ratio :1/2                              */
#define FTM_CKD_DIV4        ( FTnCLK_FTnCKD1 )                                  /**< Division ratio :1/4                              */
#define FTM_CKD_DIV8        ( FTnCLK_FTnCKD1 | FTnCLK_FTnCKD0 )                 /**< Division ratio :1/8                              */
#define FTM_CKD_DIV16       ( FTnCLK_FTnCKD2 )                                  /**< Division ratio :1/16                             */
#define FTM_CKD_DIV32       ( FTnCLK_FTnCKD2 | FTnCLK_FTnCKD0 )                 /**< Division ratio :1/32                             */
#define FTM_CKD_DIV64       ( FTnCLK_FTnCKD2 | FTnCLK_FTnCKD1 )                 /**< Division ratio :1/64                             */

#define FTM_XCK_TMCKI0      ( 0 )                                                /**< External clock source : TMCKI0                  */
#define FTM_XCK_TMCKI1      ( FTnCLK_FTnXCK0 )                                   /**< External clock source : TMCKI1                  */
#define FTM_XCK_TMCKI2      ( FTnCLK_FTnXCK1 )                                   /**< External clock source : TMCKI2                  */
#define FTM_XCK_TMCKI3      ( FTnCLK_FTnXCK1 | FTnCLK_FTnXCK0 )                  /**< External clock source : TMCKI3                  */
#define FTM_XCK_TMCKI4      ( FTnCLK_FTnXCK2 )                                   /**< External clock source : TMCKI4                  */
#define FTM_XCK_TMCKI5      ( FTnCLK_FTnXCK2 | FTnCLK_FTnXCK0 )                  /**< External clock source : TMCKI5                  */
#define FTM_XCK_TMCKI6      ( FTnCLK_FTnXCK2 | FTnCLK_FTnXCK1 )                  /**< External clock source : TMCKI6                  */
#define FTM_XCK_TMCKI7      ( FTnCLK_FTnXCK2 | FTnCLK_FTnXCK1 | FTnCLK_FTnXCK0 ) /**< External clock source : TMCKI7                  */

/* Parameters for ftimerX_setTriggerParam() */
#define FTM_ST0_DIS     ( 0 )                                               /**< Counter start : disable                          */
#define FTM_ST0_ENA     ( FTnTRG0_FTnST0 )                                  /**< Counter start : enable                           */
#define FTM_ST1_DIS     ( 0 )                                               /**< Counter stop  : disable                          */
#define FTM_ST1_ENA     ( FTnTRG0_FTnST1 )                                  /**< Counter stop  : enable                           */
#define FTM_EXCL_DIS    ( 0 )                                               /**< Counter clear  : disable                         */
#define FTM_EXCL_ENA    ( FTnTRG0_FTnEXCL )                                 /**< Counter clear  : enable                          */
#define FTM_CST_DIS    ( 0 )                                                /**< Counter start when counting is stoped : disable  */
#define FTM_CST_ENA    ( FTnTRG0_FTnCST )                                   /**< Counter start when counting is stoped : enable   */
#define FTM_STS_EXI0   ( 0 )                                                                           /**< Trigger event source : EXI0    */
#define FTM_STS_EXI1   ( FTnTRG0_FTnSTS0 )                                                             /**< Trigger event source : EXI1    */
#define FTM_STS_EXI2   ( FTnTRG0_FTnSTS1 )                                                             /**< Trigger event source : EXI2    */
#define FTM_STS_EXI3   ( FTnTRG0_FTnSTS1 | FTnTRG0_FTnSTS0 )                                           /**< Trigger event source : EXI3    */
#define FTM_STS_EXI4   ( FTnTRG0_FTnSTS2 )                                                             /**< Trigger event source : EXI4    */
#define FTM_STS_EXI5   ( FTnTRG0_FTnSTS2 | FTnTRG0_FTnSTS0 )                                           /**< Trigger event source : EXI5    */
#define FTM_STS_EXI6   ( FTnTRG0_FTnSTS2 | FTnTRG0_FTnSTS1 )                                           /**< Trigger event source : EXI6    */
#define FTM_STS_EXI7   ( FTnTRG0_FTnSTS2 | FTnTRG0_FTnSTS1 | FTnTRG0_FTnSTS0 )                         /**< Trigger event source : EXI7    */
#define FTM_STS_TM0    ( FTnTRG0_FTnSTSS | 0 )                                                         /**< Trigger event source : TM0     */
#define FTM_STS_TM1    ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS0 )                                           /**< Trigger event source : TM1     */
#define FTM_STS_TM2    ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS1 )                                           /**< Trigger event source : TM2     */
#define FTM_STS_TM3    ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS1 | FTnTRG0_FTnSTS0 )                         /**< Trigger event source : TM3     */
#define FTM_STS_TM4    ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS2 )                                           /**< Trigger event source : TM4     */
#define FTM_STS_TM5    ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS2 | FTnTRG0_FTnSTS0 )                         /**< Trigger event source : TM5     */
#define FTM_STS_FTM0   ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS3 | 0 )                                       /**< Trigger event source : FTM0    */
#define FTM_STS_FTM1   ( FTnTRG0_FTnSTSS | FTnTRG0_FTnSTS3 | FTnTRG0_FTnSTS0 )                         /**< Trigger event source : FTM1    */

#define FTM_TRM_EDGE_RISE_RISE   ( 0 )                                         /**< Trigger event edge : Counter start : rising edge,  Counter stop/clear : rising edge  */
#define FTM_TRM_EDGE_FALL_RISE   ( FTnTRG1_FTnTRM0 )                           /**< Trigger event edge : Counter start : falling edge, Counter stop/clear : rising edge  */
#define FTM_TRM_EDGE_RISE_FALL   ( FTnTRG1_FTnTRM1 )                           /**< Trigger event edge : Counter start : rising edge,  Counter stop/clear : falling edge */
#define FTM_TRM_EDGE_FALL_FALL   ( FTnTRG1_FTnTRM1 | FTnTRG1_FTnTRM0 )         /**< Trigger event edge : Counter start : falling edge, Counter stop/clear : falling edge */
#define FTM_EST_EXI0  ( 0 )                                                    /**< Emergency trigger source : EXI0 */
#define FTM_EST_EXI4  ( FTnTRG1_FTnEST0 )                                      /**< Emergency trigger source : EXI4 */
#define FTM_EST_CMP0  ( FTnTRG1_FTnEST1 )                                      /**< Emergency trigger source : CMP0 */
#define FTM_EST_CMP1  ( FTnTRG1_FTnEST1 | FTnTRG1_FTnEST0 )                    /**< Emergency trigger source : CMP1 */

/* Interrupt parameters for ftimerX_setInt() */
#define FTM_IEP_DIS       ( 0 )                                         /**< FTnIEP  : disable               */
#define FTM_IEP_ENA       ( FTnINTE_FTnIEP )                            /**< FTnIEP  : enable                */
#define FTM_IEA_DIS       ( 0 )                                         /**< FTnIEA  : disable               */
#define FTM_IEA_ENA       ( FTnINTE_FTnIEA )                            /**< FTnIEA  : enable                */
#define FTM_IEB_DIS       ( 0 )                                         /**< FTnIEB  : disable               */
#define FTM_IEB_ENA       ( FTnINTE_FTnIEB )                            /**< FTnIEB  : enable                */
#define FTM_IETS_DIS      ( 0 )                                         /**< FTnIETS : disable               */
#define FTM_IETS_ENA      ( FTnINTE_FTnIETS )                           /**< FTnIETS : enable                */
#define FTM_IETR_DIS      ( 0 )                                         /**< FTnIETR : disable               */
#define FTM_IETR_ENA      ( FTnINTE_FTnIETR )                           /**< FTnIETR : enable                */
#define FTM_IOP_DIS       ( 0 )                                         /**< FTnIOP  : disable               */
#define FTM_IOP_ENA       ( FTnINTE_FTnIOP )                            /**< FTnIOP  : enable                */
#define FTM_IOA_DIS       ( 0 )                                         /**< FTnIOA  : disable               */
#define FTM_IOA_ENA       ( FTnINTE_FTnIOA )                            /**< FTnIOA  : enable                */
#define FTM_IOB_DIS       ( 0 )                                         /**< FTnIOB  : disable               */
#define FTM_IOB_ENA       ( FTnINTE_FTnIOB )                            /**< FTnIOB  : enable                */

/* Parameters for ftimerX_start() */
#define FTM_RUN_STOP         ( 0 )                                      /**< Run     : count stop            */
#define FTM_RUN_COUNT        ( FTnCON0_FTnRUN )                         /**< Run     : counting              */
#define FTM_TGEN_DIS         ( 0 )                                      /**< Trigger : disable               */
#define FTM_TGEN_ENA         ( FTnCON0_FTnGTEN )                        /**< Trigger : enable                */
#define FTM_EMGEN_DIS        ( 0 )                                      /**< Emergency stop : disable        */
#define FTM_EMGEN_ENA        ( FTnCON0_FTnEMGEN )                       /**< Emergency stop : enable         */
#define FTM_SDN_DIS          ( 0 )                                      /**< Emergency stop : disable        */
#define FTM_SDN_ENA          ( FTnCON0_FTnSDN )                         /**< Emergency stop : enable         */


/* Parameters for ftimer_selectOutput() */
/**
 * Select TMOUT No.
 */
enum {
    FTM_TMOUT_SEL_0,                        /**< Select TMOUT0 */
    FTM_TMOUT_SEL_1,                        /**< Select TMOUT1 */
    FTM_TMOUT_SEL_2,                        /**< Select TMOUT2 */
    FTM_TMOUT_SEL_3,                        /**< Select TMOUT3 */
    FTM_TMOUT_SEL_4,                        /**< Select TMOUT4 */
    FTM_TMOUT_SEL_5,                        /**< Select TMOUT5 */
    FTM_TMOUT_SEL_6,                        /**< Select TMOUT6 */
    FTM_TMOUT_SEL_7,                        /**< Select TMOUT7 */

    FTM_TMOUT_SEL_NUM                       /**< The number of all TMOUTs */
};

/* TMOUT mapping */
#define FTM_FTO_FTM0P       ( 0x00 )                                            /**< TMOUTx : FTIMER0P     */
#define FTM_FTO_FTM0N       ( FTOxSL_FTOxS0 )                                   /**< TMOUTx : FTIMER0N     */
#define FTM_FTO_FTM1P       ( FTOxSL_FTOxS1 )                                   /**< TMOUTx : FTIMER1P     */
#define FTM_FTO_FTM1N       ( FTOxSL_FTOxS1 | FTOxSL_FTOxS0 )                   /**< TMOUTx : FTIMER1N     */

/* Output polarity */
#define FTM_FTO_NEG_POS     ( 0x00 )                                            /**< TMOUTx : Normal(positive)  */
#define FTM_FTO_NEG_NEG     ( FTOxSL_FTOxSN )                                   /**< TMOUTx : Negative          */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Selection of output TMOUT signal mapping
 * 
 * @param[in]   tmoutNo     Select TMOUT
 * @arg                     FTM_TMOUT_SEL_0 : Selecting TMOUT0
 * @arg                     FTM_TMOUT_SEL_1 : Selecting TMOUT1
 * @arg                     ( ... )
 * @arg                     FTM_TMOUT_SEL_6 : Selecting TMOUT6
 * @arg                     FTM_TMOUT_SEL_7 : Selecting TMOUT7
 * 
 * @param[in]   ftoxsl      Select FTM output
 * @arg                     FTM_FTO_FTM0P   : Setting FTM0P
 * @arg                     FTM_FTO_FTM0N   : Setting FTM0N
 * @arg                     FTM_FTO_FTM1P   : Setting FTM1P
 * @arg                     FTM_FTO_FTM1N   : Setting FTM1N
 * 
 * @param[in]   ftoxsn      FTM_FTO_NEG_POS : Outputed signal is positive.  <br>
 *                          FTM_FTO_NEG_NEG : Outputed signal is negative.
 * 
 * @return      None
 */
#define         ftimer_selectOutput( tmoutNo, ftoxsl, ftoxsn ) write_reg32( *(&FTIMER->FTOSL0 + (tmoutNo)/4), (uint32_t)((read_reg32(*(&FTIMER->FTOSL0 + (tmoutNo)/4)) & (~((uint32_t)0x17 << (8* ((tmoutNo) & 0x03))))) |(((ftoxsl) | (ftoxsn)) << (8* ((tmoutNo) & 0x03))) ))


#endif /*FTIMER_COMMON_H__*/

/** @} */
/** @} */
