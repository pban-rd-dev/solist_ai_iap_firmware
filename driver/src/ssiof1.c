/*****************************************************************************
 ssiof1.c

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
 * @file    ssiof1.c
 *
 * This module is 'SSIOF1' drivers.
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

#include "ssiof1.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
static ssiofCtrlParam_t   s_ctrlParam;        /**< control parameter for SSIOF    */

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

static void s_ssiof_writeSingleData_8( ssiofCtrlParam_t *param );
static void s_ssiof_writeDummyData_8( ssiofCtrlParam_t *param );
static void s_ssiof_writeSingleData_16( ssiofCtrlParam_t *param );
static void s_ssiof_writeDummyData_16( ssiofCtrlParam_t *param );
static void s_ssiof_writeFifo( ssiofCtrlParam_t *param, void (*wrFunc)( ssiofCtrlParam_t *prm ), unsigned char wrSize );
static void s_ssiof_writeDummyFifo( ssiofCtrlParam_t *param, void (*wrFunc)( ssiofCtrlParam_t *prm ), unsigned char wrSize );

static void s_ssiof_readSingleData_8( ssiofCtrlParam_t *param );
static void s_ssiof_readSingleData_16( ssiofCtrlParam_t *param );
static void s_ssiof_readSingleDataNotCnt_8( ssiofCtrlParam_t *param );
static void s_ssiof_readSingleDataNotCnt_16( ssiofCtrlParam_t *param );
static void s_ssiof_readFifo( ssiofCtrlParam_t *param, void (*rdFunc)( ssiofCtrlParam_t *prm ) );

/*############################################################################*/
/*#                              Subroutine                                  #*/
/*############################################################################*/

/**
 * Writing a data
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_writeSingleData_8( ssiofCtrlParam_t *param )
{
	ssiof1_putcByte( *((unsigned char *)param->txData) );
	param->txData = ( (unsigned char *)s_ctrlParam.txData ) + 1;
	param->cnt++;
}

/**
 * Writing a data(Dummy)
 *
 * @param[in,out]   *param  Pointer to control parameters <br>
 * @return          None
 */
static void s_ssiof_writeDummyData_8( ssiofCtrlParam_t *param )
{
	ssiof1_putcByte( (unsigned char)SSIOF_DUMMY_DATA );
	param->dmyCnt--;
}

/**
 * Writing a data
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_writeSingleData_16( ssiofCtrlParam_t *param )
{
	ssiof1_putcWord( *((unsigned short *)param->txData) );
	/* pointing next write data */
	param->txData = (void*)(( (unsigned short *)s_ctrlParam.txData ) + 1);
	param->cnt++;
}

/**
 * Writing a data(Dummy)
 *
 * @param[in,out]   *param  Pointer to control parameters <br>
 * @return          None
 */
static void s_ssiof_writeDummyData_16( ssiofCtrlParam_t *param )
{
	ssiof1_putcWord( (unsigned short)SSIOF_DUMMY_DATA );
	param->dmyCnt--;
}

/**
 * Writing data to FIFO
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @param[in]       *wrFunc Pointer to write data function
 * @param[in]       wrSize  The size of next writing
 * @return          None
 */
__INLINE static void s_ssiof_writeFifo( ssiofCtrlParam_t *param, void (*wrFunc)( ssiofCtrlParam_t *prm ), unsigned char wrSize )
{
	if( (param->cnt + wrSize) > param->dataSize ) {
		wrSize = (unsigned char)(param->dataSize - param->cnt);
	}
	/* Write data (to FIFO) */
	for( ; wrSize != 0; wrSize-- ) {
		wrFunc( param );
	}
}

/**
 * Writing data to FIFO(Dummy)
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @param[in]       *wrFunc Pointer to write data function
 * @param[in]       wrSize  The size of next writing
 * @return          None
 */
__INLINE static void s_ssiof_writeDummyFifo( ssiofCtrlParam_t *param, void (*wrFunc)( ssiofCtrlParam_t *prm ), unsigned char wrSize )
{
	if( (param->dmyCnt ) < wrSize ) {
		wrSize = (unsigned char)(param->cnt);
	}
	/* Write dummy data (to FIFO) */
	for( ; wrSize != 0; wrSize-- ) {
		wrFunc( param );
	}
}

/**
 * Reading a data
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_readSingleData_8( ssiofCtrlParam_t *param )
{
	*( (unsigned char *)param->rxData ) = (unsigned char)ssiof1_getcByte();
	param->rxData = ( (unsigned char *)param->rxData ) + 1;
	param->cnt++;
}

/**
 * Reading a data
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_readSingleData_16( ssiofCtrlParam_t *param )
{
	*( (unsigned short *)param->rxData ) = (unsigned short)ssiof1_getcWord();
	param->rxData = (void*)(( (unsigned short *)param->rxData ) + 1);
	param->cnt++;
}

/**
 * Reading a data (not count)
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_readSingleDataNotCnt_8( ssiofCtrlParam_t *param )
{
	*( (unsigned char *)param->rxData ) = (unsigned char)ssiof1_getcByte();
	param->rxData = ( (unsigned char *)param->rxData ) + 1;
}

/**
 * Reading a data (not count)
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
static void s_ssiof_readSingleDataNotCnt_16( ssiofCtrlParam_t *param )
{
	*( (unsigned short *)param->rxData ) = (unsigned short)ssiof1_getcWord();
	param->rxData = (void*)(( (unsigned short *)param->rxData ) + 1);
}

/**
 * Reading data from FIFO
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @param[in]       *rdFunc Pointer to read data function
 * @return          None
 */
__INLINE static void s_ssiof_readFifo( ssiofCtrlParam_t *param, void (*rdFunc)( ssiofCtrlParam_t *prm ) )
{
	unsigned char   rdSize;

	rdSize = (unsigned char)ssiof1_getReadFifoSize();        /* size of reading */
	/* Read data (from FIFO) */
	for( ; rdSize != 0; rdSize-- ) {
		rdFunc( param );
	}
}

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Crear FIFO
 *
 * @param       -
 * @return      None
 */
void ssiof1_clearFifo(void){
	unsigned char stat = 0;

	stat = get_bit(SSIOF1->SF1CTRL, (1 << 0));

	clear_bit( SSIOF1->SF1CTRL, (1 << 0) );
	set_bit( SSIOF1->SF1CTRL, (1 << 8) );
	clear_bit( SSIOF1->SF1CTRL, (1 << 8) );

	if(stat == 1) {
		set_bit(SSIOF1->SF1CTRL, (1 << 0));
	}
}


/**
 * Start to read/write data
 *
 * @param[in]   mode        Operation of SSIOF mode                                                 <br>
 *
 * @arg                         SSIOF_MODE_RD : Reading (receive)
 * @arg                         SSIOF_MODE_WR : Writing (send)
 * @arg                         SSIOF_MODE_RD_WR : Reading / Writing(receive/send)
 *
 * @param[in]   *rxData     Pointer to area where the read data is stored                           <br>
 *                              If bit-length is 16bit(SFnSIZ=1), this parameter must be 16bit address.
 *
 * @param[in]   *txData     Pointer to area where the write data is stored                          <br>
 *                              If bit-length is 16bit(SFnSIZ=1), this parameter must be 16bit address.
 *
 * @param[in]   dataCnt     Count of reading/writing operation (unit is 8bit or 16bit)              <br>
 *                          If bit-length is 8bit(SFnSIZ=0), set 8bit value to this parameter.      <br>
 *                          If bit-length is 16bit(SFnSIZ=1), set 16bit value to this parameter.    <br>
 *                          ex. 4word data writing operation                                        <br>
 * @code
                unsigned short writeData[4] = { 0x0000,0x0001,0x0002,0x0003 };
                ssiof1_start( SSIOF_MODE_WR, (void*)0, writeData, 4, anyFunction );
   @endcode
 *
 * @param[in]   *func       Pointer to callback function
 *
 * @retval      SSIOF_R_OK               : SSIOF communication is started.       (success)
 * @retval      SSIOF_R_ERR              : A parameter 'mode' is incorrect.      (error)
 *
 * @note        Transmitting data via SSIOF makes receiving dummy data at FIFO.                      <br>
 *              Therefore receive FIFO shall be cleared before receiving data when data was sent.
 */
int32_t ssiof1_start( uint8_t mode, void *rxData, void *txData, uint32_t dataCnt, cbfSsiof_t func )
{
	void             (*wrFunc)( ssiofCtrlParam_t *param );
	unsigned char    opUnit;

	/* Unit of a reading/writing operation  */
	opUnit = ( unsigned char )get_bit( SSIOF1->SF1CTRL, (1 << 2) );

	/*=== Parameters setting ===*/
	s_ctrlParam.rxData      = rxData;
	s_ctrlParam.txData      = txData;
	s_ctrlParam.dataSize    = dataCnt;
	s_ctrlParam.cnt         = 0;
	s_ctrlParam.dmyCnt      = dataCnt;
	s_ctrlParam.callBack    = func;
	s_ctrlParam.errStat     = 0;
	s_ctrlParam.status      = 0;

	switch( mode ) {
		/* Read(data receive) mode      */
		case SSIOF_MODE_RD:
			/* Master mode */
			if( get_bit(SSIOF1->SF1CTRL, (1 << 1)) == 1 ) {
				if( opUnit == 0 ) {
					wrFunc = s_ssiof_writeDummyData_8;
				}
				else {
					wrFunc = s_ssiof_writeDummyData_16;
				}
				/* write dummy data to FIFO         */
				s_ssiof_writeDummyFifo(&s_ctrlParam, wrFunc, SSIOF_WR_FIFO_MAX);
			}
			/* setting operation status */
			s_ctrlParam.status = ( unsigned char )SSIOF_READ_DATA;
			/* intruppt enable */
			write_bit( SSIOF1->SF1INTC, 0x1F, 0x1A );

			break;

		/* Write(data send) mode        */
		case SSIOF_MODE_WR:
			if( opUnit == 0 ) {
				wrFunc = s_ssiof_writeSingleData_8;
			}
			else {
				wrFunc = s_ssiof_writeSingleData_16;
			}
			/* write to FIFO */
			s_ssiof_writeFifo( &s_ctrlParam, wrFunc, SSIOF_WR_FIFO_MAX );
			/* setting operation status */
			s_ctrlParam.status = SSIOF_WRITE_DATA;
			/* intruppt enable */
			write_bit( SSIOF1->SF1INTC, 0x1F, 0x15 );

			break;

		/* bi-direction (data receive / send) mode  */
		case SSIOF_MODE_RD_WR:
			if( opUnit == 0 ) {
				wrFunc = s_ssiof_writeSingleData_8;
			}
			else {
				wrFunc = s_ssiof_writeSingleData_16;
			}
			/* write to FIFO */
			/* [Note]dummy data is not required */
			s_ssiof_writeFifo( &s_ctrlParam, wrFunc, SSIOF_WR_FIFO_MAX );
			/* setting operation status */
			s_ctrlParam.status = SSIOF_READ_WRITE_DATA;
			/* intruppt enable */
			set_bit( SSIOF1->SF1INTC, 0x1F);

			break;

		default:
			/* can not start */
			return ( SSIOF_R_ERR );
	}

	/* --- Communication setting --- */
	/* Communication start */
	set_bit( SSIOF1->SF1CTRL, (1 << 0) );

	return ( SSIOF_R_OK );
}


/**
 * Process to continue communication
 *
 * @param   -
 * @retval  SSIOF_R_TRANS_FIN      : reading/writing communication is finished
 * @retval  SSIOF_R_TRANS_CONT_OK  : reading/writing communication is continued (success)
 */
int32_t ssiof1_continue( void )
{
	int             ret;
	void            (*rwFunc)( ssiofCtrlParam_t *param );
	unsigned short  status=0;
	unsigned char   flgTransEnd=0;
	unsigned char   opUnit;

	/* communication status */
	status = (unsigned short)ssiof1_getStatus();
	ssiof1_clearStatus( status );
	/* Unit of a reading/writing operation  */
	opUnit = ( unsigned char )get_bit( SSIOF1->SF1CTRL, (1 << 2) );

	/* check mode fault */
	if(( status & SFnSRR_SFnMDF ) != 0) {
		s_ctrlParam.errStat |= SFnSRR_SFnMDF;
		s_ctrlParam.status = SSIOF_COMMUNICATION_END;
	}

	/* operation status */
	switch( s_ctrlParam.status ) {
		/* --- Write(data send) mode ---    */
		case SSIOF_WRITE_DATA:
			if( s_ctrlParam.dataSize > s_ctrlParam.cnt ) {
				/* 8bit mode */
				if( opUnit == 0 ) {
					rwFunc = s_ssiof_writeSingleData_8;
				}
				/* 16bit mode */
				else {
					rwFunc = s_ssiof_writeSingleData_16;
				}
				/* write to FIFO */
				s_ssiof_writeFifo( &s_ctrlParam, rwFunc, ( unsigned char )( SSIOF_WR_FIFO_MAX - ssiof1_getWriteFifoSize() ) );
				s_ctrlParam.errStat |= ( status & SSIOF_WR_ERR_STATUS );
			}
			/* End of communication */
			else {
				if( (status & SFnSRR_SFnFI) != 0 ){
					s_ctrlParam.status = SSIOF_COMMUNICATION_END;
					flgTransEnd = ( unsigned char )0x1;
				}
			}
			break;

		/* --- Read(data receive) mode ---  */
		case SSIOF_READ_DATA:
			if( s_ctrlParam.dataSize > s_ctrlParam.cnt ) {
				/* 8bit mode */
				if( opUnit == 0 ) {
					rwFunc = s_ssiof_readSingleData_8;
				}
				/* 16bit mode */
				else {
					rwFunc = s_ssiof_readSingleData_16;
				}
				/* read from FIFO */
				s_ssiof_readFifo( &s_ctrlParam, rwFunc );
				s_ctrlParam.errStat |= ( status & SSIOF_RD_ERR_STATUS );

				/* End of communication */
				if( s_ctrlParam.cnt >= s_ctrlParam.dataSize  ) {
					s_ctrlParam.status = SSIOF_COMMUNICATION_END;
					flgTransEnd = ( unsigned char )0x1;
					break;
				}

				/* Master mode */
				if( get_bit(SSIOF1->SF1CTRL, (1 << 1)) == 1 ) {
					if( opUnit == 0 ) {
						rwFunc = s_ssiof_writeDummyData_8;
					}
					else {
						rwFunc = s_ssiof_writeDummyData_16;
					}
					/* write dummy data to FIFO         */
					s_ssiof_writeDummyFifo(&s_ctrlParam, rwFunc, ( unsigned char )( SSIOF_WR_FIFO_MAX - ssiof1_getWriteFifoSize()));
				}
			}
			/* End of communication */
			else {
				if( (status & SFnSRR_SFnSPIF) != 0 ){
					s_ctrlParam.status = SSIOF_COMMUNICATION_END;
					flgTransEnd = ( unsigned char )0x1;
				}
			}
			break;

		/* --- bi-direction (data receive / send) mode ---  */
		case SSIOF_READ_WRITE_DATA:
			/* receive interrupt */
			/* 8bit mode */
			if( opUnit == 0 ) {
				rwFunc = s_ssiof_readSingleDataNotCnt_8;
			}
			/* 16bit mode */
			else {
				rwFunc = s_ssiof_readSingleDataNotCnt_16;
			}
			/* read from FIFO */
			s_ssiof_readFifo( &s_ctrlParam, rwFunc );

			/* send interrupt */
			if( s_ctrlParam.dataSize > s_ctrlParam.cnt ) {
				/* 8bit mode */
				if( opUnit == 0 ) {
					rwFunc = s_ssiof_writeSingleData_8;
				}
				/* 16bit mode */
				else {
					rwFunc = s_ssiof_writeSingleData_16;
				}
				/* write to FIFO */
				s_ssiof_writeFifo( &s_ctrlParam, rwFunc, ( unsigned char )( SSIOF_WR_FIFO_MAX - ssiof1_getWriteFifoSize() ) );
			}
			/* End of communication */
			else {
				if( (status & SFnSRR_SFnFI) != 0 ){
					s_ctrlParam.status = SSIOF_COMMUNICATION_END;
					flgTransEnd = ( unsigned char )0x1;
				}
			}
			s_ctrlParam.errStat |= ( status & ( SSIOF_RD_ERR_STATUS | SSIOF_WR_ERR_STATUS ));

			break;

		/* Communication stop */
		case SSIOF_COMMUNICATION_END:
		default:
			flgTransEnd = ( unsigned char )0x1;
			break;
	}

	ret = (int)( SSIOF_R_TRANS_CONT_OK );
	/* If communication is completed, then callback function is called. */
	if( flgTransEnd != 0 ) {
		clear_reg32( SSIOF1->SF1INTC, 0x0000FFFF );
		if( s_ctrlParam.callBack != (void *)0 ) {
			s_ctrlParam.callBack( s_ctrlParam.cnt, s_ctrlParam.errStat );
		}
		ret = (int)( SSIOF_R_TRANS_FIN );
	}
	return ret;
}

/** @} */
/** @} */
