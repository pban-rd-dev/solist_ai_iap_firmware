/*****************************************************************************
 i2cf0_slv.c

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
 * @file    i2cf0_slv.c
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

#include "mcu.h"
#include "rdwr_reg.h"
#include "i2cf0.h"
#include "i2cf_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

#define I2F_SLAVE_DISABLE           ( 0U )			/**< Parameter unestablished  */
#define I2F_SLAVE_ENABLE            ( 1U )			/**< Parameter established    */


/**
 * @brief Control parameter for I2C slave (normal mode)
 */
typedef struct {
    uint8_t     *data;                              /**< pointer to area where the write/read data is stored      */
    uint32_t    dataSize;                           /**< size of write/read data                                  */
    uint32_t    cnt;                                /**< size of data which is writing/reading                    */
    cbfI2f_t    callBack;                           /**< callback function                                        */
    uint8_t     enable;                             /**< transfer mode ebakbe (0:disabel, 1:ebnable)              */
    uint8_t     errStat;                            /**< error status                                             */
	uint16_t    dummy;                              /**< Dummy for the padding                                    */
} I2fCtrlParamSlv_t;


/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
volatile static I2fCtrlParamSlv_t   s_ctrlParam_slv_read;    /**< control parameter for I2C slave with FIFO(nomal mode)    */
volatile static I2fCtrlParamSlv_t   s_ctrlParam_slv_write;   /**< control parameter for I2C slave with FIFO(nomal mode)    */

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Initialize I2C (Normal mode)
 *
 * @param[in]   mode     Configuration of I2C mode                           <br>
 *                       Specify the logical sum of the following items.     <br>
 * @arg                   Transmission mode     : I2F_MOD_STD, or I2F_MOD_FST
 * 
 * @param[in]   slaveAddr   Slave address                                    <br>
 *
 * @param[in]   rate     Set to Transfer rate.
 *
 * @return      None
 *
 */
void i2cf0_initSlaveMode(uint8_t mode, uint8_t rate , uint16_t slaveAddr)
{
	/*=== Register setting ===*/
	/* Set Transfer mode */
	set_reg32( I2CF0->I2F0CTL, ((mode & 0x03) | (1 << 7)) );
	
	/* Set transfer rate */
	write_reg32( I2CF0->I2F0BC, rate );
	
	/* Not use buffer mode */
	clear_bit( I2CF0->I2F0MOD, (1 << 0) );
	
	/* Disable DR_LD */
	clear_bit( I2CF0->I2F0CTL, (1 << 12) );
	
	/* Disable STP */
	clear_bit( I2CF0->I2F0CTL, (1 << 11) );
	
	/* Enable MCF */
	set_bit( I2CF0->I2F0CTL, (1 << 9) );
	
	/* Disable MAAS */
	clear_bit( I2CF0->I2F0CTL, (1 << 6) );
	
	/* Set slave address */
	write_reg32( I2CF0->I2F0SAD, slaveAddr );
	
	s_ctrlParam_slv_write.enable     = I2F_SLAVE_DISABLE;
	s_ctrlParam_slv_read.enable      = I2F_SLAVE_DISABLE;

}


/**
 * Start to send writing data (normal mode)
 *
 * @param[in]   *buf        Pointer to area where the write data is stored
 *
 * @param[in]   size        Size of data to write (unit is byte)                <br>
 *                          Exclude the size of slave address.
 *
 * @param[in]   func        Pointer to callback function
 *
 * @retval      I2F_R_OK(=0)               : I2C communication is started.      (success)
 */
int32_t i2cf0_slv_write( uint8_t *buf, uint16_t size, cbfI2f_t func )
{
	/*=== Transmission of a message system order parameter setting. ===*/
	s_ctrlParam_slv_write.enable     = I2F_SLAVE_ENABLE;
	s_ctrlParam_slv_write.data       = buf;
	s_ctrlParam_slv_write.dataSize   = size;
	s_ctrlParam_slv_write.cnt        = 0;
	s_ctrlParam_slv_write.callBack   = func;
	s_ctrlParam_slv_write.errStat    = 0;

	/* Enable MAAS interrupt */
	set_bit( I2CF0->I2F0CTL, (1 << 6));
	
	return ( I2F_R_OK );
}

/**
 * Start to receive reading data (normal mode)
 *
 * @param[in]   *buf        Pointer to area where the receive data is stored
 *
 * @param[in]   size        Size of data to read (unit is byte)                 <br>
 *                          Exclude the size of slave address.
 *
 * @param[in]   func        Pointer to callback function
 *
 * @retval      I2F_R_OK(=0)               : I2C communication is started.      (success)
 */
int32_t i2cf0_slv_read( uint8_t *buf, uint16_t size, cbfI2f_t func )
{
	/*=== Reception mode setting ===*/
	/*=== Transmission of a message system order parameter setting. ===*/
	s_ctrlParam_slv_read.enable      = I2F_SLAVE_ENABLE;
	s_ctrlParam_slv_read.data        = buf;
	s_ctrlParam_slv_read.dataSize    = size;
	s_ctrlParam_slv_read.cnt         = 0;
	s_ctrlParam_slv_read.callBack    = func;
	s_ctrlParam_slv_read.errStat     = 0;
	
	/* Enable MAAS interrupt */
	set_bit( I2CF0->I2F0CTL, (1 << 6));

	/* Read Dummy */
//	i2cf0_getc();
	
	return ( I2F_R_OK );
}



/**
 * Continue to communicate of writing/reading data (normal mode)
 *
 * @param       -
 *
 * @retval      I2F_R_TRANS_FIN(=1)        : Communication of writing/reading data is finished  (success/failure) <br>
 *                                           The callback function parameter has the causes of this failure.
 * @retval      I2F_R_TRANS_CONT_OK(=0)    : Communication of writing/reading data is continued (success)
 * @retval      I2F_R_INTERRUPT_NG(=-1)    : Interrupt ERROR
 */

int32_t i2cf0_slv_continue( void )
{
	int32_t ret = I2F_R_INTERRUPT_NG;
	
	/* Check external master device specifies as the slave device. */
	if(get_bit( I2CF0->I2F0SR, (1 << 6) ) != 0)
	{
		/* Clear interrupt */
		i2cf0_clearCF_IF();
		/* Clear MAAS interrupt */
		clear_bit( I2CF0->I2F0SR, (1 << 6));
		/* Check Write/Read */
		if(get_bit( I2CF0->I2F0SR, (1 << 2) ) == 0){
			/* Read */
			if(	s_ctrlParam_slv_read.enable == I2F_SLAVE_DISABLE)
			{
				/* intial errort */
				/* End of communication */
				s_ctrlParam_slv_read.errStat = I2F_ERR_SLVINIT;
				ret = I2F_R_INTERRUPT_NG;
			} else {
				/* Set Set ACK output for receive transfer completiont */
				clear_bit( I2CF0->I2F0CTL, (1 << 3));
				/* Enable MCF interruptt */
				set_bit( I2CF0->I2F0CTL, (1 << 9));
				/* Enable STP interruptt */
				set_bit( I2CF0->I2F0CTL, (1 << 11));
				
				/* Read Dummy */
				i2cf0_getc();
				ret = I2F_R_TRANS_CONT_OK;
			}
		} else {
			/* Write */
			if(	s_ctrlParam_slv_write.enable == I2F_SLAVE_DISABLE)
			{
				/* intial errort */
				s_ctrlParam_slv_write.errStat = I2F_ERR_SLVINIT;
				ret = I2F_R_INTERRUPT_NG;
			} else {
				/* data writet */
				i2cf0_putc( (uint8_t)*s_ctrlParam_slv_write.data );
				s_ctrlParam_slv_write.data++;
				s_ctrlParam_slv_write.cnt++;
				
				/* Enable MCF interruptt */
				set_bit( I2CF0->I2F0CTL, (1 << 9));
				/* Enable STP interruptt */
				set_bit( I2CF0->I2F0CTL, (1 << 11));
				ret = I2F_R_TRANS_CONT_OK;
			}
			
		}
		
	}
	
	/* Check Data transfer is completed. */
	else if(get_bit( I2CF0->I2F0SR, (1 << 7) ) != 0)
	{
		/* Check Write/Read */
		if(get_bit( I2CF0->I2F0SR, (1 << 2) ) == 0){
			/* readt */
			/* Get data */
			*s_ctrlParam_slv_read.data = (uint8_t)i2cf0_getc();
			s_ctrlParam_slv_read.data++;
			s_ctrlParam_slv_read.cnt++;
			/* Clear interrupt */
			i2cf0_clearCF_IF();
		} else {
			/* write */
			/* data write */
			i2cf0_putc( (uint8_t)*s_ctrlParam_slv_write.data );
			s_ctrlParam_slv_write.data++;
			s_ctrlParam_slv_write.cnt++;
			if( i2cf0_checkRXAK() == 1 ){
				s_ctrlParam_slv_write.enable = I2F_SLAVE_DISABLE;
				/* End of communication */
				if( s_ctrlParam_slv_write.callBack != (void *)0 ){
					s_ctrlParam_slv_write.callBack( s_ctrlParam_slv_write.cnt, s_ctrlParam_slv_write.errStat );
				}
				/* Disable MCF interruptt */
				clear_bit( I2CF0->I2F0CTL, (1 << 9));
				/* Disable STP interruptt */
				clear_bit( I2CF0->I2F0CTL, (1 << 11));
			}
			/* Clear interrupt */
			i2cf0_clearCF_IF();
		}
		ret = I2F_R_TRANS_CONT_OK;
	}
	
	/* Check Data reception is completed in the slave mode. */
	else if(get_bit( I2CF0->I2F0SR, (1 << 9) ) != 0)
	{
		/* Clear MIF interruptt */
		clear_bit( I2CF0->I2F0SR, (1 << 1));
		/* Clear STP interruptt */
		clear_bit( I2CF0->I2F0SR, (1 << 9));
		/* Check Write/Read */
		if(get_bit( I2CF0->I2F0SR, (1 << 2) ) == 0){
			/* readt */
			s_ctrlParam_slv_read.enable      = I2F_SLAVE_DISABLE;
			/* End of communication */
			if( s_ctrlParam_slv_read.callBack != (void *)0 ){
				s_ctrlParam_slv_read.callBack( s_ctrlParam_slv_read.cnt, s_ctrlParam_slv_read.errStat );
			}
			/* Clear interrupt */
			i2cf0_clearCF_IF();
			/* Disable MCF interruptt */
			clear_bit( I2CF0->I2F0CTL, (1 << 9));
			/* Disable STP interruptt */
			clear_bit( I2CF0->I2F0CTL, (1 << 11));
		} else {
			s_ctrlParam_slv_write.errStat = I2F_ERR_SLVSTP;
			s_ctrlParam_slv_write.enable  = I2F_SLAVE_DISABLE;
			/* End of communication */
			if( s_ctrlParam_slv_write.callBack != (void *)0 ){
				s_ctrlParam_slv_write.callBack( s_ctrlParam_slv_write.cnt, s_ctrlParam_slv_write.errStat );
			}
		}
		ret = I2F_R_TRANS_CONT_OK;
	}
	else 
	{
		/* There is no causes of interruptst */
		ret = I2F_R_INTERRUPT_NG;
	}
	
	return (ret);
}

/** @} */
/** @} */
