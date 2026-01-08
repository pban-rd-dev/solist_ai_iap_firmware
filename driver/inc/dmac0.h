/*****************************************************************************
 dmac0.h

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
 * @file    dmac0.h
 *
 * This module is 'DMAC0' drivers.
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

#ifndef DMAC0_H_
#define DMAC0_H_

#include "mcu.h"
#include "rdwr_reg.h"
#include "dmac_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Set DMAC0 Enable 
 *
 * @param       -
 * @return      None
 */
#define         dmac0_enable()         clear_bit( DMAC0->DMACMSK0 , (0x00000001UL) )

/**
 * Set DMAC0 Disable 
 *
 * @param       -
 * @return      None
 */
#define         dmac0_disable()        set_bit( DMAC0->DMACMSK0 , (0x00000001UL) )

/**
 * Clear Interrupt status  
 *
 * @param       -
 * @return      None
 */
#define        dmac0_clrIntStatus()    write_reg32( DMAC0->DMACCINT0 ,  (0x00000000UL) )


/**
 * Check Transfer Count
 *
 * @param       -
 * @return      DMACSIZ0 registor read value
 */
#define        dmac0_checkTransferSize()    read_reg32( DMAC0->DMACSIZ0 )


void dmac0_init( uint32_t tmod , uint8_t dreq_sel , cbfDmac_t func); 
void dmac0_Transfer (void *dma_sad , void *dma_dad , uint32_t dma_size );
void dmac0_endTrans(void);

#endif /*DMAC0_H_*/

/** @} */
/** @} */
