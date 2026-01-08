/*****************************************************************************
 i2cf0.h

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
 * @file    i2cf0.h
 *
 * This module is 'I2CF0' drivers.
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

#ifndef _I2CF0_H_
#define _I2CF0_H_

#include "rdwr_reg.h"
#include "i2cf_common.h"

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/


/**
 * Reading reception data
 *
 * @param[in]   data        A transmission data                                 <br>
                            (Setting value of I2F0DR register)
 * @return      None
 */
#define         i2cf0_putc( data )                  write_reg32( I2CF0->I2F0DR, data )

/**
 * Writing reception data
 *
 * @param[in]   -
 * @return      A reception data                                                <br>
 *              (A value of I2F0DR)
 */
#define         i2cf0_getc()                        read_reg32( I2CF0->I2F0DR )

/**
 * Trig StartCondition
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_trigStart()                   set_bit( I2CF0->I2F0CTL, (1 << 5) )

/**
 * Trig StopCondition
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_trigStop()                    clear_bit( I2CF0->I2F0CTL, (1 << 5) )

/**
 * Trig RestartCondition
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_trigRestart()                 set_bit( I2CF0->I2F0CTL, (1 << 2) )

/**
 * Check data reception
 *
 * @param[in]   -
 * @return      0 : ACK reception                                               <br>
 *              1 : NACK reception                                              <br>
 */
#define         i2cf0_checkRXAK()                   get_bit( I2CF0->I2F0SR, (1 << 0) )

/**
 * Trig StartCondition for buffer mode
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_trigStartBuffMode()           set_bit( I2CF0->I2F0BCT, (1 << 0) )

/**
 * Trig StopCondition for buffer mode
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_trigStopBuffMode()            clear_bit( I2CF0->I2F0BCT, (1 << 0) )

/**
 * Check data transfer complete
 *
 * @param[in]   -
 * @return      0 : not complete                                                <br>
 *              1 : completed                                                   <br>
 */
#define         i2cf0_checkBMFI()                   get_bit( I2CF0->I2F0BSR, (1 << 0) )

/**
 * Check ACK/NACK
 *
 * @param[in]   -
 * @return      0 : Has not received a NACK                                     <br>
 *              1 : Received a NACK and terminated transfer                     <br>
 */
#define         i2cf0_checkBMNA()                   get_bit( I2CF0->I2F0BSR, (1 << 2) )

/**
 * Check time out
 *
 * @param[in]   -
 * @return      0 : Timeout has not occurred                                    <br>
 *              1 : Timeout has occurred                                        <br>
 */
#define         i2cf0_checkBMTO()                   get_bit( I2CF0->I2F0BSR, (1 << 3) )

/**
 * Check illegal stop condition
 *
 * @param[in]   -
 * @return      0 : Has not detected an unexpected STOP condition               <br>
 *              1 : Detected an unexpected STOP condition                       <br>
 */
#define         i2cf0_checkBMIS()                   get_bit( I2CF0->I2F0BSR, (1 << 4) )

/**
 * Check transmit data count mismatch
 *
 * @param[in]   -
 * @return      0 : Match at the start of the transmission                      <br>
 *              1 : Do not match at the start of the transmission               <br>
 */
#define         i2cf0_checkBMAG()                   get_bit( I2CF0->I2F0BSR, (1 << 5) )

/**
 * Check transfer data count is 0
 *
 * @param[in]   -
 * @return      0 : The number of transferred bytes is not 0 at the start of transmission  <br>
 *              1 : The transmission has not been started                                  <br>
 */
#define         i2cf0_checkBMDZ()                   get_bit( I2CF0->I2F0BSR, (1 << 6) )

/**
 * Clear data transfer completion flag and interrupt flag
 *
 * @param[in]   -
 * @return      -                                                               <br>
 */
#define         i2cf0_clearCF_IF()     do {                                   \
                                        clear_bit( I2CF0->I2F0SR, ((1 << 7) | (1 << 1)) );    \
                                     } while (0)

/**
 * Clear buffer
 *
 * @param[in]   -
 * @return      -
 */
#define         i2cf0_clearBuffer()                       write_reg32( I2CF0->I2F0BLV, 0x0000 )

/**
 * Set slave address
 *
 * @param[in]   addr      Slave address
 * @return      -
 */
#define         i2cf0_setSlaveAddr( addr )                write_reg32( I2CF0->I2F0SAD, (addr & 0xFE) )

/**
 * Set buffer slave address
 *
 * @param[in]   addr      Slave address (for buffer mode)
 * @return      -
 */
#define         i2cf0_setBufferSlaveAddr( addr )          write_reg32( I2CF0->I2F0BSV, addr )

/**
 * Set sub address 0
 *
 * @param[in]   addr      Sub Address 0
 * @return      -
 */
#define         i2cf0_setSubAddr( addr )                  write_reg32( I2CF0->I2F0BSB, addr )

/**
 * Get the size of buffer
 *
 * @param[in]   -
 * @return      buffer size (0 - 4byte)
 */
#define         i2cf0_getBufferSize()                      read_reg32( I2CF0->I2F0BLV )

void            i2cf0_initNormalMode( uint8_t mode, uint8_t rate );
void            i2cf0_initBufferMode( uint8_t mode, uint8_t rate );
int32_t         i2cf0_write( uint8_t slaveAddr, uint8_t *addr, uint32_t addrSize, uint8_t *buf, uint16_t size, cbfI2f_t func );
int32_t         i2cf0_read( uint8_t slaveAddr, uint8_t *addr, uint32_t addrSize, uint8_t *buf, uint16_t size, cbfI2f_t func );
int32_t         i2cf0_writeBuffer( uint8_t slaveAddr, uint8_t subAddr1, uint8_t subAddr0, uint8_t *data, uint16_t size );
int32_t         i2cf0_readBuffer( uint8_t slaveAddr, uint8_t subAddr1, uint8_t subAddr0, uint8_t *data, uint16_t size );
int32_t         i2cf0_continue( void );
int32_t         i2cf0_continueBuffer( void );

void            i2cf0_initSlaveMode(uint8_t mode, uint8_t rate , uint16_t slaveAddr);
int32_t         i2cf0_slv_write( uint8_t *buf, uint16_t size, cbfI2f_t func );
int32_t         i2cf0_slv_read( uint8_t *buf, uint16_t size, cbfI2f_t func );
int32_t         i2cf0_slv_continue( void );



#endif /*_I2C0_H_*/

/** @} */
/** @} */
