/*****************************************************************************
 uartf3.c

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
 * @file    uartf3.c
 *
 * This module is 'UARTF3' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup UARTF
 *  @{
 *
 *  @brief UART with FIFO register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "uartf3.h"
/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
#define UARTFn_WR_FIFO_MAX  ( 4 )               /**< Size of FIFO (Write)       */

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/
static uartfCtrlParam_t s_writeCtrlParam;   /**< write control parameters for UARTF */
static uartfCtrlParam_t s_readCtrlParam;    /**< read control parameters for UARTF  */

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/
static void           s_writeSingleData( uartfCtrlParam_t *param );
static void           s_writeFifo( uartfCtrlParam_t *param );
static uint16_t       s_readSingleData( uartfCtrlParam_t *param );
static uint16_t       s_readFifo( uartfCtrlParam_t *param );

/*############################################################################*/
/*#                              Subroutine                                  #*/
/*############################################################################*/
/**
 * Writing a data
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
__INLINE static void s_writeSingleData( uartfCtrlParam_t *param )
{
	uartf3_putc( *param->data );
	param->data++;
	param->cnt++;
}

/**
 * Writing data to FIFO
 *
 * @param[in,out]   *param  Pointer to control parameters
 * @return          None
 */
__INLINE static void s_writeFifo( uartfCtrlParam_t *param )
{
	uint8_t   wr_size;

	wr_size = param->blockSize;     /* size of next writing */
	if( (param->cnt + wr_size) > param->size ) {
		wr_size = (uint8_t)(param->size - param->cnt);
	}
	/* Write data (to FIFO) */
	while( uartf3_checkWriteBusy() != 0){}
	for( ; wr_size != 0; wr_size-- ) {
		s_writeSingleData( param );
	}
}

/**
 * Reading a data
 *
 * @param[in,out]   *param Pointer to control parameters
 * @return          Status (the value of UAF0LSR register)
 */
__INLINE static uint16_t s_readSingleData( uartfCtrlParam_t *param )
{
	uint16_t  status;

	status = (uint16_t)uartf3_getStatus();
	*param->data = (uint8_t)uartf3_getc();
	param->data++;
	param->cnt++;
	return status;
}

/**
 * Reading data from FIFO
 *
 * @param[in,out]   *param Pointer to control parameters
 * @return          Status (the value of UAF0LSR register)      <br>
 *                  Status is logical sum of all reception data in FIFO.
 */
__INLINE static uint16_t s_readFifo( uartfCtrlParam_t *param )
{
	uint16_t  status;
	uint8_t   rd_size;

	status = 0;
	rd_size = param->blockSize;     /* size of reading */
	if( (param->cnt + rd_size) > param->size ) {
		rd_size = (uint8_t)(param->size - param->cnt);
	}
	/* Read data (from FIFO) */
	for( ; rd_size != 0; rd_size-- ) {
		if ( uartf3_checkReadReady() == 1) {
			status |= s_readSingleData( param );
		}
	}
	return status;
}

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize UARTF
 *
 * @param[in]   uafnmod     Configuration UART mode                             <br>
 *                          (Setting value of UAF0MOD)                          <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Data length             : UARTF_LG_5BIT, or UARTF_LG_6BIT, or UARTF_LG_7BIT, or UARTF_LG_8BIT
 * @arg                         Stop bit                : UARTF_STP_1BIT, or UARTF_STP_2BIT
 * @arg                         Parity bit              : UARTF_PT_NON, or UARTF_PT_ODD, or UARTF_PT_EVEN, or UARTF_PT_FIXED_1, or UARTF_PT_FIXED_0
 * @arg                         Enable 'break control'  : UARTF_BC_DIS, or UARTF_BC_ENA
 * @arg                         UAFnBUF access          : UARTF_DLAB_RBR_THR, or UARTF_DLAB_DLR
 * @arg                         FIFO data (read)        : UARTF_RFR_KEEP, or UARTF_RFR_CLR
 * @arg                         FIFO data (write)       : UARTF_TFR_KEEP, or UARTF_TFR_CLR
 * @arg                         Read FIFO trigger level : UARTF_FTL_1BYTE, or UARTF_FTL_2BYTE, or UARTF_FTL_3BYTE, or UARTF_FTL_4BYTE
 * 
 * @param[in]   uafncaj         Clock adjustment                                 <br>
 *                              (Setting value of UAF0CAJ)                       <br>
 *                              Specify the logical sum of the following items.
 * @arg                         Adjustment mode                    : UARTF_RMV_DIS, or UARTF_RMV_ENA
 * @arg                         Clock adjustment value
 
 * @param[in]   brDivisorLatch  Divisor latch value    (for baud-rate generator) <br>
 *                              (set to 0, or greater than 2)
 *
 * @return      None
 * 
 * @note
 * @arg         About baud-rate clock calculation                               <br>
 *                  Baud-rate clock = 'Sysytem clock' * ( ('uafncaj' - 1) / 'uafncaj' ) / ('brDivisorLatch' * 16)
 *
 * @arg         About system clock frequency                                    <br>
 *                  If you watnt to use UARTF, Set sysytem clock over 4MHz.
 *
 * @arg         About using FIFO                                                <br>
 *                  This module is FIFO-mode only. You cannot disable FIFO.
 *
 * @arg         About interrupt                                                 <br>
 *                  Interrupt is prohibited except during communication.
 *
 */
void uartf3_init( uint16_t uafnmod, uint16_t uafncaj, uint16_t brDivisorLatch )
{
	/*=== Register setting ===*/
	/*---   Baud rate clock setting   ---*/
	set_bit( UARTF3->UAF0MOD, (1 << 7) );               /* enable DLR(Divisor Latch Resister) access */
	write_reg32( UARTF3->UAF0BUF, brDivisorLatch );     /* set DLR */
	clear_bit( UARTF3->UAF0MOD, (1 << 7) );             /* disable DLR(Divisor Latch Resister) access */
	write_reg32( UARTF3->UAF0CAJ, uafncaj );            /* set adjustment mode and value */

	/*---   Communication setting   ---*/
	uafnmod |= UAFnMOD_UFnFEN;                  /* enable FIFO */
	write_reg32( UARTF3->UAF0MOD, uafnmod );
	write_reg32( UARTF3->UAF0IER, (uint16_t)(UARTF_ERBFI_DIS | UARTF_ETBEI_DIS | UARTF_ELSI_DIS) );
	s_writeCtrlParam.blockSize    = (uint8_t)UARTFn_WR_FIFO_MAX;
	s_readCtrlParam.blockSize     = (uint8_t)( (read_reg32( UARTF3->UAF0MOD ) >> 12) & 0x3 );
	s_readCtrlParam.blockSize++;

	/*---   Communication status is clear   ---*/
	uartf3_clearReadFifo();                     /* clear Read FIFO */
	uartf3_clearWriteFifo();                    /* clear Write FIFO */
	uartf3_getStatus();      /* clear UF0OER,UF0PER,UF0FER */
	uartf3_getIntCause();    /* clear write request */
}

/**
 * Start to sending write data
 *
 * @param[in]   *data   pointer to area where the send data is stored
 * @param[in]   size    size of data (unit is byte)
 * @param[in]   *func   pointer to callback function
 * @return      None
 * @note        Transmit FIFO is an empty state, but there is the case that all transmit        <br>
 *              processing doesn't complete. Confirm that transmit shift register (TSR) became  <br>
 *              empty in UF0TEMT bit of UAF0LSR register before stopping high-speed clock       <br>
 *              (Transition to modes such as STOP / DEEP-HALT / HALT-H).
 */
void uartf3_write( uint8_t *data, uint32_t size, cbfUartF_t func )
{
	/*=== Transmission of a message system order parameter setting. ===*/
	s_writeCtrlParam.data      = data;
	s_writeCtrlParam.size      = size;
	s_writeCtrlParam.cnt       = 0;
	s_writeCtrlParam.callBack  = func;
	s_writeCtrlParam.errStat   = 0;

	/*=== transmit a message, and it is worked to start. ===*/
	/* Write next data block */
	s_writeFifo( &s_writeCtrlParam );

	/* Interrupt enable */
	set_reg32( UARTF3->UAF0IER, UARTF_ETBEI_ENA);
}

/**
 * Start to receiving read data
 *
 * @param[in]   *data   pointer to area where the receive data is stored
 * @param[in]   size    receive buffer size (the unit is byte)
 * @param[in]   *func   pointer to callback function
 * @return      None
 */
void uartf3_read( uint8_t *data, uint32_t size, cbfUartF_t func )
{
	/*===   Clear status    ===*/
	uartf3_getStatus();                /* clear UF0OER,UF0PER,UF0FER */

	/*=== Transmission of a message system order parameter setting. ===*/
	s_readCtrlParam.data       = data;
	s_readCtrlParam.size       = size;
	s_readCtrlParam.cnt        = 0;
	s_readCtrlParam.callBack   = func;
	s_readCtrlParam.errStat    = 0;

	/* Interrupt enable */
	set_reg32( UARTF3->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
}

/**
 * Process to continue writing
 *
 * @param[in] intStatus  interrupt status(UARTF_IRID_WRITE_REQ, UARTF_IRID_TRANS_COMP)
 * @retval  UARTF_R_TRANS_FIN(=1)      : writing communication is finished
 * @retval  UARTF_R_TRANS_CONT_OK(=0)  : writing communication is continued (success)
 */
int32_t uartf3_continueWrite( uint16_t intStatus )
{
	int32_t ret = UARTF_R_TRANS_CONT_OK;

	intStatus = intStatus & UARTF_IRID_MASK;
	if ( (intStatus != UARTF_IRID_WRITE_REQ) && (intStatus != UARTF_IRID_TRANS_COMP) ) {
		return UARTF_R_TRANS_CONT_OK;
	}
	/*---   Are transmission of a message data left?    ---*/
	if( s_writeCtrlParam.size > s_writeCtrlParam.cnt ){
		/*=== I transmit a message, and it is worked to continue. ===*/
		/*--- There are data in the transmission of a message buffer? ---*/
		/* Write next data block */
		s_writeFifo( &s_writeCtrlParam );
		ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
	}
	else{
		/*=== It is returned that the transmission ended. ===*/
		if(intStatus == UARTF_IRID_WRITE_REQ){
			clear_reg32( UARTF3->UAF0IER, UARTF_ETBEI_ENA);
			set_reg32( UARTF3->UAF0IER, UARTF_TEMTI_ENA);
			ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
		}
		else{
			clear_reg32( UARTF3->UAF0IER, (UARTF_ETBEI_ENA | UARTF_TEMTI_ENA));

			/* Callback */
			if( s_writeCtrlParam.callBack != (void *)0 ){
				s_writeCtrlParam.callBack( s_writeCtrlParam.cnt, s_writeCtrlParam.errStat );
			}
			ret = (int32_t)( UARTF_R_TRANS_FIN );
		}
	}
	return ret;
}

/**
 * Process to continue reading
 * 
 * @param   -
 * @retval  UARTF_R_TRANS_FIN(=1)     : reading communication is finished
 * @retval  UARTF_R_TRANS_CONT_OK(=0) : reading communication is continued (success)
 */
int32_t uartf3_continueRead( void )
{
	int32_t   ret;
	uint16_t  status;
	uint8_t   flgReadEnd=0;

	status = (uint16_t)uartf3_getStatus();
	/*---   Are reception data left?    ---*/
	if( s_readCtrlParam.size > s_readCtrlParam.cnt ){
		/* Getting reception data */
		/* Read next data block */
		status |= s_readFifo( &s_readCtrlParam );
		s_readCtrlParam.errStat |= status & (uint16_t)(UAFnLSR_UFnFER|UAFnLSR_UFnOER|UAFnLSR_UFnPER|UAFnLSR_UFnBI);

		/* End of communication */
		if( s_readCtrlParam.size <= s_readCtrlParam.cnt ){
			flgReadEnd = 1;
		}
		/*---   Frame Error ---*/
		else if( ( status & ( uint16_t )( UAFnLSR_UFnFER ) ) != 0 ) {
			/* This program execute callback and return UARTF_R_TRANS_FIN, if "frame error" occurs. */
			flgReadEnd = 1;
		}
	}
	/* overflow */
	else {
		s_readCtrlParam.errStat |= status;
		/* FIFO reset */ 
		uartf3_clearReadFifo();
		flgReadEnd = 1;
	}

	ret = (int32_t)( UARTF_R_TRANS_CONT_OK );
	/* If communication is completed, then callback function is called. */
	if( flgReadEnd != 0 ){
		/* interrupt disable */
		clear_reg32( UARTF3->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );

		ret = (int32_t)( UARTF_R_TRANS_FIN );
		/* Callback */
		if( s_readCtrlParam.callBack != (void *)0 ){
			s_readCtrlParam.callBack( s_readCtrlParam.cnt, s_readCtrlParam.errStat );
		}
	}
	return ret;
}

/**
 * Stop transmission of UARTF
 *
 * @param       -
 * @return      None
 */
void uartf3_stopWrite( void )
{
	/* Disable interrupts */
	clear_reg32( UARTF3->UAF0IER, (UARTF_ETBEI_ENA | UARTF_TEMTI_ENA));
	/* Clear FIFO */
	uartf3_clearWriteFifo();
	/* Init parameters */
	s_writeCtrlParam.data      = (void*)0;
	s_writeCtrlParam.size      = 0;
	s_writeCtrlParam.cnt       = 0;
	s_writeCtrlParam.callBack  = (void*)0;
	s_writeCtrlParam.errStat   = 0;
}

/**
 * Stop reception of UARTF
 *
 * @param       -
 * @return      None
 */
void uartf3_stopRead( void )
{
	/* Disable interrupts */
	clear_reg32( UARTF3->UAF0IER, (UARTF_ERBFI_ENA | UARTF_ELSI_ENA) );
	/* Clear FIFO */
	uartf3_clearReadFifo();
	/* Init parameters */
	s_readCtrlParam.data       = (void*)0;
	s_readCtrlParam.size       = 0;
	s_readCtrlParam.cnt        = 0;
	s_readCtrlParam.callBack   = (void*)0;
	s_readCtrlParam.errStat    = 0;
	/* Clear error status */
	uartf3_getStatus(); /* clear UF0OER,UF0PER,UF0FER */
}

/** @} */
/** @} */
