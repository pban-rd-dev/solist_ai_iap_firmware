/*****************************************************************************
 dmac1.c

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
 * @file    dmac1.c
 *
 * This module is 'DMAC1' drivers.
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "dmac.h"
#include "dmac1.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
static dmacCtrlParam_t  st_ctrlParam;         /**< control parameter for Dmac */

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Initialize DMAC1
 * @param[in]    tmod dma trance setting 
 *                               - DMAC_TMOD_ARQ_EXT      
 *                               - DMAC_TMOD_ARQ_AUTO     
 *                               - DMAC_TMOD_TSIZ_BYTE    
 *                               - DMAC_TMOD_TSIZ_HWORD   
 *                               - DMAC_TMOD_TSIZ_WORD    
 *                               - DMAC_TMOD_SDP_FIX      
 *                               - DMAC_TMOD_SDP_INC      
 *                               - DMAC_TMOD_DDP_FIX      
 *                               - DMAC_TMOD_DDP_INC      
 *                               - DMAC_TMOD_BRQ_BURST    
 *                               - DMAC_TMOD_BRQ_CYCLE    
 *                               - DMAC_TMOD_IMK_IMASK_REL
 *                               - DMAC_TMOD_IMK_IMASK
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
 *  @param[in]   func pointer of callback function
 * @return      None
*/
void dmac1_init( uint32_t tmod , uint8_t dreq_sel , cbfDmac_t func)
{

    /* stop dmac */
    dmac1_disable();

    /* clear status */
    dmac1_clrIntStatus();

    /* init control parameter */
    st_ctrlParam.callback = func;
	
    /* DMA request select registert setting */
	dmac1_dreqsel (dreq_sel);

    /*=== Register setting ===*/
    write_reg32( DMAC1->DMACTMOD1, tmod );

}


/**
 * transfer DMAC1
 *
 * @param[in]   *dma_sad  DMAC1 transfer source address 
 * @param[in]   *dma_dad  DMAC1 transfer destination address  
 * @param[in]   dma_size  DMAC1 transfer count
 * @return      None
 */
void dmac1_Transfer (void *dma_sad , void *dma_dad , uint32_t dma_size )
{
	
    /*=== Register setting ===*/
    write_reg32( DMAC1->DMACSAD1 , dma_sad );
    write_reg32( DMAC1->DMACDAD1 , dma_dad );
    write_reg32( DMAC1->DMACSIZ1 , dma_size );
	
}


/**
 *  End of Transfer 
 *
 * @param       -
 * @return      None
 */
void dmac1_endTrans(void) 
{
	
    dmacStatus_t cbStatus;

    /* stop dmac */
    dmac1_disable();

    cbStatus.status   = dmac1_checkEnd();
    cbStatus.ireq   = dmac1_checkIREQ();
    cbStatus.ista   = dmac1_checkISTA();
    cbStatus.istp   = dmac1_checkISTP();
	

    /* clear status */
	dmac1_clrIntStatus();

    if( st_ctrlParam.callback != (void *)0 ){
        st_ctrlParam.callback( cbStatus );
    }

}

/** @} */
/** @} */
