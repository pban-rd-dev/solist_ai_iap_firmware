/*****************************************************************************
 ssiof_common.h

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
 * @file    ssiof_common.h
 *
 * This module is 'SSIOF' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup SSIOF
 *  @{
 *
 *  @brief Synchronous Serial Port with FIFO register related
 *
 *  @file
 */

#ifndef SSIOF_COMMON_H__
#define SSIOF_COMMON_H__

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* SFnCTRL */
#define SFnCTRL_SFnSPE          ( 0x0001 )                          /**< SFnCTRL Register SFnSPE bit                            */
#define SFnCTRL_SFnMST          ( 0x0002 )                          /**< SFnCTRL Register SFnMST bit                            */
#define SFnCTRL_SFnSIZ          ( 0x0004 )                          /**< SFnCTRL Register SFnSIZ bit                            */
#define SFnCTRL_SFnMDF          ( 0x0008 )                          /**< SFnCTRL Register SFnMDF bit                            */
#define SFnCTRL_SFnLSB          ( 0x0010 )                          /**< SFnCTRL Register SFnLSB bit                            */
#define SFnCTRL_SFnCPHA         ( 0x0020 )                          /**< SFnCTRL Register SFnCPHA bit                           */
#define SFnCTRL_SFnCPOL         ( 0x0040 )                          /**< SFnCTRL Register SFnCPOL bit                           */
#define SFnCTRL_SFnFICL         ( 0x0100 )                          /**< SFnCTRL Register SFnFICL bit                           */
#define SFnCTRL_SFnSSZ          ( 0x0200 )                          /**< SFnCTRL Register SFnSSZ bit                            */
#define SFnCTRL_SFnSOZ          ( 0x0400 )                          /**< SFnCTRL Register SFnSOZ bit                            */
#define SFnCTRL_SFnMOZ          ( 0x0800 )                          /**< SFnCTRL Register SFnMOZ bit                            */

/* SFnINTC */
#define SFnINTC_SFnTFIE         ( 0x0001 )                          /**< SFnINTC Register SFnTFIE bit                           */
#define SFnINTC_SFnRFIE         ( 0x0002 )                          /**< SFnINTC Register SFnRFIE bit                           */
#define SFnINTC_SFnFIE          ( 0x0004 )                          /**< SFnINTC Register SFnFIE bit                            */
#define SFnINTC_SFnORIE         ( 0x0008 )                          /**< SFnINTC Register SFnORIE bit                           */
#define SFnINTC_SFnMFIE         ( 0x0010 )                          /**< SFnINTC Register SFnMFIE bit                           */
#define SFnINTC_SFnTFIC0        ( 0x0100 )                          /**< SFnINTC Register SFnTFIC0 bit                          */
#define SFnINTC_SFnTFIC1        ( 0x0200 )                          /**< SFnINTC Register SFnTFIC1 bit                          */
#define SFnINTC_SFnRFIC0        ( 0x1000 )                          /**< SFnINTC Register SFnRFIC0 bit                          */
#define SFnINTC_SFnRFIC1        ( 0x2000 )                          /**< SFnINTC Register SFnRFIC1 bit                          */

/* SFnSRR */
#define SFnSRR_SFnTFI           ( 0x0001 )                          /**< SFnSRR Register SFnTFI bit                             */
#define SFnSRR_SFnRFI           ( 0x0002 )                          /**< SFnSRR Register SFnRFI bit                             */
#define SFnSRR_SFnFI            ( 0x0004 )                          /**< SFnSRR Register SFnFI bit                              */
#define SFnSRR_SFnORF           ( 0x0008 )                          /**< SFnSRR Register SFnORF bit                             */
#define SFnSRR_SFnMDF           ( 0x0010 )                          /**< SFnSRR Register SFnMDF bit                             */
#define SFnSRR_SFnSPIF          ( 0x0020 )                          /**< SFnSRR Register SFnSPIF bit                            */
#define SFnSRR_SFnWOF           ( 0x0100 )                          /**< SFnSRR Register SFnWOF bit                             */
#define SFnSRR_SFnTFF           ( 0x0200 )                          /**< SFnSRR Register SFnTFF bit                             */
#define SFnSRR_SFnTFE           ( 0x0400 )                          /**< SFnSRR Register SFnTFE bit                             */
#define SFnSRR_SFnRFF           ( 0x0800 )                          /**< SFnSRR Register SFnRFF bit                             */
#define SFnSRR_SFnRFE           ( 0x1000 )                          /**< SFnSRR Register SFnRFE bit                             */

/* SFnSRC */
#define SFnSRC_SFnTFC           ( SFnSRR_SFnTFI   )                 /**< SFnSRC Register SFnTFC bit                             */
#define SFnSRC_SFnRFC           ( SFnSRR_SFnRFI   )                 /**< SFnSRC Register SFnRFC bit                             */
#define SFnSRC_SFnFC            ( SFnSRR_SFnFI    )                 /**< SFnSRC Register SFnFC bit                              */
#define SFnSRC_SFnORFC          ( SFnSRR_SFnORF  )                  /**< SFnSRC Register SFnORFC bit                            */
#define SFnSRC_SFnMDFC          ( SFnSRR_SFnMDF  )                  /**< SFnSRC Register SFnMDFC bit                            */
#define SFnSRC_SFnSPIFC         ( SFnSRR_SFnSPIF )                  /**< SFnSRC Register SFnSPIFC bit                           */
#define SFnSRC_SFnWOFC          ( SFnSRR_SFnWOF  )                  /**< SFnSRC Register SFnWOFC bit                            */

/*=== API return value ===*/
#define SSIOF_R_OK                (  0 )                              /**< Status of OK                                           */
#define SSIOF_R_ERR               ( -1 )                              /**< Noticed error                                          */
#define SSIOF_R_TRANS_FIN         (  1 )                              /**< Reading/writing communication is finished              */
#define SSIOF_R_TRANS_CONT_OK     (  0 )                              /**< Reading/writing communication is continued             */

/*=== API parameters value ===*/
/* Initial configuration parameters for ssiof*_init() */
/* Mode */
#define SSIOF_MST_SLAVE           ( 0 )                               /**< Master/Slave  : Slave                                  */
#define SSIOF_MST_MASTER          ( SFnCTRL_SFnMST )                  /**< Master/Slave  : Master                                 */
#define SSIOF_LG_8BIT             ( 0 )                               /**< Buffer mode   : 8bit                                   */
#define SSIOF_LG_16BIT            ( SFnCTRL_SFnSIZ )                  /**< Buffer mode   : 16bit                                  */
#define SSIOF_MDF_DIS             ( 0 )                               /**< Mode fault    : disable                                */
#define SSIOF_MDF_ENA             ( SFnCTRL_SFnMDF )                  /**< Mode fault    : enable                                 */
#define SSIOF_DIR_LSB             ( 0 )                               /**< Endian        : LSB first                              */
#define SSIOF_DIR_MSB             ( SFnCTRL_SFnLSB )                  /**< Endian        : MSB first                              */
#define SSIOF_CPHA_1SM_2SH        ( 0 )                               /**< Clock phase   : 1st edge=sampling, 2nd edge=shift      */
#define SSIOF_CPHA_1SH_2SM        ( SFnCTRL_SFnCPHA )                 /**< Clock phase   : 1st edge=shift,    2nd edge=sampling   */
#define SSIOF_CPOL_LOW            ( 0 )                               /**< Clock type    : default low                            */
#define SSIOF_CPOL_HIGH           ( SFnCTRL_SFnCPOL )                 /**< Clock type    : default high                           */
#define SSIOF_SSZ_OUTPUT          ( 0 )                               /**< SSn output    : 0/1 output                             */
#define SSIOF_SSZ_HIZ             ( SFnCTRL_SFnSSZ )                  /**< SSn output    : Hi-Z                                   */
#define SSIOF_SOZ_OUTPUT          ( 0 )                               /**< MISO output   : 0/1 output                             */
#define SSIOF_SOZ_HIZ             ( SFnCTRL_SFnSOZ )                  /**< MISO output   : Hi-Z                                   */
#define SSIOF_MOZ_OUTPUT          ( 0 )                               /**< MOSI output   : 0/1 output                             */
#define SSIOF_MOZ_HIZ             ( SFnCTRL_SFnMOZ )                  /**< MOSI output   : Hi-Z                                   */
/* Interrupt */
#define SSIOF_INT_TFIE_DIS        ( 0 )                               /**< SFnTFIE       : disable                                */
#define SSIOF_INT_TFIE_ENA        ( SFnINTC_SFnTFIE )                 /**< SFnTFIE       : enable                                 */
#define SSIOF_INT_RFIE_DIS        ( 0 )                               /**< SFnRFIE       : disable                                */
#define SSIOF_INT_RFIE_ENA        ( SFnINTC_SFnRFIE )                 /**< SFnRFIE       : enable                                 */
#define SSIOF_INT_FIE_DIS         ( 0 )                               /**< SFnFIE        : disable                                */
#define SSIOF_INT_FIE_ENA         ( SFnINTC_SFnFIE )                  /**< SFnFIE        : enable                                 */
#define SSIOF_INT_ORIE_DIS        ( 0 )                               /**< SFnORIE       : disable                                */
#define SSIOF_INT_ORIE_ENA        ( SFnINTC_SFnORIE )                 /**< SFnORIE       : enable                                 */
#define SSIOF_INT_MFIE_DIS        ( 0 )                               /**< SFnMFIE       : disable                                */
#define SSIOF_INT_MFIE_ENA        ( SFnINTC_SFnMFIE )                 /**< SFnMFIE       : enable                                 */
#define SSIOF_INT_WR_THRESH_0     ( 0 )                                     /**< Interrupt threshold (Write) : remain 0 byte      */
#define SSIOF_INT_WR_THRESH_1     ( SFnINTC_SFnTFIC0 )                      /**< Interrupt threshold (Write) : remain 1 byte      */
#define SSIOF_INT_WR_THRESH_2     ( SFnINTC_SFnTFIC1 )                      /**< Interrupt threshold (Write) : remain 2 byte      */
#define SSIOF_INT_WR_THRESH_3     ( SFnINTC_SFnTFIC1 | SFnINTC_SFnTFIC0 )   /**< Interrupt threshold (Write) : remain 3 byte      */
#define SSIOF_INT_RD_THRESH_1     ( 0 )                                     /**< Interrupt threshold (Read)  : 1 byte reception   */
#define SSIOF_INT_RD_THRESH_2     ( SFnINTC_SFnRFIC0 )                      /**< Interrupt threshold (Read)  : 2 byte reception   */
#define SSIOF_INT_RD_THRESH_3     ( SFnINTC_SFnRFIC1 )                      /**< Interrupt threshold (Read)  : 3 byte reception   */
#define SSIOF_INT_RD_THRESH_4     ( SFnINTC_SFnRFIC1 | SFnINTC_SFnRFIC0 )   /**< Interrupt threshold (Read)  : 4 byte reception   */


/* Select SSIOF mode parameter for ssiof*_start() */
#define SSIOF_MODE_RD             (  0 )                              /**< Operation of SSIOF mode : Reading(receive)                */
#define SSIOF_MODE_WR             (  1 )                              /**< Operation of SSIOF mode : Writing(send)                   */
#define SSIOF_MODE_RD_WR          (  2 )                              /**< Operation of SSIOF mode : Reading / Writing(receive/send) */

/* Set baudrate parameter for ssiof*_setBaudrate() */
#define SSIOF_LEAD_05             ( 0x1000U )                         /**< LEAD delay : 0.5 SCK                                   */
#define SSIOF_LEAD_10             ( 0x2000U )                         /**< LEAD delay : 1.0 SCK                                   */
#define SSIOF_LEAD_15             ( 0x3000U )                         /**< LEAD delay : 1.5 SCK                                   */
#define SSIOF_LAG_05              ( 0x4000U )                         /**< LAG delay : 0.5 SCK                                    */
#define SSIOF_LAG_10              ( 0x8000U )                         /**< LAG delay : 1.0 SCK                                    */
#define SSIOF_LAG_15              ( 0xC000U )                         /**< LAG delay : 1.5 SCK                                    */

/*=== Internal parameters value ===*/
/* Internal parameter(internal) */
#define SSIOF_WR_FIFO_MAX             ( 4 )                                       /**< Size of FIFO (Write)             */
#define SSIOF_DUMMY_DATA              ( 0x0000 )                                  /**< dummy data                       */
#define SSIOF_RD_ERR_STATUS           ( SFnSRR_SFnORF | SFnSRR_SFnMDF )           /**< Mask of error status (for Read)  */
#define SSIOF_WR_ERR_STATUS           ( SFnSRR_SFnMDF | SFnSRR_SFnWOF )           /**< Mask of error status (for Write) */

/* Operation status(internal) */
#define SSIOF_COMMUNICATION_END       ( 0 )                           /**< Operation status : Communication completion            */
#define SSIOF_READ_DATA               ( 1 )                           /**< Operation status : Data read operation mode            */
#define SSIOF_WRITE_DATA              ( 2 )                           /**< Operation status : Data write operation mode           */
#define SSIOF_READ_WRITE_DATA         ( 3 )                           /**< Operation status : Data read / write operation mode    */


/*=== Callback function ===*/
/**
 * SSIOF callback function
 *
 * @param[in]   dataCnt     Count of writing/reading operation data                                 <br>
 *                          Unit is 8bit or 16bit.                                                  <br>
 *                          If bit-length is 8bit(SFnSIZ=0), unit is 8bit in this parameter.        <br>
 *                          If bit-length is 16bit(SFnSIZ=1), unit is 16bit in this parameter.
 *
 * @param[in]   errStatus   Status of SSIOF operation result                                        <br>
 *                          The logical sum of the following items.
 * @arg                         SFnSRR_SFnMDF
 * @arg                         SFnSRR_SFnWOF
 * @arg                         SFnSRR_SFnORF
 * @return      None
 */
typedef void (*cbfSsiof_t)( uint32_t dataCnt, uint16_t errStatus );

/*=== Strunct define ===*/
/**
 * @brief Control parameters for SSIOF
 */
typedef struct {
	uint32_t        dataSize;               /**< Size of writing/reading data                               */
	uint32_t        cnt;                    /**< Size of data which is writing/reading                      */
	uint32_t        dmyCnt;                 /**< Remaining data size for dummy transmission                 */
	cbfSsiof_t      callBack;               /**< Callback function                                          */
	void            *rxData;                /**< Pointer to area where the receive data is stored           */
	void            *txData;                /**< Pointer to area where the send data is stored              */
	uint16_t        errStat;                /**< error status (for callback)                                */
	uint8_t         status;                 /**< Read/write operation status                                */
	uint8_t         dymmy;
} ssiofCtrlParam_t;


#endif /* SSIOF_COMMON_H__ */

/** @} */
/** @} */
