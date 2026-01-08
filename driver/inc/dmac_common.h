/******************************************************************************
 dmac_common.h

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
 * @file    dmac_common.h
 *
 * This module is 'DMAC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup DMAC
 *  @{
 *
 *  @brief DMAC register related
 *
 *  @file
 */

#ifndef DMAC_COMMON_H_
#define DMAC_COMMON_H_

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*<=== API parameters value ===*/
/* Initial configuration parameters for dmac*_init() */
#define DMAC_REQ_SSIOF0_RX        ( 0x01U )               /**< DMA request select SSIOF0 (RX)                 */
#define DMAC_REQ_SSIOF0_TX        ( 0x02U )               /**< DMA request select SSIOF0 (TX)                 */
#define DMAC_REQ_SSIOF1_RX        ( 0x03U )               /**< DMA request select SSIOF1 (RX)                 */
#define DMAC_REQ_SSIOF1_TX        ( 0x04U )               /**< DMA request select SSIOF1 (TX)                 */
#define DMAC_REQ_UARTF1_RX        ( 0x05U )               /**< DMA request select UART0 (RX)                  */
#define DMAC_REQ_UARTF1_TX        ( 0x06U )               /**< DMA request select UART0 (TX)                  */
#define DMAC_REQ_UARTF0_RX        ( 0x07U )               /**< DMA request select UARTF (RX)                  */
#define DMAC_REQ_UARTF0_TX        ( 0x08U )               /**< DMA request select UARTF (TX)                  */
#define DMAC_REQ_I2CF_RX          ( 0x09U )               /**< DMA request select I2CF (RX)                   */
#define DMAC_REQ_I2CF_TX          ( 0x0AU )               /**< DMA request select I2CF (TX)                   */
#define DMAC_REQ_SAADC1           ( 0x0BU )               /**< DMA request select SA-ADC1                     */
#define DMAC_REQ_SAADC0           ( 0x0CU )               /**< DMA request select SA-ADC0                     */
#define DMAC_REQ_UARTF2_RX        ( 0x0DU )               /**< DMA request select UART2 (RX)                  */
#define DMAC_REQ_UARTF2_TX        ( 0x0EU )               /**< DMA request select UART2 (TX)                  */
#define DMAC_REQ_UARTF3_RX        ( 0x0FU )               /**< DMA request select UART3 (RX)                  */
#define DMAC_REQ_UARTF3_TX        ( 0x10U )               /**< DMA request select UART3 (TX)                  */
#define DMAC_REQ_NOTSEL           ( 0x00U )               /**< DMA request Neo selected                       */
#define DMAC_STA_NOUNTRANS        ( 0 )                   /**< No untransferred data present                  */
#define DMAC_STA_UNTRANS          ( 1 )                   /**< Untransferred data present                     */
#define DMAC_IREQ_GEN             ( 0 )                   /**< Does not generate interrupt request            */
#define DMAC_IREQ_NOGEN           ( 1 )                   /**< Generates Interrupt request                    */
#define DMAC_ISTA_NOMAL           ( 0 )                   /**< Normal end                                     */
#define DMAC_ISTA_ABNOMAL         ( 1 )                   /**< Abnormal end                                   */
#define DMAC_ISTP_READ            ( 0 )                   /**< Abnormal end of read cycle                     */
#define DMAC_ISTP_WRITE           ( 1 )                   /**< Abnormal end of write cycle                    */
#define DMAC_TMOD_ARQ_EXT         ( 0x00000000UL )        /**< External transfer request                      */
#define DMAC_TMOD_ARQ_AUTO        ( 0x00000001UL )        /**< Auto request transfer request                  */
#define DMAC_TMOD_TSIZ_BYTE       ( 0x00000000UL )        /**< Byte transfer                                  */
#define DMAC_TMOD_TSIZ_HWORD      ( 0x00000002UL )        /**< Half-word transfer                             */
#define DMAC_TMOD_TSIZ_WORD       ( 0x00000004UL )        /**< Word transfer                                  */
#define DMAC_TMOD_SDP_FIX         ( 0x00000000UL )        /**< Transfer source Fixed address device           */
#define DMAC_TMOD_SDP_INC         ( 0x00000008UL )        /**< Transfer source Increment address device       */
#define DMAC_TMOD_DDP_FIX         ( 0x00000000UL )        /**< Transfer destination Fixed address device      */
#define DMAC_TMOD_DDP_INC         ( 0x00000010UL )        /**< Transfer destination Increment address device  */
#define DMAC_TMOD_BRQ_BURST       ( 0x00000000UL )        /**< Burst mode                                     */
#define DMAC_TMOD_BRQ_CYCLE       ( 0x00000020UL )        /**< Cycle steal mode                               */
#define DMAC_TMOD_IMK_IMASK_REL   ( 0x00000000UL )        /**< Releases interrupt mask                        */
#define DMAC_TMOD_IMK_IMASK       ( 0x00000040UL )        /**< Interrupt mask                                 */


/**
 * @brief Status of DMAC transfer result
 *
 */
typedef struct {
    int8_t status;        /**< transfer trigger           */
    int8_t ireq;          /**< transfer unit              */
    int8_t ista;          /**< destination address mode   */
    int8_t istp;          /**< source address mode        */
} dmacStatus_t;


/*=== callback function ===*/
/**
 * DMAC callback function 
 *
 * @param       status  status of DMAC transfer result 
 * @return      None
 */
typedef void (*cbfDmac_t)( dmacStatus_t status );


/**
 * @brief Control parameter for Dmac
 *
 */
typedef struct {
    cbfDmac_t callback;          /**< callback function        */
} dmacCtrlParam_t;



#endif /**<DMAC_COMMON_H_*/

/** @} */
/** @} */
