/*****************************************************************************
 can_reg.h

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
 * @file    can_reg.h
 *
 * This module is 'CAN' drivers.
 *
 */
 /**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup CAN
 *  @{
 *
 *  @brief CAN FD Controller register related
 *
 *  @file
 */

#ifndef CAN_REG_H
#define CAN_REG_H

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/**
 * @name DBTP register related
 *
 * @{
 */
/*DBTP register*/
#define CAN_DBTP_TDC_DIS (0U << 23U) /**< TDC disabled */

#define CAN_DBTP_TDC_EN (1U << 23U)  /**< TDC enabled */

/*DBTP register BIT*/
#define CAN_DBTP_DSJW (0U)   /**< DSJW bit */

#define CAN_DBTP_DTSEG2 (4U) /**< DTSEG2 bit */

#define CAN_DBTP_DTSEG1 (8U) /**< DTSEG1 bit */

#define CAN_DBTP_DBRP (16U)  /**< DBRP bit */

#define CAN_DBTP_TDC (23U)   /**< TDC Bit */

/*DBTP register mask*/
#define CAN_DBTP_MASK (0x009F1FFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name CCCR register related
 *
 * @{
 */
/*CCCR register*/
#define CAN_CCCR_INIT (1U << 0U)  /**< initialization */

#define CAN_CCCR_CCE (1U << 1U)   /**< Write Protect Enabled */

#define CAN_CCCR_ASM (1U << 2U)   /**< Limited operating mode */

#define CAN_CCCR_CSA (1U << 3U)   /**< Power Down */

#define CAN_CCCR_CSR (1U << 4U)   /**< Stop Clock Request */

#define CAN_CCCR_MON (1U << 5U)   /**< Bus monitoring mode enabled */

#define CAN_CCCR_DAR (1U << 6U)   /**< Auto-retransmission enabled */

#define CAN_CCCR_TEST (1U << 7U)  /**< Test Mode Enabled */

#define CAN_CCCR_FODE (1U << 8U)  /**< @~japanese-en CAN FD */

#define CAN_CCCR_BRSE (1U << 9U)  /**< Bitrate switch enabled */

#define CAN_CCCR_WMM (1U << 11U)  /**< 16bit MessageMaker */

#define CAN_CCCR_PXHD (1U << 12U) /**< Protocol exception handling disabled */

#define CAN_CCCR_EFBI (1U << 13U) /**< Hard and synchronous detection */

#define CAN_CCCR_TXP (1U << 14U)  /**< Transmission pause enabled */

#define CAN_CCCR_NISO (1U << 15U) /**< Frame format Boash CANFD */

/*CCCR register BIT*/
#define CAN_CCCR_MON_BIT (5U)   /**< MON Bit */

#define CAN_CCCR_FDOE_BIT (8U)  /**< FODE bit */

#define CAN_CCCR_PHXD_BIT (12U) /**< PXHD bit */

/*CCCR register mask*/
#define CAN_CCCR_MASK (0x0000FFFCU)     /**< Enabled (init,cccr) bitmask  */

#define CAN_CCCR_CSR_MASK (0x0000FFEFU) /**< Enabled (CSR) Bitmask */


/** @}*/

/**
 * @name TSCC register related
 *
 * @{
 */
/*TSCC register*/
#define CAN_TSCC_TSS_0 (0U << 0U)        /**< Timestamp selection Always 0 */

#define CAN_TSCC_TSS_TCP (1U << 0U)      /**< Timestamp selection using TCP */

#define CAN_TSCC_TSS_EXTSTAMP (2U << 0U) /**< Timestamp selection Using External Counters */

/*TSCC register BIT*/
#define CAN_TSCC_TCP (16U) /**< TCP Bit */

/** @}*/

/**
 * @name GFC register related
 *
 * @{
 */
/*GFC register*/
#define CAN_GFC_RRFE_FILTER (0U << 0U) /**< Remote Extended Frame Filtering */

#define CAN_GFC_RRFE_REJECT (1U << 0U) /**< Remote Extended Frame Reject */

#define CAN_GFC_RRFS_FILTER (0U << 1U) /**< Remote Standard Frame Filtering */

#define CAN_GFC_RRFS_REJECT (1U << 1U) /**< Remote Standard Frame Reject */

#define CAN_GFC_ANFE_FIFO0 (0U << 2U)  /**< Unmatched expansion frames accepted by FIFO0 */

#define CAN_GFC_ANFE_FIFO1 (1U << 2U)  /**< Unmatched expansion frame accepted by FIFO1 */

#define CAN_GFC_ANFE_REJECT (2U << 2U) /**< Unmatched extended frame rejection */

#define CAN_GFC_ANFS_FIFO0 (0U << 4U)  /**< Unmatched standard frames accepted with FIFO0 */

#define CAN_GFC_ANFS_FIFO1 (1U << 4U)  /**< Unmatched standard frames accepted by FIFO1 */

#define CAN_GFC_ANFS_REJECT (2U << 4U) /**< Unmatched extended frame rejection */

/*GFC register mask*/
#define CAN_GFC_MASK (0x0000003FU) /**< Effective Bitmask */

/** @}*/

/**
 * @name RXF0C register related
 *
 * @{
 */
/*RXF0C register*/
#define CAN_RXF0C_F0OM_BLOCK (0U << 31U) /**< Blocking Mode */

#define CAN_RXF0C_F0OM_OVER (1U << 31U)  /**< Overwrite Mode */

/*RXF0C register BIT*/
#define CAN_RXF0C_F0S (16U)  /**< F0S bit */

#define CAN_RXF0C_F0WM (24U) /**< F0WM bit */

/*RXF0C register mask*/
#define CAN_RXF0C_MASK (0xFF7FFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name RXF1C register related
 *
 * @{
 */
/*RXF1C register*/
#define CAN_RXF1C_F1OM_BLOCK (0U << 31U) /**< Blocking Mode */

#define CAN_RXF1C_F1OM_OVER (1U << 31U)  /**< Overwrite Mode */

/*RXF1C register BIT*/
#define CAN_RXF1C_F1S (16U)  /**< F1S bit */

#define CAN_RXF1C_F1WM (24U) /**< F1WM bit */

/*RXF1C register mask*/
#define CAN_RXF1C_MASK (0xFF7FFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name RXESC register related
 *
 * @{
 */
/*RXESC register*/
#define CAN_RXESC_F0DS_8 (0U << 0U)  /**< FIFO0 8byte */

#define CAN_RXESC_F0DS_12 (1U << 0U) /**< FIFO0 12byte */

#define CAN_RXESC_F0DS_16 (2U << 0U) /**< FIFO0 16byte */

#define CAN_RXESC_F0DS_20 (3U << 0U) /**< FIFO0 20byte */

#define CAN_RXESC_F0DS_24 (4U << 0U) /**< FIFO0 24byte */

#define CAN_RXESC_F0DS_32 (5U << 0U) /**< FIFO0 32byte */

#define CAN_RXESC_F0DS_48 (6U << 0U) /**< FIFO0 48byte */

#define CAN_RXESC_F0DS_64 (7U << 0U) /**< FIFO0 64byte */

#define CAN_RXESC_F1DS_8 (0U << 4U)  /**< FIFO1 8byte */

#define CAN_RXESC_F1DS_12 (1U << 4U) /**< FIFO1 12byte */

#define CAN_RXESC_F1DS_16 (2U << 4U) /**< FIFO1 16byte */

#define CAN_RXESC_F1DS_20 (3U << 4U) /**< FIFO1 20byte */

#define CAN_RXESC_F1DS_24 (4U << 4U) /**< FIFO1 24byte */

#define CAN_RXESC_F1DS_32 (5U << 4U) /**< FIFO1 32byte */

#define CAN_RXESC_F1DS_48 (6U << 4U) /**< FIFO1 48byte */

#define CAN_RXESC_F1DS_64 (7U << 4U) /**< FIFO1 64byte */

#define CAN_RXESC_RBDS_8 (0U << 8U)  /**< BUFFER 8byte */

#define CAN_RXESC_RBDS_12 (1U << 8U) /**< BUFFER 12byte */

#define CAN_RXESC_RBDS_16 (2U << 8U) /**< BUFFER 16byte */

#define CAN_RXESC_RBDS_20 (3U << 8U) /**< BUFFER 20byte */

#define CAN_RXESC_RBDS_24 (4U << 8U) /**< BUFFER 24byte */

#define CAN_RXESC_RBDS_32 (5U << 8U) /**< BUFFER 32byte */

#define CAN_RXESC_RBDS_48 (6U << 8U) /**< BUFFER 48byte */

#define CAN_RXESC_RBDS_64 (7U << 8U) /**< BUFFER 64byte */

/*RXESC register BIT*/
#define CAN_RXESC_F0DS (0U) /**< F0DS bit */

#define CAN_RXESC_F1DS (4U) /**< F1DS bit */

#define CAN_RXESC_RBDS (8U) /**< RBDS Bit */

/*RXESC register mask*/
#define CAN_RXESC_MASK (0x0000F777U) /**< Effective Bitmask */

/** @}*/

/**
 * @name TXBC register related
 *
 * @{
 */
/*TXBC register*/
#define CAN_TXBC_TFQM_FIFO (0U << 30U)  /**< FIFO Mode */

#define CAN_TXBC_TFQM_QUEUE (1U << 30U) /**< QUEUE mode */

/*TXBC register BIT*/
#define CAN_TXBC_NDTB (16U) /**< NDTB Bit */

#define CAN_TXBC_TFQS (24U) /**< TFQS */

/*TXBC register mask*/
#define CAN_TXBC_MASK (0x7F3FFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name PSR register related
 *
 * @{
 */
/*PSR register*/
#define CAN_PSR_EP (1U << 5U)    /**< Error Passive */

#define CAN_PSR_EW (1U << 6U)    /**< Warning Status */

#define CAN_PSR_BO (1U << 7U)    /**< Bus Off */

#define CAN_PSR_RESI (1U << 11U) /**< ESI Flag */

#define CAN_PSR_RBRS (1U << 12U) /**< BRS Flag */

#define CAN_PSR_RFDF (1U << 13U) /**< CAN FD message reception */

#define CAN_PSR_PXE (1U << 14U)  /**< Protocol exception events */

/*PSR register BIT*/
#define CAN_PSR_LEC (0U)   /**< LEC Bit */

#define CAN_PSR_ACT (3U)   /**< ACT */

#define CAN_PSR_DLEC (8U)  /**< DLEC */

#define CAN_PSR_TDCV (16U) /**< TDCV */

/*PSR register mask*/
#define CAN_PSR_MASK (0x007FFFFFU) /**< Effective Bitmask */


/** @}*/

/**
 * @name TXESC register related
 *
 * @{
 */
/*TXESC register*/
#define CAN_TXESC_TBDS_8 (0U << 0U)  /**< 8byte */

#define CAN_TXESC_TBDS_12 (1U << 0U) /**< 12byte */

#define CAN_TXESC_TBDS_16 (2U << 0U) /**< 16byte */

#define CAN_TXESC_TBDS_20 (3U << 0U) /**< 20byte */

#define CAN_TXESC_TBDS_24 (4U << 0U) /**< 24byte */

#define CAN_TXESC_TBDS_32 (5U << 0U) /**< 32byte */

#define CAN_TXESC_TBDS_48 (6U << 0U) /**< 48byte */

#define CAN_TXESC_TBDS_64 (7U << 0U) /**< 64byte */

/*TXESC register mask*/
#define CAN_TXESC_MASK (0x00000007U) /**< Effective Bitmask */

/** @}*/

/**
 * @name IR register related
 *
 * @{
 */
/*IR register*/
#define CAN_IR_RF0N (1U << 0U)   /**< RF0N bit */

#define CAN_IR_RF0W (1U << 1U)   /**< RF0W bit */

#define CAN_IR_RF0F (1U << 2U)   /**< RF0F bit */

#define CAN_IR_RF0L (1U << 3U)   /**< RF0L bit */

#define CAN_IR_RF1N (1U << 4U)   /**< RF1N bit */

#define CAN_IR_RF1W (1U << 5U)   /**< RF1W bit */

#define CAN_IR_RF1F (1U << 6U)   /**< RF1F bit */

#define CAN_IR_RF1L (1U << 7U)   /**< RF1L bit */
 
#define CAN_IR_HPM (1U << 8U)    /**< HPM bit */

#define CAN_IR_TC (1U << 9U)     /**< TC bit */

#define CAN_IR_TCF (1U << 10U)   /**< TCF bit */

#define CAN_IR_TFE (1U << 11U)   /**< TFE bit */

#define CAN_IR_TEFN (1U << 12U)  /**< TEFN bit */

#define CAN_IR_TEFW (1U << 13U)  /**< TEFW bit */

#define CAN_IR_TEFF (1U << 14U)  /**< TEFF bit */

#define CAN_IR_TEFL (1U << 15U)  /**< TEFL bit */

#define CAN_IR_TSW (1U << 16U)   /**< TSW bit */

#define CAN_IR_MRAF (1U << 17U)  /**< MRAF bit */

#define CAN_IR_TDO (1U << 18U)   /**< TDO bit */

#define CAN_IR_DRX (1U << 19U)   /**< DRX bit */

#define CAN_IR_BEC (1U << 20U)   /**< BEC bit */

#define CAN_IR_BEU (1U << 21U)   /**< BEU bit */

#define CAN_IR_ELO (1U << 22U)   /**< ELO bit */

#define CAN_IR_EP (1U << 23U)    /**< EP bit */

#define CAN_IR_EW (1U << 24U)    /**< EW bit */
 
#define CAN_IR_BO (1U << 25U)    /**< BO bit */

#define CAN_IR_WDI (1U << 26U)   /**< WDI bit */

#define CAN_IR_PEA (1U << 27U)   /**< PEA bit */

#define CAN_IR_PED (1U << 28U)   /**< PED bit */

#define CAN_IR_ARA (1U << 29U)   /**< ARA bit */

#define CAN_IR_ALL (0xFFFFFFFFU) /**< ALL bit */

/*IE register mask*/
#define CAN_IE_MASK (0x3FFFFFFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name ILE register related
 *
 * @{
 */
/*ILE register*/
#define CAN_ILE_EINT0_DIS (0U << 0U) /**< EINT0 disabled */

#define CAN_ILE_EINT0_EN (1U << 0U)  /**< EINT0 enabled */

#define CAN_ILE_EINT1_DIS (0U << 1U) /**< EINT1 disabled */

#define CAN_ILE_EINT1_EN (1U << 1U)  /**< EINT1 enabled */

/*ILE register mask*/
#define CAN_ILE_MASK (0x00000003U) /**< Effective Bitmask */

/** @}*/

/**
 * @name NBTP register related
 *
 * @{
 */
/*NBTP register BIT*/
#define CAN_NBTP_NTSEG2 (0U) /**< NTSEG2 bit */

#define CAN_NBTP_NTSEG1 (8U) /**< NTSEG1 bit */

#define CAN_NBTP_NBRP (16U)  /**< NBRP bit */

#define CAN_NBTP_NSJW (25U)  /**< NSJW bit */

/*NBTP register mask*/
#define CAN_NBTP_MASK (0xFFFFFF7FU) /**< Effective Bitmask */

/** @}*/

/**
 * @name TDCR register related
 *
 * @{
 */
/*TDCR register BIT*/
#define CAN_TDCR_TDCF (0U) /**< TDCR bit */

#define CAN_TDCR_TDCO (8U) /**< TDCO bit */

/*TDCR register mask*/
#define CAN_TDCR_MASK (0x00007F7FU) /**< Effective Bitmask */

/** @}*/

/**
 * @name ECR register related
 *
 * @{
 */
/*ECR register */
#define CAN_ECR_RP (1U << 15U) /**< RP bit */

/*ECR register BIT*/
#define CAN_ECR_TEC (0U)  /**< TEC bit */

#define CAN_ECR_REC (8U)  /**< REC bit */

#define CAN_ECR_CEL (16U) /**< CEL bit */

/*ECR register mask*/
#define CAN_ECR_ECMASK (0x00FFFFFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name SIDFC register related
 *
 * @{
 */
/*SIDFC register BIT*/
#define CAN_SIDFC_LSS (16U) /**< LSS bit */

/*SIDFC register mask*/
#define CAN_SIDFC_MASK (0x00FFFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name XIDAM register related
 *
 * @{
 */
/*XIDAM register mask*/
#define CAN_XIDAM_MASK (0x1FFFFFFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name XIDFC register related
 *
 * @{
 */
/*SIDFC register BIT*/
#define CAN_XIDFC_LSE (16U) /**< LSE bit */

/*SIDFC register mask*/
#define CAN_XIDFC_MASK (0x007FFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name RXF0S register related
 *
 * @{
 */
/*RXF0S register BIT*/
#define CAN_RXF0S_F0GI (8U)  /**< F0GI bit */

#define CAN_RXF0S_F0PI (16U) /**< F0PI bit */

#define CAN_RXF0S_F0F (24U)  /**< F0F bit */

#define CAN_RXF0S_RF0L (25U) /**< RF0L bit */

/** @}*/

/**
 * @name RXF1S register related
 *
 * @{
 */
/*RXF1S register BIT*/
#define CAN_RXF1S_F1GI (8U)  /**< F1GI bit */

#define CAN_RXF1S_F1PI (16U) /**< F1PI bit */

#define CAN_RXF1S_F1F (24U)  /**< F1F bit */

#define CAN_RXF1S_RF1L (25U) /**< RF1L bit */

#define CAN_RXF1S_DMS (30U)  /**< DMS bit */

/** @}*/

/**
 * @name TXEFC register related
 *
 * @{
 */
/*TXEFC register BIT*/
#define CAN_TXEFC_EFS (16U)  /**< EFS bit */

#define CAN_TXEFC_EFWM (24U) /**< EFWM bit */

/*TXEFC register mask*/
#define CAN_TXEFC_MASK (0x3F3FFFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name TXEFS register related
 *
 * @{
 */
/*TXEFS register BIT*/
#define CAN_TXEFS_EFGI (8U)  /**< EFGI bit */

#define CAN_TXEFS_EFPI (16U) /**< EFPI bit */

#define CAN_TXEFS_EFF (24U)  /**< EFF bit */

#define CAN_TXEFS_TEFL (25U) /**< TEFL bit */

/** @}*/

/**
 * @name XIDAM register related
 *
 * @{
 */
/*XIDAM register mask*/
#define CAN_XIDAM_MASK (0x1FFFFFFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name RXBC register related
 *
 * @{
 */
/*RXBC register mask*/
#define CAN_RXBC_MASK (0x0000FFFCU) /**< Effective Bitmask */

/** @}*/

/**
 * @name ILS Register Related
 * @~
 *
 * @{
 */
/*ILS register mask*/
#define CAN_ILS_MASK (0x3FFFFFFFU) /**< Effective Bitmask */

/** @}*/

/**
 * @name CANRXFnS register related
 * @anchor CAN_RXFS_X
 * @{
 */
#define CAN_RXFS_FFL_MASK (0x0000007FU) /**< FnFL (Rx FIFO 1 Fill Level) bitmask */

#define CAN_RXFS_FGI_MASK (0x00003F00U) /**< @~japanese-en FnGI (Rx FIFO 1 Get Index) bitmask */

#define CAN_RXFS_FPI_MASK (0x003F0000U) /**< @~japanese-en FnPI (Rx FIFO 1 Put Index) bitmask */

#define CAN_RXFS_FF_MASK (0x01000000U)  /**< @~japanese-en FnF (Rx FIFO 1 Full) bitmask */

#define CAN_RXFS_RFL_MASK (0x02000000U) /**< RFnL (Rx FIFO 1 Message Lost) bitmask */

#define CAN_RXFS_DMS_MASK (0xC0000000U) /**< DMS (Debug Message Status) bitmask */

/** @}*/

/**
 * @name Test registers
 *
 * @{
 */
/*LBCK register BIT*/
#define CAN_TEST_LBCK_MASK (1U << 4U) /**< LBCK Effective Bitmask */

#define CAN_TEST_LBCK_ENA (1U)        /**< Loopback mode enabled */

#define CAN_TEST_LBCK_DIS (0U)        /**< Loopback mode disabled */

/*TX register BIT*/
#define CAN_TEST_TX_MASK (3U << 5U) /**< TX Effective Bitmask */

#define CAN_TEST_TX_CAN (0U)        /**< Controlled by CAN0_TXD-terminal CAN core */

#define CAN_TEST_TX_SAMPLE (1U)     /**< CAN0_TXD terminals monitor sample points */

#define CAN_TEST_TX_DOMINANT (2U)   /**< Dominant level output at CAN0_TXD terminal */

#define CAN_TEST_TX_RECESSIVE (3U)  /**< Recessive level output at CAN0_TXD terminal */

/*RX register BIT*/
#define CAN_TEST_RX_MASK (1U << 7U) /**< RX Effective Bitmask */

#define CAN_TEST_RX_DOMINANT (0U)   /**< CAN bus is dominant level */

#define CAN_TEST_RX_RECESSIVE (1U)  /**< CAN bus is recessive-level */

/*TXBNP register BIT*/
#define CAN_TEST_TXBNP_MASK (0x1FU << 8U) /**< TXBNP Effective Bitmask */

/*PVAL register BIT*/
#define CAN_TEST_PVAL_MASK (1U << 13U) /**< PVAL Effective Bitmask */

/*TXBNP register BIT*/
#define CAN_TEST_TXBNS_MASK (0x1FU << 16U) /**< TXBNS Effective Bitmask */

/*SVAL register BIT*/
#define CAN_TEST_SVAL_MASK (1U << 21U) /**< SVAL Effective Bitmask */

/** @}*/

/**
 * @name CANTXEFS register related
 * @anchor CAN_TXEFS_X
 * @{
 */
/*CANTXEFS register BIT*/
#define CAN_TXEFS_EFF (24U)  /**< EFF bit */

#define CAN_TXEFS_TEFL (25U) /**< TEFL bit */

/*CANTXEFS register mask*/
#define CAN_TXEFS_EFFL_MASK (0x0000003FU) /**< EFFL Effective Bitmask */

#define CAN_TXEFS_EFGI_MASK (0x00001F00U) /**< EFGI Effective Bitmask */

#define CAN_TXEFS_EFPI_MASK (0x001F0000U) /**< EFPI Effective Bitmask */

/** @}*/

/**
 * @name CANTXFQS register related
 * @anchor CAN_TXFQS_X
 * @{
 */
/*CANTXFQS register BIT*/
#define CAN_TXFQS_TFQF (21U) /**< TFQF bit */

/*CANTXFQS register mask*/
#define CAN_TXFQS_TFFL_MASK (0x0000003FU)  /**< TFFL Effective Bitmask */

#define CAN_TXFQS_TFGI_MASK (0x00001F00U)  /**< TFGI Effective Bitmask */

#define CAN_TXFQS_TFQPI_MASK (0x001F0000U) /**< TFQPI Effective Bitmask */

/** @}*/

#endif /* CAN_REG_H */

/** @} */
/** @} */
