/*****************************************************************************
 dmac.h

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
 * @file    dmac.h
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

#ifndef DMAC_H_
#define DMAC_H_

#include "mcu.h"
#include "rdwr_reg.h"
#include "dmac_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Clear transfer channel fix
 *
 * @param       -
 * @return      None
 */
#define         dmac_setChFix()           clear_bit( DMAC->DMAMOD , (0x00000001UL) )

/**
 * Set transfer channel fix
 *
 * @param       -
 * @return      None
 */
#define         dmac_setChRound()         set_bit( DMAC->DMAMOD , (0x00000001UL)  )


/**
 * dmac0_checkEnd  
 *
 * @param       -
 * @retval      0                STA_NOUNTRANS
 * @retval      "other than 0"   STA_UNTRANS
 */
#define         dmac0_checkEnd()        get_bit( DMAC->DMASTA , (0x00000001UL)  )

/**
 * Set transfer channel fix  
 *
 * @param       -
 * @retval      0                STA_NOUNTRANS
 * @retval      "other than 0"   STA_UNTRANS
 */
#define         dmac1_checkEnd()        get_bit( DMAC->DMASTA , (0x00000002UL)  )


/**
 * Set transfer channel fix  
 *
 * @param       -
 * @retval      0                IREQ_GEN 
 * @retval      "other than 0"   IREQ_NOGEN
 */
#define         dmac0_checkIREQ()       get_bit( DMAC->DMAINT , (0x00000001UL)  )

/**
 * Set transfer channel fix  
 *
 * @param       -
 * @retval      0                IREQ_GEN 
 * @retval      "other than 0"   IREQ_NOGEN
 */
#define         dmac1_checkIREQ()       get_bit( DMAC->DMAINT , (0x00000002UL)  )

/**
 * Set transfer channel fix 
 *
 * @param       -
 * @retval      0                ISTA_NOMA
 * @retval      "other than 0"   ISTA_ABNOMAL
 */
#define         dmac0_checkISTA()       get_bit( DMAC->DMAINT , (0x00000100UL)  )

/**
 * Set transfer channel fix 
 *
 * @param       -
 * @retval      0                ISTA_NOMAL
 * @retval      "other than 0"   ISTA_ABNOMAL
 */
#define         dmac1_checkISTA()       get_bit( DMAC->DMAINT , (0x00000200UL)  )

/**
 * Set transfer channel fix
 *
 * @param       -
 * @retval      0                IISTP_READ
 * @retval      "other than 0"   ISTP_WRITE
 */
#define         dmac0_checkISTP()       get_bit( DMAC->DMAINT , (0x00010000UL)  )

/**
 * Set transfer channel fix
 *
 * @param       -
 * @retval      0                IISTP_READ
 * @retval      "other than 0"   ISTP_WRITE
 */
#define         dmac1_checkISTP()       get_bit( DMAC->DMAINT , (0x00020000UL)  )

/**
 * Set transfer channel fix  
 *
 * @param       -
 * @retval      DMAINT registor read value
 */
#define         dmac_checkIntStatus()       read_reg32( DMAC->DMAINT )


/**
 * Set transfer channel fix 
 *
 *  @param[in]   dreq_sel dma external request setting
 *                               - DMAC_REQ_SSIOF0_RX  
 *                               - DMAC_REQ_SSIOF0_TX  
 *                               - DMAC_REQ_SSIOF1_RX  
 *                               - DMAC_REQ_SSIOF1_TX  
 *                               - DMAC_REQ_UARTF1_RX  
 *                               - DMAC_REQ_UARTF1_TX  
 *                               - DMAC_REQ_UARTF0_RX  
 *                               - DMAC_REQ_UARTF0_TX  
 *                               - DMAC_REQ_I2CF_RX    
 *                               - DMAC_REQ_I2CF_TX    
 *                               - DMAC_REQ_SAADC1     
 *                               - DMAC_REQ_SAADC0     
 *                               - DMAC_REQ_UARTF2_RX  
 *                               - DMAC_REQ_UARTF2_TX  
 *                               - DMAC_REQ_UARTF3_RX  
 *                               - DMAC_REQ_UARTF3_TX  
 *                               - DMAC_REQ_NOTSEL     
 * @return      None
 */
#define         dmac0_dreqsel( dreq_sel )       write_reg32( LSICNT->DREQSEL , ((read_reg32( LSICNT->DREQSEL ) & 0xFFFFFFF0) |  (uint32_t)dreq_sel) )

/**
 * Set transfer channel fix 
 *
 *  @param[in]   dreq_sel dma external request setting
 *                               - DMAC_REQ_SSIOF0_RX  
 *                               - DMAC_REQ_SSIOF0_TX  
 *                               - DMAC_REQ_SSIOF1_RX  
 *                               - DMAC_REQ_SSIOF1_TX  
 *                               - DMAC_REQ_UARTF1_RX  
 *                               - DMAC_REQ_UARTF1_TX  
 *                               - DMAC_REQ_UARTF0_RX  
 *                               - DMAC_REQ_UARTF0_TX  
 *                               - DMAC_REQ_I2CF_RX    
 *                               - DMAC_REQ_I2CF_TX    
 *                               - DMAC_REQ_SAADC1     
 *                               - DMAC_REQ_SAADC0     
 *                               - DMAC_REQ_UARTF2_RX  
 *                               - DMAC_REQ_UARTF2_TX  
 *                               - DMAC_REQ_UARTF3_RX  
 *                               - DMAC_REQ_UARTF3_TX  
 *                               - DMAC_REQ_NOTSEL     
 * @return      None
 */
#define         dmac1_dreqsel( dreq_sel )       write_reg32( LSICNT->DREQSEL , ((read_reg32( LSICNT->DREQSEL ) & 0xFFFFF0FF) |  (uint32_t)(dreq_sel << 8)) )



#endif /*DMAC_H_*/

/** @} */
/** @} */
