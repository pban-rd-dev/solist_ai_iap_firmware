/*****************************************************************************
 can.h

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
 * @file    can.h
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

#ifndef CAN_H
#define CAN_H

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/**
 * @name Common
 *
 * @{
 */
#define LOCAL_INLINE __attribute__((always_inline)) static __inline  /**< static inline */

#define NULL_PTR ((void *)0)      /**< NULL Pointer */

#define E_OK 0x00u                /**< E_OK */
    
#define E_NOT_OK 0x01u            /**< E_NOT_OK */
/** @}*/

/**
 * @name MessageRAM related (address)
 *
 * @{
 */
#ifdef PARASOFT_CPPTEST
#define CAN_COUNT_LIMIT (0x0003U) /**< For weight time (limit) testing during processing */

#else
#define CAN_COUNT_LIMIT (0x1000U) /**< Weight time during processing (limit) */

#endif
#define CAN_TXBUFF_SIZE (0x480U)  /**< MessageRAM(TX Buffer size) */

#define CAN_RXBUFF_SIZE (0x900U)  /**< MessageRAM(RX Buffer size) */

#define CAN_RXFIFO_SIZE (0x480U)  /**< MessageRAM (RX FIFO size) */

#define CAN_SFILTER_SIZE (0x100U) /**< MessageRAM (standard filter size) */

#define CAN_EFILTER_SIZE (0x100U) /**< MessageRAM (Extended Filter Size) */

#define CAN_TXEVENT_SIZE (0x80U)  /**< MessageRAM (TX event size) */

#ifdef PARASOFT_CPPTEST
#define CAN_MESSAGE_RAM_SIZE (0x0020U) /**< MessageRAM Size for Test */

#else
#define CAN_MESSAGE_RAM_SIZE (0x1900U) /**< MessageRAM Size */

#endif

/*Message RAM Area*/
#define CAN_MESSAGE_RAM_ADDR (0x47050000U)  /**< MessageRAM start address */

#define CAN_OFFSET_SIDFC_SADDR (0x0U)       /**< Offset from MessageRAM Start Address (Standard Filter) */

#define CAN_OFFSET_XIDFC_SADDR (0x100U)     /**< Offset from MessageRAM Start Address (Extended Filter) */

#define CAN_OFFSET_XIDFC_F0_SADDR (0x100U)  /**< Offset from MessageRAM Start Address (Enhanced Filter FO) */

#define CAN_OFFSET_XIDFC_F1_SADDR (0x104U)  /**< Offset from MessageRAM start address (Advanced Filter F1) */

#define CAN_OFFSET_RXF0C_SADDR (0x200U)     /**< Offset from MessageRAM start address (FIFO0) */

#define CAN_OFFSET_RXF0C_R0_SADDR (0x200U)  /**< Offset from MessageRAM start address (FIFO0 R0) */

#define CAN_OFFSET_RXF0C_R1_SADDR (0x204U)  /**< Offset from MessageRAM start address (FIFO0 R1) */

#define CAN_OFFSET_RXF0C_SDADDR (0x208U)    /**< Offset from MessageRAM start address (FIFO0 data) */

#define CAN_OFFSET_RXF1C_SADDR (0x680U)     /**< Offset from MessageRAM start address (FIFO1) */

#define CAN_OFFSET_RXF1C_R0_SADDR (0x680U)  /**< Offset from MessageRAM start address (FIFO1 R0) */

#define CAN_OFFSET_RXF1C_R1_SADDR (0x684U)  /**< Offset from MessageRAM start address (FIFO1 R1) */

#define CAN_OFFSET_RXF1C_SDADDR (0x688U)    /**< Offset from MessageRAM start address (FIFO1 data) */

#define CAN_OFFSET_RXBC_SADDR (0xB00U)      /**< Offset from MessageRAM Start Address (BUFFER) */

#define CAN_OFFSET_RXBC_R0_SADDR (0xB00U)   /**< Offset from MessageRAM start address (BUFFER R0) */

#define CAN_OFFSET_RXBC_R1_SADDR (0xB04U)   /**< Offset from MessageRAM start address (BUFFER R1) */

#define CAN_OFFSET_RXBC_SDADDR (0xB08U)     /**< Offset from MessageRAM start address (BUFFER data) */

#define CAN_OFFSET_TXEFC_SADDR (0x1400U)    /**< Offset from MessageRAM start address (TX event) */

#define CAN_OFFSET_TXEFC_E1_SADDR (0x1404U) /**< Offset from MessageRAM start address (TX event data) */

#define CAN_OFFSET_TXBC_SADDR (0x1480U)     /**< Offset from MessageRAM start address (TX BUFFER) */

#define CAN_OFFSET_TXBC_T0_SADDR (0x1480U)  /**< Offset from MessageRAM start address (TX BUFFER T0) */

#define CAN_OFFSET_TXBC_T1_SADDR (0x1484U)  /**< Offset from MessageRAM start address (TX BUFFER T1) */

#define CAN_OFFSET_TXBC_SDADDR (0x1488U)    /**< Offset from MessageRAM start address (TX BUFFER data) */

/** @}*/

/**
 * @name MessageRAM related (TXBUFFER)
 *
 * @{
 */
/*TXbuffer element related*/
/*TXbuffer element T0 BIT*/
#define CAN_TXBUFF_ID_STANDARD (18U) /**< MessageRAM(TX BUFFER standard ID bit) */

#define CAN_TXBUFF_RTR (29U)         /**< MessageRAM(TX BUFFER Remote bit) */

#define CAN_TXBUFF_XTD (30U)         /**< MessageRAM(TX BUFFER extended ID bit) */

/*TXbuffer element T1 BIT*/
#define CAN_TXBUFF_MMH (8U)   /**< MessageRAM(TX BUFFER Message marker bit) */

#define CAN_TXBUFF_DLC (16U)  /**< MessageRAM(TX BUFFER DLC bit) */

#define CAN_TXBUFF_BRS (20U)  /**< MessageRAM(TX BUFFER Bitrate Switch bit) */

#define CAN_TXBUFF_FDF (21U)  /**< MessageRAM(TX BUFFER communication format bit) */

#define CAN_TXBUFF_TSCE (22U) /**< MessageRAM(TX BUFFER Timestamp Capture Bit) */

#define CAN_TXBUFF_EFC (23U)  /**< MessageRAM(TX BUFFER Event FIFO bit) */

#define CAN_TXBUFF_MML (24U)  /**< MessageRAM(TX BUFFER Message marker bit) */


#define CAN_TXBUFF_ID_MASK (0x1FFFFFFFU)   /**< MessageRAM(TX BUFFER ID mask) */

#define CAN_TXBUFF_RTR_MASK (0x00000001U)  /**< MessageRAM(TX BUFFER Remote mask) */

#define CAN_TXBUFF_XTD_MASK (0x00000001U)  /**< MessageRAM(TX BUFFER extended ID mask ) */

#define CAN_TXBUFF_MM_MASK (0x0000FFFFU)   /**< MessageRAM(TX BUFFER Message marker mask) */

#define CAN_TXBUFF_DLC_MASK (0x0000000FU)  /**< MessageRAM(TX BUFFER DLC mask) */

#define CAN_TXBUFF_BRS_MASK (0x00000001U)  /**< MessageRAM(TX BUFFER Bitrate Switch mask) */

#define CAN_TXBUFF_FDF_MASK (0x00000001U)  /**< MessageRAM(TX BUFFER communication format mask) */

#define CAN_TXBUFF_TSCE_MASK (0x00000001U) /**< MessageRAM(TX BUFFER Timestamp Capture Mask ) */

#define CAN_TXBUFF_EFC_MASK (0x00000001U)  /**< MessageRAM(TX BUFFER Event FIFO mask) */


#define CAN_TXBUF_RTR_DATA (0U)    /**< MessageRAM(TX BUFFER data) */

#define CAN_TXBUF_RTR_REMOTE (1U)  /**< MessageRAM(TX BUFFER remote) */

#define CAN_TXBUF_XTD_11 (0U)      /**< MessageRAM( TX BUFFER standard) */

#define CAN_TXBUF_XTD_29 (1U)      /**< MessageRAM(TX BUFFER extension) */

#define CAN_TXBUF_FDF_CLASSIC (0U) /**< MessageRAM(TX BUFFER CAN2.0B) */

#define CAN_TXBUF_FDF_FD (1U)      /**< MessageRAM(TX BUFFER CANFD) */

#define CAN_TXBUF_BRS_DIS (0U)     /**< MessageRAM (TX BUFFER Bitrate Switch Disabled) */

#define CAN_TXBUF_BRS_EN (1U)      /**< MessageRAM ( TX BUFFER Bitrate Switch Enabled) */

#define CAN_TXBUF_TSU_DIS (0U)     /**< MessageRAM (TX BUFFER timestamp capture disabled) */

#define CAN_TXBUF_TSU_EN (1U)      /**< MessageRAM ( TX BUFFER timestamp capture enabled) */

#define CAN_TXBUF_EFIFO_DIS (0U)   /**< MessageRAM(TX BUFFER Event FIFO disabled) */

#define CAN_TXBUF_EFIFO_EN (1U)    /**< MessageRAM(TX BUFFER Event FIFO enabled) */

/** @}*/

/**
 * @name MessageRAM related (RXBUFFER)
 *
 * @{
 */
/*RXbuffer element related*/
/*RXbuffer element R0 BIT*/
#define CAN_RXBUFF_SID (18U) /**< MessageRAM(RX BUFFER standard id bit) */

#define CAN_RXBUFF_RTR (29U) /**< MessageRAM(RX BUFFER Remote bit) */

#define CAN_RXBUFF_XTD (30U) /**< MessageRAM(RX BUFFER extended id bit ) */

#define CAN_RXBUFF_ESI (31U) /**< MessageRAM(RX BUFFER error condition bit) */

/*RXbuffer element R1A BIT*/
#define CAN_RXBUFF_DLC (16U)  /**< MessageRAM(RX BUFFER DLC bit) */

#define CAN_RXBUFF_BRS (20U)  /**< MessageRAM(RX BUFFER Bitrate Switch bit) */

#define CAN_RXBUFF_FDF (21U)  /**< @~japanese-en MessageRAM(RX BUFFER communication format bit) */

#define CAN_RXBUFF_FIDX (24U) /**< MessageRAM(RX BUFFER Filter index bit) */

#define CAN_RXBUFF_ANMF (31U) /**< MessageRAM (RX BUFFER Accept mismatched frame bit) */

/*RXbuffer element R1B BIT*/
#define CAN_RXBUFF_TSC (4U) /**< MessageRAM(RX BUFFER timestamp bit) */


#define CAN_RXBUFF_ID_MASK (0x1FFFFFFFU)    /**< MessageRAM(RX BUFFER ID mask) */

#define CAN_RXBUFF_RTR_MASK (0x20000000U)   /**< MessageRAM(RX BUFFER Remote mask) */

#define CAN_RXBUFF_XTD_MASK (0x40000000U)   /**< MessageRAM(RX BUFFER extended id mask ) */

#define CAN_RXBUFF_ESI_MASK (0x80000000U)   /**< MessageRAM(RX BUFFER error condition mask) */

#define CAN_RXBUFF_RXTS_MASK (0x0000FFFFU)  /**< MessageRAM(RX BUFFER received timestamp mask) */

#define CAN_RXBUFF_RXTSP_MASK (0x0000000FU) /**< MessageRAM(RX BUFFER Receive Timestamp Pointer mask) */

#define CAN_RXBUFF_TSC_MASK (0x00000010U)   /**< MessageRAM(RX BUFFER timestamp mask) */

#define CAN_RXBUFF_DLC_MASK (0x000F0000U)   /**< MessageRAM(RX BUFFER DLC mask) */

#define CAN_RXBUFF_BRS_MASK (0x00100000U)   /**< MessageRAM(RX BUFFER Bitrate Switch mask) */

#define CAN_RXBUFF_FDF_MASK (0x00200000U)   /**< MessageRAM(RX BUFFER communication format mask) */

#define CAN_RXBUFF_FIDX_MASK (0x3F000000U)  /**< MessageRAM(RX BUFFER Filter index mask) */

#define CAN_RXBUFF_ANMF_MASK (0x80000000U)  /**< MessageRAM(RX BUFFER Accept mismatched frames mask) */

/** @}*/

/**
 * @name MessageRAM related (standard filter)
 *
 * @{
 */
/*Standard Filter element related*/
/*Standard Filter element S0 BIT*/
#define CAN_SFILTER_SSYNC (15U) /**< MessageRAM(STANDARD FILTER standard synchronous bit) */

#define CAN_SFILTER_SFID1 (16U) /**< MessageRAM(STANDARD FILTER standard filter 1 bit) */

#define CAN_SFILTER_SFEC (27U)  /**< MessageRAM(STANDARD FILTER standard filter configuration bit) */

#define CAN_SFILTER_SFT (30U)   /**< MessageRAM(STANDARD FILTER standard filter type bit) */


#define CAN_SFILTER_SFID2_MASK (0x000007FFU) /**< MessageRAM(STANDARD FILTER ID2 mask) */

#define CAN_SFILTER_SSYNC_MASK (0x00000001U) /**< MessageRAM(STANDARD FILTER standard synchronous mask) */

#define CAN_SFILTER_SFID1_MASK (0x000007FFU) /**< MessageRAM(STANDARD FILTER ID 1 mask) */

#define CAN_SFILTER_SFEC_MASK (0x00000007U)  /**< MessageRAM(STANDARD FILTER standard filter configuration mask mask) */

#define CAN_SFILTER_SFT_MASK (0x00000003U)   /**< MessageRAM(STANDARD FILTER Standard filter type mask) */

/** @}*/

/**
 * @name MessageRAM related (extended filter)
 *
 * @{
 */
/*Extend Filter element related*/
/*Extend Filter element F0 BIT*/
#define CAN_EFILTER_EFEC (29U) /**< MessageRAM(EXTENDED FILTER extended filter configuration bit) */

/*Extend Filter element F1 BIT*/
#define CAN_EFILTER_ESYNC (29U) /**< MessageRAM(EXTENDED FILTER extended sync bit) */

#define CAN_EFILTER_EFT (30U)   /**< MessageRAM(EXTENDED FILTER extended filter type bit) */


#define CAN_EFILTER_EFID1_MASK (0x1FFFFFFFU) /**< MessageRAM(EXTENDED FILTER ID1 mask) */

#define CAN_EFILTER_EFEC_MASK (0x00000007U)  /**< MessageRAM(EXTENDED FILTER extended filter configuration mask) */

#define CAN_EFILTER_EFID2_MASK (0x1FFFFFFFU) /**< MessageRAM(EXTENDED FILTER extended filter ID2 bit) */

#define CAN_EFILTER_ESYNC_MASK (0x00000001U) /**< MessageRAM(EXTENDED FILTER extended sync bit) */

#define CAN_EFILTER_EFT_MASK (0x00000003U)   /**< MessageRAM(EXTENDED FILTER extended filter type bit) */

/** @}*/

/**
 * @name MessageRAM related (filter)
 *
 * @{
 */
#define CAN_FILTER_SYNC_DIS (0U)      /**< MessageRAM (filter sync stamp disabled) */

#define CAN_FILTER_SYNC_EN (1U)       /**< MessageRAM (filter sync stamp enabled) */

#define CAN_FILTER_FEC_DIS (0U)       /**< MessageRAM (filter disabled) */

#define CAN_FILTER_FEC_FIFO0 (1U)     /**< MessageRAM (stored in filter FIFO0) */

#define CAN_FILTER_FEC_FIFO1 (2U)     /**< MessageRAM (stored in filter FIFO1) */

#define CAN_FILTER_FEC_REJECT (3U)    /**< MessageRAM (Filter Reject) */

#define CAN_FILTER_FEC_PRI (4U)       /**< MessageRAM (filter priority) */

#define CAN_FILTER_FEC_PRI_FIFO0 (5U) /**< MessageRAM (Filter Priority FIFO0) */

#define CAN_FILTER_FEC_PRI_FIFO1 (6U) /**< MessageRAM (Filter Priority FIFO1) */

#define CAN_FILTER_FEC_BUF (7U)       /**< MessageRAM (Filter Priority BUFFER) */

#define CAN_FILTER_FT_SCOPE (0U)      /**< MessageRAM (filter type SCOPE) */

#define CAN_FILTER_FT_DUAL (1U)       /**< MessageRAM (filter type DUAL) */

#define CAN_FILTER_FT_CLASSIC (2U)    /**< MessageRAM (filter type CLASSIC) */

#define CAN_FILTER_FT_DIS (3U)        /**< MessageRAM (filter type disabled) */

/** @}*/

/**
 * @name MessageRAM related (Tx event)
 *
 * @{
 */
/*TxEvent FIFO element related*/
/*TxEvent FIFO element E0 BIT*/
#define CAN_TXEVENT_RTR (29U) /**< MessageRAM(TX EVENT Remote Bit) */

#define CAN_TXEVENT_XTD (30U) /**< MessageRAM(TX EVENT extended id bit ) */

#define CAN_TXEVENT_ESI (31U) /**< MessageRAM(TX EVENT Error State Bit) */

/*TxEvent FIFO element E1A BIT*/
#define CAN_TXEVENT_DLC (16U) /**< MessageRAM(TX EVENT DLC bit) */

#define CAN_TXEVENT_BRS (20U) /**< essageRAM(TX EVENT Bitrate Switch bit) */

#define CAN_TXEVENT_FDF (21U) /**< MessageRAM(TX EVENT communication format bit) */

#define CAN_TXEVENT_ET (22U)  /**< MessageRAM(TX EVENT Event Type Bit) */

#define CAN_TXEVENT_MML (24U) /**< MessageRAM(TX EVENT Message Marker Bit) */

/*TxEvent FIFO element E1B BIT*/
#define CAN_TXEVENT_TSC (4U) /**< MessageRAM(TX EVENT timestamp bit) */

#define CAN_TXEVENT_MMH (8U) /**< MessageRAM(TX EVENT Message Marker Bit) */


#define CAN_TXEVENT_ID_MASK (0x1FFFFFFFU)    /**< MessageRAM(TX EVENT ID mask) */

#define CAN_TXEVENT_RTR_MASK (0x20000000U)   /**< MessageRAM(TX EVENT Remote mask) */

#define CAN_TXEVENT_XTD_MASK (0x40000000U)   /**< MessageRAM(TX EVENT extended id mask ) */

#define CAN_TXEVENT_ESI_MASK (0x80000000U)   /**< essageRAM(TX EVENT Error State Mask) */

#define CAN_TXEVENT_TXTS_MASK (0x0000FFFFU)  /**< MessageRAM(TX EVENT Transmission Timestamp Mask) */

#define CAN_TXEVENT_DLC_MASK (0x000F0000U)   /**< MessageRAM(TX EVENT DLC mask) */

#define CAN_TXEVENT_BRS_MASK (0x00100000U)   /**< MessageRAM(TX EVENT Bitrate Mask) */

#define CAN_TXEVENT_FDF_MASK (0x00200000U)   /**< MessageRAM(TX EVENT communication format mask) */

#define CAN_TXEVENT_ET_MASK (0x00C00000U)    /**< MessageRAM(TX EVENT Event Type Mask) */

#define CAN_TXEVENT_MML_MASK (0xFF000000U)   /**< MessageRAM(TX EVENT Message Marker Mask) */

#define CAN_TXEVENT_TXTSP_MASK (0x0000000FU) /**< MessageRAM(TX EVENT timestamp pointer mask) */

#define CAN_TXEVENT_TSC_MASK (0x00000010U)   /**< MessageRAM(TX EVENT timestamp mask) */

#define CAN_TXEVENT_MMH_MASK (0x0000FF00U)   /**< MessageRAM(TX EVENT Message Marker Mask) */

/** @}*/

/**
 * @name BUFF selection
 *
 * @{
 */
#define CAN_SEL_BUFF1 (1U) /**< NDAT1 Selection */

#define CAN_SEL_BUFF2 (2U) /**< NDAT2 Selection */

/** @}*/

/**
 * @name FIFO selection
 *
 * @{
 */
#define CAN_SEL_FIFO0 (0U) /**< FIFO0 selection */

#define CAN_SEL_FIFO1 (1U) /**< FIFO1 selection */

/** @}*/

/* Status value of API */
#define CAN_STATUS_IDLE (0x0u)           /**< Internal Status IDLE */

#define CAN_STATUS_TRANSMIT_START (0x1u) /**< Internal Status Transmission Start */

#define CAN_STATUS_RECEIVE_START (0x2u)  /**< Internal Status Reception Start */


/*############################################################################*/
/*#                                 Typedef                                  #*/
/*############################################################################*/
/**
 * This type can be used as API return type 
 * It shall be defined as follows:
 * typedef uint8_t Std_ReturnType;
 */
typedef uint8_t Std_ReturnType;
    
/**
 * @brief Prototype of the function that the CAN calls back (transmission complete)
 *
 */
typedef void can_TransmitCompCallBackType(void);

/**
 * @brief Prototype of the function that the CAN calls back (Received Complete (BUFF))
 *
 */
typedef void can_ReceiveBuffCompCallBackType(void);

/**
 * @brief Prototype of the function that the CAN calls back (Received Complete (FIFO))
 *
 */
typedef void can_ReceiveFifoCompCallBackType(uint8_t sel);

/**
 * @brief Prototype of the function that the CAN calls back (bus-off)
 *
 */
typedef void can_BusOffCallBackType(void);

/**
 * @brief Prototype of the function (other) that the CAN calls back
 *
 * @note Called when bits other than TC, DRX, RF0N, RF1N, BO of CANIR stand
 */
typedef void can_OthersCallBackType(void);

/**
 * @brief CAN IRQ callback function structure
 *
 */
typedef struct
{
    /** @copybrief can_TransmitCompCallBackType */
    can_TransmitCompCallBackType *transmit;
    /** @copybrief can_ReceiveBuffCompCallBackType */
    can_ReceiveBuffCompCallBackType *receive_buff;
    /** @copybrief can_ReceiveFifoCompCallBackType */
    can_ReceiveFifoCompCallBackType *receive_fifo;
    /** @copybrief can_BusOffCallBackType */
    can_BusOffCallBackType *busoff;
    /** @copybrief can_OthersCallBackType */
    can_OthersCallBackType *others;
} can_CallBackType;

/**
 * @brief Initial settings (operating mode, bit timing, filter, buffer/FIFO configuration, interrupt settings)
 */
typedef struct
{
    uint32_t cccr;  /**< Setting value to CCCR register */

    uint32_t dbtp;  /**< Setting value to DBTP register */
 
    uint32_t nbtp;  /**< Setting value to NBTP register */

    uint32_t tdcr;  /**< Setting value to TDCR register */

    uint32_t gfc;   /**< Setting value to GFC register */

    uint32_t sidfc; /**< Setting value to SIDFC register */

    uint32_t xidfc; /**< Setting value to XIDFC register */

    uint32_t xidam; /**< Setting to the XIDAM register. */

    uint32_t rxf0c; /**< Setting value to RXF0C register */

    uint32_t rxf1c; /**< Setting value to RXF0C register */

    uint32_t rxbc;  /**< Setting value to RXBC register */

    uint32_t rxesc; /**< Setting value to RXESC register */

    uint32_t txefc; /**< Setting value to TXEFC register */

    uint32_t txbc;  /**< Setting value to TXBC register */

    uint32_t txesc; /**< Setting value to TXESC register */

    uint32_t ie;    /**< Setting value to IE register */

    uint32_t ils;   /**< Setting value to ILS register */

    uint32_t ile;   /**< Setting value to ILE register */


    can_CallBackType *cb; /**< Callback Functions */

} can_InitParamType;

/**
 * @brief TxBuffer element T0 configuration
 *
 */
typedef struct
{
    uint32_t id; /**< ID */

    uint8_t rtr; /**< remote */

    uint8_t xtd; /**< Extended Identification */

} can_TxBufferT0Type;

/**
 * @brief TxBuffer element T1 configuration
 *
 */
typedef struct
{
    uint16_t mm_h; /**< Message Markers */

    uint8_t dlc;   /**< DLC */

    uint8_t brs;   /**< Bitrate */

    uint8_t fdf;   /**< Communication format */

    uint8_t tsce;  /**< timestamp */

    uint8_t efc;   /**< Event FIFO */

    uint16_t mm_l; /**< Message Markers */

} can_TxBufferT1Type;

/**
 * @brief TxBuffer element T0/T1 configuration
 *
 */
typedef struct
{
    can_TxBufferT0Type element_t0; /**< T0 configuration */

    can_TxBufferT1Type element_t1; /**< T1 configuration */

} can_TxBufferType;

/**
 * @brief TxBuffer element information
 *
 */
typedef struct
{
    uint8_t data_size;            /**< Data Size */

    uint8_t element_num;          /**< Number of elements */

    uint8_t write_element_number; /**< Element Number */

} can_TxBuffer_InfoType;

/**
 * @brief MessageRAM reception area element R0 configuration
 *
 */
typedef struct
{
    uint32_t id; /**< ID */

    uint8_t rtr; /**< remote */

    uint8_t xtd; /**< Extended Identification */

    uint8_t esi; /**< Error condition */

} can_RxMEMR0Type;

/**
 * @brief MessageRAM reception area element R1A configuration
 *
 */
typedef struct
{
    uint16_t rxts; /**< Received timestamp */

    uint8_t dlc;   /**< @~japanese-en DLC */

    uint8_t brs;   /**< Bitrate Switch */

    uint8_t fdf;   /**< Communication format */

    uint8_t fidx;  /**< Filter Index */

    uint8_t anmf;  /**< Accepting mismatched frames */

} can_RxMEMR1AType;

/**
 * @brief MessageRAM reception area element R1B configuration
 *
 */
typedef struct
{
    uint8_t rxtsp; /**< Receive Timestamp Pointer */

    uint8_t tsc;   /**< timestamp */

    uint8_t dlc;   /**< DLC */

    uint8_t brs;   /**< Bitrate Switch */

    uint8_t fdf;   /**< Communication format */

    uint8_t fidx;  /**< Filter Index */

    uint8_t anmf;  /**< Accepting mismatched frames */

} can_RxMEMR1BType;

/**
 * @brief MessageRAM reception area element R0/R1A/R1B configuration
 *
 */
typedef struct
{
    can_RxMEMR0Type element_r0;   /**< R0 configuration */

    can_RxMEMR1AType element_r1a; /**< R1A Configuration */

    can_RxMEMR1BType element_r1b; /**< R1B Configuration */

} can_RxMEMType;

/**
 * @brief MessageRAM reception area element information
 *
 */
typedef struct
{
    uint8_t data_size;           /**< Data Size */

    uint8_t element_num;         /**< Number of elements */

    uint8_t read_element_number; /**< Element Number */

    uint8_t fifo_sel;            /**< FIFO selection */

} can_RxMEM_InfoType;

/**
 * @brief Standard Message ID Filter S0 configuration
 *
 */
typedef struct
{
    uint16_t sfid2; /**< ID2 */

    uint8_t ssync;  /**< Standard synchronization */

    uint16_t sfid1; /**< ID1 */

    uint8_t sfec;   /**< Standard filter configuration */

    uint8_t sft;    /**< Standard Filter Types */

} can_StandardFilterType;

/**
 * @brief Extended Message ID Filter F0 configuration
 *
 */
typedef struct
{
    uint32_t efid1; /**< ID1 */

    uint8_t efec;   /**< Extended filter configuration  */

} can_ExtFilterF0Type;

/**
 * @brief Standard Message ID Filter F1 configuration
 *
 */
typedef struct
{
    uint32_t efid2; /**< ID2 */

    uint8_t esync;  /**< Extended Filter Synchronization  */

    uint8_t eft;    /**< Extended Filter Type */

} can_ExtFilterF1Type;

/**
 * @brief Standard Message ID Filter F0/F1 configuration
 *
 */
typedef struct
{
    can_ExtFilterF0Type element_f0; /**< F0 configuration */

    can_ExtFilterF1Type element_f1; /**< F1 configuration */

} can_ExtFilterType;

/**
 * @brief Tx Event FIFO E0 configuration
 *
 */
typedef struct
{
    uint32_t id; /**< ID */

    uint8_t rtr; /**< remote */

    uint8_t xtd; /**< Extended Identification */

    uint8_t esi; /**< Error condition */

} can_TxEventE0Type;

/**
 * @brief Tx Event FIFO E1A Configuration
 *
 */
typedef struct
{
    uint16_t txts; /**< Transmission timestamp */

    uint8_t dlc;   /**< DLC */

    uint8_t brs;   /**< Bitrate */

    uint8_t fdf;   /**< Communication format */

    uint8_t et;    /**< Event Type */

    uint8_t mm_l;  /**< Message Markers */

} can_TxEventE1AType;

/**
 * @brief Tx Event FIFO E1B Configuration
 *
 */
typedef struct
{
    uint8_t txtsp; /**< Timestamp pointer */

    uint8_t tsc;   /**< timestamp */

    uint8_t mm_h;  /**< Message Markers */

    uint8_t dlc;   /**< DLC */

    uint8_t brs;   /**< Bitrate */

    uint8_t fdf;   /**< Communication format */

    uint8_t et;    /**< Event Type */

    uint8_t mm_l;  /**< Message Markers */

} can_TxEventE1BType;

/**
 * @brief Tx Event FIFO E0/E1A/E1B configuration
 *
 */
typedef struct
{
    can_TxEventE0Type element_e0;   /**< E0 configuration */

    can_TxEventE1AType element_e1a; /**< E1A configuration */

    can_TxEventE1BType element_e1b; /**< E1B configuration */

} can_TxEventType;

/**
 * @brief CAN Controller Parameters
 */
typedef struct
{
    uint8_t *tx_data;      /**< Pointer to area where the send data is stored */

    uint32_t tx_data_size; /**< Size of send data */

    uint32_t tx_cnt;       /**< Size of data which is sent */

    uint32_t tx_status;    /**< Send status */

    uint8_t *rx_data;      /**< Pointer to area where the receive data is stored */

    uint32_t rx_data_size; /**< Size of receive data */

    uint32_t rx_cnt;       /**< Size of data which is received */

    uint32_t rx_status;    /**< Receive status */
 
} can_CtrlParamType;

/**
 * @brief Tx Event FIFO E0/E1A/E1B configuration
 *
 */
typedef struct
{
    uint8_t lbck;  /**< Loopback mode */

    uint8_t tx;    /**< CAN0_TXD terminal control */

    uint8_t rx;    /**< CAN0_RXD terminal */

    uint8_t txbnp; /**< Transmission ready Tx buffer number  */

    uint8_t pval;  /**< TXBNP value enabled/disabled */

    uint8_t txbns; /**< Transmission Start Tx Buffer Number */

    uint8_t sval;  /**< TXBNS value enabled/disabled */

} can_TestType;

/**
 * @brief CAN register readback structure
 *
 */
typedef struct
{
    uint32_t CANCCCR; /**< CANCCCR Register */

    uint32_t CANNBTP; /**< CANNBTP Register */

} can_RbRegisterType;

/**
 * @brief Register Readback
 * 
 * @param[out] *rbReg @ref can_RbRegisterType
 * 
 * @retval E_OK normal
 * 
 * @retval E_NOT_OK abnormal
 * 
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @par Description of Operation
 * @endif
 */
LOCAL_INLINE Std_ReturnType can_ReadbackRegister(can_RbRegisterType *rbReg)
{
    Std_ReturnType Ret = E_NOT_OK;
    if(rbReg != NULL_PTR)
    {
        rbReg->CANCCCR = read_reg32(CAN0->CANCCCR);
        rbReg->CANNBTP = read_reg32(CAN0->CANNBTP);

        Ret = E_OK;
    }
    return Ret;
}

Std_ReturnType can_Reset(void);
Std_ReturnType can_Init(can_InitParamType *Param);
void           can_InitCanRAM(void);
void           can_SetBitTiming(uint32_t dbtp, uint32_t nbtp, uint32_t tdcr);
Std_ReturnType can_SetStandardFilter(can_StandardFilterType *element, uint8_t element_number);
Std_ReturnType can_SetExtFilter(can_ExtFilterType *element, uint8_t element_number);
Std_ReturnType can_SetNormalMode(void);
Std_ReturnType can_SetSleepMode(void);
void           can_ClearClockStopRequest(void);
void           can_CancelTransmit(uint8_t element_number);
uint32_t       can_CheckPendingTransmit(void);
Std_ReturnType can_PrepareTransmit(can_TxBufferType      *element,
                                   can_TxBuffer_InfoType *info,
                                   uint8_t                   data[]);
void           can_RequestTransmit(uint8_t element_number);
void           can_ReceiveBuffData(can_RxMEMType      *element,
                                   can_RxMEM_InfoType *info,
                                   uint32_t                  data[]);
void           can_ReceiveFifoData(can_RxMEMType      *element,
                                   can_RxMEM_InfoType *info,
                                   uint32_t                  data[]);
uint32_t       can_CheckReceiveBuffData(uint8_t buf_sel);
uint8_t        can_CheckReceiveFifoData(uint8_t fifo_sel);
Std_ReturnType can_GetTxEvent(can_TxEventType *element, uint8_t element_number);
void           can_SetTestMode(uint32_t lbck, uint32_t tx);
void           can_GetTestMode(can_TestType *test);
void           can_SetIrq(uint32_t ie);
uint32_t       can_GetPSR(void);
uint32_t       can_GetECR(void);
uint32_t       can_GetStatus(void);
void           can_ClearStatus(uint32_t canir);
uint32_t       can_GetTxEventStatus(void);
uint32_t       can_GetTxFifoQueueStatus(void);
void           CAN0_INT1_IRQHandler(void);
void           CAN0_INT0_IRQHandler(void);
void           Can_IRQHandler(void);

#endif /* CAN_H */

/** @} */
/** @} */
