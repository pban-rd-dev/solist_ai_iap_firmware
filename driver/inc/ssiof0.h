/*****************************************************************************
 ssiof0.h

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
 * @file    ssiof0.h
 *
 * This module is 'SSIOF0' drivers.
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

#ifndef SSIOF0_H__
#define SSIOF0_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "ssiof_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize SSIOF
 *
 * @param[in]   sfnctrl     Configuration of SSIOF mode , and so on             <br>
 *                          (Setting value of SF0CTRL register)                 <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Master/Slave                : SSIOF_MST_MASTER, or SSIOF_MST_SLAVE
 * @arg                         Buffer mode                 : SSIOF_LG_8BIT, or SSIOF_LG_16BIT
 * @arg                         Mode fault                  : SSIOF_MDF_DIS, or SSIOF_MDF_ENA
 * @arg                         Endian                      : SSIOF_DIR_LSB, or SSIOF_DIR_MSB
 * @arg                         Clock phase                 : SSIOF_CPHA_1SM_2SH, or SSIOF_CPHA_1SH_2SM
 * @arg                         Clock type                  : SSIOF_CPOL_LOW, or SSIOF_CPOL_HIGH
 * @arg                         SSn output                  : SSIOF_SSZ_OUTPUT, or SSIOF_SSZ_HIZ
 * @arg                         MISO output                 : SSIOF_SOZ_OUTPUT, or SSIOF_SOZ_HIZ
 * @arg                         MOSI output                 : SSIOF_MOZ_OUTPUT, or SSIOF_MOZ_HIZ
 *
 * @param[in]   sfnintc     Configuration of interrupt                          <br>
 *                          (Setting value of SF0INTC register)                 <br>
 *                          Specify the logical sum of the following items.
 * @arg                         SFnTFIE                     : SSIOF_INT_TFIE_DIS
 * @arg                         SFnRFIE                     : SSIOF_INT_RFIE_DIS
 * @arg                         SFnFIE                      : SSIOF_INT_FIE_DIS
 * @arg                         SFnORIE                     : SSIOF_INT_ORIE_DIS
 * @arg                         SFnMFIE                     : SSIOF_INT_MFIE_DIS
 * @arg                         Interrupt threshold (Write) : SSIOF_INT_WR_THRESH_0, or SSIOF_INT_WR_THRESH_1, or SSIOF_INT_WR_THRESH_2, or SSIOF_INT_WR_THRESH_3
 * @arg                         Interrupt threshold (Read)  : SSIOF_INT_RD_THRESH_1, or SSIOF_INT_RD_THRESH_2, or SSIOF_INT_RD_THRESH_3, or SSIOF_INT_RD_THRESH_4
 * 
 * @return      None
 * @note        If you set master mode(=SSIOF_MST_MASTER), wait over 100ns.
 * @note        Trigger-Level of the Reading FIFO is fixed at 1Byte.
 * @note        Initial state to disable interrupts.
 */
#define         ssiof0_init( sfnctrl, sfnintc )           do {                                                   \
                                                            ssiof0_clearFifo();                                  \
                                                            write_reg32( SSIOF0->SF0CTRL,  sfnctrl );  \
                                                            write_reg32( SSIOF0->SF0INTC,  (sfnintc & (SSIOF_INT_WR_THRESH_3|SSIOF_INT_RD_THRESH_4|SSIOF_INT_TFIE_ENA|SSIOF_INT_RFIE_ENA|SSIOF_INT_FIE_ENA|SSIOF_INT_ORIE_ENA|SSIOF_INT_MFIE_ENA)) ); \
                                                             }while(0)

/**
 * Setting baud-rate
 *
 * @param[in]   sfnbrr  Setting baud-rate                                       <br>
 *                      (Setting value of SF0BRR register)                      <br>
 *                          Specify the logical sum of the following items.
 * @arg                 baud-rate               : 0-1023
 * @arg                 SF0LEAD                 : SSIOF_LEAD_05 or SSIOF_LEAD_10 or SSIOF_LEAD_15
 * @arg                 SF0LAG                  : SSIOF_LAG_05 or SSIOF_LAG_10 or SSIOF_LAG_15
 * 
 * @note
 * @arg         About baud-rate clock calculation                               <br>
 *                  Baud-rate clock = 'HSCLK' / ( 2 * 'sfnbrr[9:0]')            <br>
 * @arg         About maximum of baud-rate clock frequency                      <br>
 *                  Do not set greater than 4MHz baud-rate.
 * @return      None
 *
 * @note        The maximum SSIOF0 transfer frequency is 4MHz. This setting should not exceed 4MHz.
 */
#define         ssiof0_setBaudrate( sfnbrr )              write_reg32( SSIOF0->SF0BRR, sfnbrr )

/**
 * Setting interval time
 *
 * @param[in]   sfntrac Setting interval time <br>
 *                      (Setting value of SF0TRAC register)
 * @return      None
 * @note        This macro is master mode only.
 */
#define         ssiof0_setIntervalTime( sfntrac )         write_reg32( SSIOF0->SF0TRAC, sfntrac )

/**
 * Writing transmission of data (for 8bit access)
 *
 * @param[in]   data    A transmission data (8bit length)                       <br>
 *                      (Setting value of SF0DWR register)
 * @return      None
 */
#define         ssiof0_putcByte( data )   write_reg8( SSIOF0->SF0DWR, data )

/**
 * Writing transmission of data (for 16bit access)
 *
 * @param[in]   data    A transmission data (16bit length)                      <br>
 *                      (Setting value of SF0DWR register)
 * @return      None
 */
#define         ssiof0_putcWord( data )   write_reg16( SSIOF0->SF0DWR, data )

/**
 * Reading reception data (for 8bit access)
 *
 * @param       -
 * @return      A reception data (8bit length)                                  <br>
 *              (A value of SF0DRR register)
 */
#define         ssiof0_getcByte()         read_reg8( SSIOF0->SF0DRR )

/**
 * Reading reception data (for 16bit access)
 *
 * @param       -
 * @return      A reception data (16bit length)                                 <br>
 *              (A value of SF0DRR register)
 */
#define         ssiof0_getcWord()         read_reg16( SSIOF0->SF0DRR )

/**
 * Getting SSIOF status
 *
 * @param       -
 * @return      SSIOF status <br>
 *              (A value of SF0SRR register)
 */
#define         ssiof0_getStatus()        read_reg32( SSIOF0->SF0SRR )

/**
 * Crear SSIOF status
 *
 * @param       sfnsrc  Mask value of the status you want to clear      <br>
 *                      Specify the logical sum of the following items.
 * @arg                     SF0TFC      : SFnSRC_SFnTFC
 * @arg                     SF0RFC      : SFnSRC_SFnRFC
 * @arg                     SF0FC       : SFnSRC_SFnFC
 * @arg                     SF0ORFC     : SFnSRC_SFnORFC
 * @arg                     SF0MDFC     : SFnSRC_SFnMDFC
 * @arg                     SF0SPIFC    : SFnSRC_SFnSPIFC
 * @arg                     SF0WOFC     : SFnSRC_SFnWOFC
 *
 * @return      None
 */
#define         ssiof0_clearStatus( sfnsrc )  write_reg32( SSIOF0->SF0SRC, sfnsrc )

/**
 * Checking SSIOF busy status
 *
 * @param       -
 * @return      Mask value of status
 * @retval      0   busy status : Ready(able to read/write next operation)
 * @retval      1   busy status : Busy
 */
#define         ssiof0_checkReady()       ( ( ! get_bit( SSIOF0->SF0SRR, (1 << 5) ) ) )

/**
 * Getting FIFO(for read) size
 *
 * @param       -
 * @return      Received data size in the FIFO
 */
#define         ssiof0_getReadFifoSize()  (( read_reg32( SSIOF0->SF0FSR ) >> 8) & 0x00000007 )

/**
 * Getting FIFO(for write) size
 *
 * @param       -
 * @return      The size of data remaining in the FIFO
 */
#define         ssiof0_getWriteFifoSize() ( read_reg32( SSIOF0->SF0FSR ) & 0x00000007 )

/**
 * Stop Stop communication
 *
 * @param       -
 * @return      None
 */
#define         ssiof0_stop()             clear_bit( SSIOF0->SF0CTRL, 0x00000001U )

void            ssiof0_clearFifo(void);
int32_t         ssiof0_start( uint8_t mode, void *rxData, void *txData, uint32_t dataCnt, cbfSsiof_t func );
int32_t         ssiof0_continue( void );

#endif /* SSIOF0_H__ */

/** @} */
/** @} */
