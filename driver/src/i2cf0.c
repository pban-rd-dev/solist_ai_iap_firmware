/*****************************************************************************
 i2cf0.c

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
 * @file    i2cf0.c
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

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*--- Operation status ---*/
#define I2F_COMMUNICATION_END       ( 0 )       /**< Operation status : communication completion              */
#define I2F_TRANS_SLAVE_ADDRESS     ( 1 )       /**< Operation status : slave address send mode               */
#define I2F_TRANS_ADDRESS           ( 2 )       /**< Operation status : write/read address send mode          */
#define I2F_WRITE_DATA              ( 3 )       /**< Operation status : data write mode                       */
#define I2F_READ_DATA               ( 4 )       /**< Operation status : data read mode                        */

/**
 * @brief Control parameter for I2C master (normal mode)
 */
typedef struct {
    uint32_t    addrSize;                       /**< size of send address                                     */
    uint32_t    dataSize;                       /**< size of write/read data                                  */
    uint32_t    cnt;                            /**< size of data which is writing/reading                    */
    uint8_t     *addr;                          /**< pointer to area where the send/receive address is stored */
    uint8_t     *data;                          /**< pointer to area where the write/read data is stored      */
    cbfI2f_t    callBack;                       /**< callback function                                        */
    uint8_t     mode;                           /**< transfer mode(0:write, 1:read) after send slave address  */
    uint8_t     slaveAddr;                      /**< slave address                                            */
    uint8_t     errStat;                        /**< error status                                             */
    uint8_t     status;                         /**< writing/reading operation status                         */
} I2fCtrlParam_t;

/**
 * @brief Control parameter for I2C master (buffer mode)
 */
typedef struct {
	uint8_t     *data;                          /**< Read data from buffer                                    */
	uint16_t    size;                           /**< Read size                                                */
	uint8_t     mode;                           /**< transfer mode(0:write, 1:read)                           */
	uint8_t     dummy;						    /**< Dummy for the padding                                    */
} I2fBufParam_t;


/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
static I2fCtrlParam_t       s_ctrlParam;            /**< control parameter for I2C with FIFO (normal mode)   */
static I2fBufParam_t        s_bufParam;             /**< control parameter for I2C with FIFO (buffer mode)    */

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
 * @param[in]   rate     Set to Transfer rate.
 *
 * @return      None
 *
 */
void i2cf0_initNormalMode(uint8_t mode, uint8_t rate)
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
}

/**
 * Initialize I2C (Buffer mode)
 *
 * @param[in]   mode     Configuration of I2C mode                                 <br>
 *                       Specify the logical sum of the following items.           <br>
 * @arg                    Transmission mode     : I2F_MOD_STD, or I2F_MOD_FST     <br>
 *
 * @param[in]   rate     Set to Transfer rate.
 *
 * @return      None
 *
 */
void i2cf0_initBufferMode(uint8_t mode, uint8_t rate) 
{
	/*=== Register setting ===*/
	/* Set Transfer mode */
	set_reg32( I2CF0->I2F0CTL, ((mode & 0x03) | (1 << 7)) );
	
	/* Set transfer rate */
	write_reg32( I2CF0->I2F0BC, rate );
	
	/* Enable Buffer mode */
	set_bit( I2CF0->I2F0MOD, (1 << 0) );
	
	/* Set interrupt option (All enable) */
	write_reg32(
		I2CF0->I2F0BMK,
		(I2F_BMFT_ENA | I2F_BNIE_ENA | I2F_BTIE_ENA | I2F_BSIE_ENA |I2F_BAIE_ENA | I2F_BDIE_ENA)
	);
}

/**
 * Start to send writing data (normal mode)
 *
 * @param[in]   slaveAddr   Slave address                                       <br>
 *
 * @param[in]   *addr       Pointer to area where the adress data is stored
 *
 * @param[in]   addrSize    Size of data to adress (unit is byte)               <br>
 *                          Exclude the size of slave address.                  <br>
 *                          If you don't need to transmit address data, Set this parameter '0'.
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
int32_t i2cf0_write( uint8_t slaveAddr, uint8_t *addr, uint32_t addrSize, uint8_t *buf, uint16_t size, cbfI2f_t func )
{
	/*=== Transmission of a message system order parameter setting. ===*/
	s_ctrlParam.mode       = 0;    /* write */
	s_ctrlParam.addr       = addr;
	s_ctrlParam.addrSize   = addrSize;
	s_ctrlParam.data       = buf;
	s_ctrlParam.dataSize   = size;
	s_ctrlParam.cnt        = 0;
	s_ctrlParam.callBack   = func;
	s_ctrlParam.errStat    = 0;
	s_ctrlParam.status     = 0;

	/*=== transmit a address, and it is worked to start. ===*/
	s_ctrlParam.status = I2F_TRANS_SLAVE_ADDRESS;
	
	/* Check Bus idle */
	while ( get_bit(I2CF0->I2F0SR, (1 << 5)) != 0 ) { }
	
	/* Write mode */
	set_bit( I2CF0->I2F0CTL, (1 << 4));
	
	/* Set slave address */
	write_reg32( I2CF0->I2F0DR, slaveAddr );
	
	/* Start condition */
	i2cf0_trigStart();
	
	return ( I2F_R_OK );
}

/**
 * Start to receive reading data (normal mode)
 *
 * @param[in]   slaveAddr   Slave address                                       <br>
 *
 * @param[in]   *addr       Pointer to area where the adress data is stored
 *
 * @param[in]   addrSize    Size of data to adress (unit is byte)               <br>
 *                          Exclude the size of slave address.                  <br>
 *                          If you don't need to transmit address data, Set this parameter '0'.
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
int32_t i2cf0_read( uint8_t slaveAddr, uint8_t *addr, uint32_t addrSize, uint8_t *buf, uint16_t size, cbfI2f_t func )
{
	/*=== Reception mode setting ===*/
	/*=== Transmission of a message system order parameter setting. ===*/
	s_ctrlParam.mode        = 1;    /* read */
	s_ctrlParam.addr        = addr;
	s_ctrlParam.addrSize    = addrSize;
	s_ctrlParam.data        = buf;
	s_ctrlParam.dataSize    = size;
	s_ctrlParam.cnt         = 0;
	s_ctrlParam.callBack    = func;
	s_ctrlParam.errStat     = 0;
	s_ctrlParam.status      = 0;

	/*=== I receive it, and it is worked to start. ===*/
	s_ctrlParam.status = I2F_TRANS_SLAVE_ADDRESS;

	/* save slave address */
	s_ctrlParam.slaveAddr = slaveAddr;
	
	/* Check Bus idle */
	while ( get_bit(I2CF0->I2F0SR, (1 << 5)) != 0 ) { }

	if( addrSize == 0 ){
		clear_bit( I2CF0->I2F0CTL, (1 << 4) );        /* Data receive mode (data read) */
		write_reg32( I2CF0->I2F0DR, slaveAddr | 0x01 );  /* Set slave adrress */
	}
	else{
		set_bit( I2CF0->I2F0CTL, (1 << 4) );         /* Data transmit mode (data write) */
		write_reg32( I2CF0->I2F0DR, slaveAddr);  /* Set slave adrress */
	}
	
	/* Start condition */
	i2cf0_trigStart();
	
	return ( I2F_R_OK );
}

/**
 * Start to send writing data (buffer mode)
 *
 * @param[in]   slaveAddr   Slave address                                       <br>
 *
 * @param[in]   subAddr1    Sub address 1
 *
 * @param[in]   subAddr0    Sub address 0
 *
 * @param[in]   *data       Pointer to area where the receive data is stored
 *
 * @param[in]   size        Size of data to read (unit is byte)                 <br>
 *                          Exclude the size of slave address.
 *
 * @retval      I2F_R_OK(=0)               : I2C communication is started.      (success)
 */
int32_t i2cf0_writeBuffer( uint8_t slaveAddr, uint8_t subAddr1, uint8_t subAddr0, uint8_t *data, uint16_t size )
{
	uint16_t loopCnt;
	/*=== Transmission of a message system order parameter setting. ===*/
	s_bufParam.mode = 0;   /* write */
	
	/* Set slave address */
	i2cf0_setBufferSlaveAddr( slaveAddr );
	
	/* Set sub address */
	i2cf0_setSubAddr( (subAddr1 << 8 ) | subAddr0 );
	
	/* Buffer clear */
	i2cf0_clearBuffer();
	
	/* Set buffer format */
	write_reg32( I2CF0->I2F0BFR, ((I2F_SUB_SENT10 | I2F_BUF_DIR_TRANS) | (size << 8)));
	
	for (loopCnt=0; loopCnt < size ; loopCnt++) {
		i2cf0_putc( (uint8_t)*data );
		data++;
	}
	
	/* Check Bus idle */
	while ( get_bit(I2CF0->I2F0SR, (1 << 5)) != 0 ) { }
	
	/* Start condition */
	i2cf0_trigStartBuffMode();
	
	return ( I2F_R_OK );
}

/**
 * Start to receive reading data (buffer mode)
 *
 * @param[in]   slaveAddr   Slave address                                       <br>
 *
 * @param[in]   subAddr1    Sub address 1
 *
 * @param[in]   subAddr0    Sub address 0
 *
 * @param[in]   *data       Pointer to area where the receive data is stored
 *
 * @param[in]   size        Size of data to read (unit is byte)                 <br>
 *                          Exclude the size of slave address.
 *
 * @retval      I2F_R_OK(=0)               : I2C communication is started.      (success)
 */
int32_t i2cf0_readBuffer( uint8_t slaveAddr, uint8_t subAddr1, uint8_t subAddr0, uint8_t *data, uint16_t size )
{
	/*=== Transmission of a message system order parameter setting. ===*/
	s_bufParam.mode = 1;    /* read */
	s_bufParam.size = size; /* read size */
	s_bufParam.data = data; /* read data */
	
	/* Set slave address */
	i2cf0_setBufferSlaveAddr( slaveAddr );
	
	/* Set sub address */
	i2cf0_setSubAddr( subAddr0 | (subAddr1 << 8) );
	
	/* Buffer clear */
	i2cf0_clearBuffer();
	
	/* Set buffer format */
	write_reg32( I2CF0->I2F0BFR, ((I2F_SUB_SENT10 | I2F_BUF_DIR_RECV) | (size << 8)) );
	
	/* Check Bus idle */
	while ( get_bit(I2CF0->I2F0SR, (1 << 5)) != 0 ) { }
	
	/* Start condition */
	i2cf0_trigStartBuffMode();
	
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
 */

int32_t i2cf0_continue( void )
{
	switch( s_ctrlParam.status ){
		case I2F_TRANS_SLAVE_ADDRESS:
			/* Clear interrupt */
			i2cf0_clearCF_IF();
			
			/* NACK reception check */
			if( i2cf0_checkRXAK() != 0 ){
				s_ctrlParam.status = I2F_COMMUNICATION_END;
				s_ctrlParam.errStat = I2F_ERR_ACR;
				i2cf0_trigStop();       /* Stop condition */
			}
			/* Slave address transmission success */
			else{
				if( s_ctrlParam.addrSize > 0 ){
					s_ctrlParam.status = I2F_TRANS_ADDRESS;
					i2cf0_putc( (uint8_t)*s_ctrlParam.addr );
					s_ctrlParam.addr++;
					s_ctrlParam.addrSize--;
				}
				else{
					/* Read */
					if( s_ctrlParam.mode != 0 ){
						if( ( s_ctrlParam.dataSize - 1) == s_ctrlParam.cnt ){
							s_ctrlParam.status = I2F_COMMUNICATION_END;
							/* Out NACK */
							set_bit( I2CF0->I2F0CTL, (1 << 3) );
						}
						else {
							s_ctrlParam.status = I2F_READ_DATA;
							clear_bit( I2CF0->I2F0CTL, (1 << 3) );
						}
						/* dummy read */
						*s_ctrlParam.data = (uint8_t)i2cf0_getc();
					}
					/* Write */
					else{
						s_ctrlParam.status = I2F_WRITE_DATA;
						i2cf0_putc( (uint8_t)*s_ctrlParam.data );
						s_ctrlParam.data++;
						s_ctrlParam.cnt++;
					}
				}
			}
			return(I2F_R_TRANS_CONT_OK);

		case I2F_TRANS_ADDRESS:
			/* Clear interrupt */
			i2cf0_clearCF_IF();
			
			if( i2cf0_checkRXAK() != 0 ){
				s_ctrlParam.status = I2F_COMMUNICATION_END;
				s_ctrlParam.errStat = I2F_ERR_ACR;
				i2cf0_trigStop();       /* Stop condition */
			}
			else{
				if( s_ctrlParam.addrSize > 0 ){
					i2cf0_putc( (uint8_t)*s_ctrlParam.addr );
					s_ctrlParam.addr++;
					s_ctrlParam.addrSize--;
					if( ( s_ctrlParam.addrSize == 0 ) &&
						( s_ctrlParam.mode != 0 ) ){    /* Read mode */
						/* Last transmission data */
						set_bit( I2CF0->I2F0CTL, (1 << 13) );
					}
				}
				/* Address transmission success */
				else{
					if( s_ctrlParam.dataSize == 0 ){
						s_ctrlParam.status = I2F_COMMUNICATION_END;
						i2cf0_trigStop();       /* Stop condition */
					}
					else{
						if( s_ctrlParam.mode != 0 ){    /* Read mode */
							s_ctrlParam.status = I2F_TRANS_SLAVE_ADDRESS;
							/* Set slave adrress for READ */
							write_reg32( I2CF0->I2F0DR, s_ctrlParam.slaveAddr | 0x01 );
							i2cf0_trigRestart();       /* Restart condition */
							clear_bit( I2CF0->I2F0CTL, (1 << 4) );
							clear_bit( I2CF0->I2F0CTL, (1 << 13) );
						}
						else{/* send mode */
							s_ctrlParam.status = I2F_WRITE_DATA;
							i2cf0_putc( (uint8_t)*s_ctrlParam.data );
							s_ctrlParam.data++;
							s_ctrlParam.cnt++;
						}
					}
				}
			}
			return ( I2F_R_TRANS_CONT_OK );

		case I2F_WRITE_DATA:
			/* Clear interrupt */
			i2cf0_clearCF_IF();
			
			/* NACK reception check */
			if( i2cf0_checkRXAK() == 1 ){
				s_ctrlParam.status = I2F_COMMUNICATION_END;
				s_ctrlParam.errStat = I2F_ERR_ACR;
				i2cf0_trigStop();       /* Stop condition */
			}
			/* Check to communication finish */
			else if( s_ctrlParam.dataSize > s_ctrlParam.cnt ){
				clear_bit( I2CF0->I2F0CTL, (1 << 3) );
				i2cf0_putc( (uint8_t)*s_ctrlParam.data );
				s_ctrlParam.data++;
				s_ctrlParam.cnt++;
			}
			else{
				s_ctrlParam.status = I2F_COMMUNICATION_END;
				i2cf0_putc( (uint8_t)0 );
			}
			return ( I2F_R_TRANS_CONT_OK );

		case I2F_READ_DATA:
			/* Clear intrruput */
			i2cf0_clearCF_IF();
			/* Get data */
			*s_ctrlParam.data = (uint8_t)i2cf0_getc();
			s_ctrlParam.data++;
			s_ctrlParam.cnt++;
			if( ( s_ctrlParam.dataSize - 1) == s_ctrlParam.cnt ){
				s_ctrlParam.status = I2F_COMMUNICATION_END;
				/* Out NACK */
				set_bit( I2CF0->I2F0CTL, (1 << 3) );
			}
			return ( I2F_R_TRANS_CONT_OK );

		case I2F_COMMUNICATION_END:
		default:
			/* Clear interrupt */
			i2cf0_clearCF_IF();
			
			/* End of communication */
			if( s_ctrlParam.callBack != (void *)0 ){
				s_ctrlParam.callBack( s_ctrlParam.cnt, s_ctrlParam.errStat );
			}
			i2cf0_trigStop();

			/* Read */
			if( s_ctrlParam.mode != 0 ){
				/* Get data */
				*s_ctrlParam.data = (uint8_t)i2cf0_getc();
				s_ctrlParam.data++;
				s_ctrlParam.cnt++;
			}
			break;
	}
	return ( I2F_R_TRANS_FIN );
}

/**
 * Continue to communicate of writing/reading data (buffer mode)
 *
 * @param       -
 *
 * @retval      I2F_R_TRANS_FIN(=1)        : Communication of writing/reading data is finished  (success/failure) <br>
 *                                           The callback function parameter has the causes of this failure.
 * @retval      I2F_ERR_NACK(=-1)          : Nack Receiption
 * @retval      I2F_ERR_BMDZ(=-2)          : Transferred bytes is 0 of transmission
 * @retval      I2F_ERR_BMAG(=-3)          : Capacity number of mismatch of transfer bytes and buffer
 * @retval      I2F_ERR_BMIS(=-4)          : Unexpected stop condition
 * @retval      I2F_ERR_BMTO(=-5)          : Timeout
 */
int32_t i2cf0_continueBuffer( void )
{
	int32_t ret = I2F_R_TRANS_FIN;
	uint16_t loopCnt;
	
	if( i2cf0_checkBMDZ() != 0 ){
		/* clear interrupt */
		clear_bit( I2CF0->I2F0BSR, (1 << 6) );
		ret = I2F_ERR_BMDZ;
	}
	else if( i2cf0_checkBMAG() != 0 ){
		/* clear interrupt */
		clear_bit( I2CF0->I2F0BSR, (1 << 5) );
		ret = I2F_ERR_BMAG;
	} 
	else {
		if( i2cf0_checkBMIS() != 0 ){
			/* clear interrupt */
			clear_bit( I2CF0->I2F0BSR, (1 << 4) );
			ret = I2F_ERR_BMIS;
		}
		else if( i2cf0_checkBMTO() != 0 ){
			/* clear interrupt */
			clear_bit( I2CF0->I2F0BSR, (1 << 3) );
			ret = I2F_ERR_BMTO;
		}
		else if( i2cf0_checkBMNA() != 0 ){
			/* clear interrupt */
			clear_bit( I2CF0->I2F0BSR, (1 << 2) );
			ret = I2F_ERR_NACK;
		}
		else if( i2cf0_checkBMFI() != 0 ){
			/* Transfer in the buffer mode is completed */
			/* clear interrupt */
			clear_bit( I2CF0->I2F0BSR, (1 << 0) );
			if (s_bufParam.mode == 1) {
			/* Read buffer */
				for (loopCnt=0; loopCnt<s_bufParam.size ; loopCnt++) {
					*s_bufParam.data = (uint8_t)i2cf0_getc();
					s_bufParam.data++;
				}
			}
			else {
			/* Write Mode */
			}
		}
		else{
			/* Transfer in the buffer mode is not completed */
		}
	}
	return ( ret );
}

/** @} */
/** @} */
