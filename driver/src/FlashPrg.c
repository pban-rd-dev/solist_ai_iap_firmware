/*****************************************************************************
 FlashPrg.c

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
 * @file    FlashPrg.c
 *
 * This module is 'FlashPrg' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup FlashPrg
 *  @{
 *
 *  @brief Flash memory register related
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "FlashPrg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
#define FLC_ACP_lock                    ( 0x00 )                /**< flash acceptor lock Data */
#define FLC_ACP_1st                     ( 0xA5 )                /**< flash acceptor 1st Data */
#define FLC_ACP_2nd                     ( 0x5A )                /**< flash acceptor 2nd Data */
#define FLC_ERA_BLOCK_ERASE             ( 0x01 )                /**< FLCERA Register FLE[1:0] = 01 */
#define FLC_ERA_SECTOR_ERASE            ( 0x02 )                /**< FLCERA Register FLE[1:0] = 10 */
#define FLC_PROGRAMFLASH_MSK            ( 0xF8000000 )          /**< The mask value of judgement flash area */
#define FLC_PROGRAMFLASH_AREA           ( 0x10000000 )          /**< The address value of program area */


/* === flash programming command === */
#define FLASH_ENABLE_SELF_PROGRAMMING   ( 0xA5A5A5A5UL )        /**< Expected value for enabling flash memory programming */
#define FLASH_DISABLE_SELF_PROGRAMMING  ( 0x5A5A5A5AUL )        /**< Expected value for disabling flash memory programming */

/* === accepter flags === */
#define FLASH_ACCEPTER_FLAG1_INIT       ( 0x003CC300UL )        /**< Initial value for the flash flag variable 1 */
#define FLASH_ACCEPTER_FLAG2_INIT       ( 0x00699600UL )        /**< Initial value for the flash flag variable 2 */
#define FLASH_ACCEPTER_FLAG1_SET        ( 0x3CC33CFAUL )        /**< Expected value for the flash flag variable 1 */
#define FLASH_ACCEPTER_FLAG2_SET        ( 0x699669F5UL )        /**< Expected value for the flash flag variable 2 */

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/

static uint32_t s_facp1;          /**< flash flag variable 1 */
static uint32_t s_facp2;          /**< flash flag variable 2 */
static uint32_t s_fself_en;       /**< flash control variable for enabling flash programming */
static uint32_t s_fself_dis;      /**< flash control variable for disabling flash programming */

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

static void flash_error( void );

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Flash error routine.  
 *
 * @param       -
 * @return      None
 */
static void flash_error( void )
{
	/* disable flash programming */
	write_reg8( FLASHCNT->FLASHSLF, 0x00U );

	/* reset FLASH Accepter */
	write_reg8( FLASHCNT->FLASHACP, 0x00U );

	s_facp1 = 0x00000000UL;
	s_facp2 = 0x00000000UL;
	s_fself_en = 0x00000000UL;
	s_fself_dis = 0x00000000UL;
}

/**
 * Control Flash Self Programming 
 *
 * Enable or disable the erase and 
 * write protection to the flash
 * memory if argument is valid.
 * Invokes flash_error if argument is invalid.
 *
 * @param[in]   fselfFlag       Enable or disable to erase and write.
 * @retval      FLASH_OK            : Flash Access successed
 * @retval      FLASH_NG            : Flash Access Failed 
 *
 */
int8_t flash_controlSelfProg( uint32_t fselfFlag )
{
	int8_t ret = FLASH_OK;

	if((fselfFlag==(uint32_t) FLASH_ENABLE_SELF_PROGRAMMING)||(fselfFlag==(uint32_t) FLASH_DISABLE_SELF_PROGRAMMING )){
		write_reg8( FLASHCNT->FLASHSLF, (uint8_t)(fselfFlag & (uint32_t)0x01U));
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Flash control and flag variables initialization
 *
 * Initialize flash flag and control variables
 *
 * @param       -
 * @return      None
 * @note        Call this function before invoking functions flash_open.
 *
 */
void flash_init( void )
{
	s_facp1 = FLASH_ACCEPTER_FLAG1_INIT;
	s_facp2 = FLASH_ACCEPTER_FLAG2_INIT;
	s_fself_en = 0x00000000UL;
	s_fself_dis = 0x00000000UL;
}

/**
 * Set flash control information
 *
 * Checks if the flash flag and control
 * variables have correct values. If values are correct,
 * control variables and flag variables are set.
 * Otherwise, flash_error is invoked.
 *
 * @param       -
 * @retval      FLASH_OK        : Flash Access successed
 * @retval      FLASH_NG        : Flash Access Failed 
 * @note        Call this function before the command
 *              for enabling flash write and erase operation.
 *              Call flash_init before calling this
 *              function for successful operation.
 *
 */
int8_t flash_open( void )
{
	int8_t ret = FLASH_OK;

	if((s_facp1==(uint32_t)FLASH_ACCEPTER_FLAG1_INIT)&&(s_facp2==(uint32_t)FLASH_ACCEPTER_FLAG2_INIT)&&
	   (s_fself_en==(uint32_t)0x00000000U)&&(s_fself_dis==(uint32_t)0x00000000U)){
		s_facp1 = FLASH_ACCEPTER_FLAG1_SET;
		s_facp2 = FLASH_ACCEPTER_FLAG2_SET;
		s_fself_en = FLASH_ENABLE_SELF_PROGRAMMING;
		s_fself_dis = FLASH_DISABLE_SELF_PROGRAMMING;
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Reset flash control information
 *
 * Checks if the flash flag and control
 * variables have correct values. 
 * If values are correct, control variables
 * and flag variables are reset.
 * Otherwise, flash_error is invoked.
 *
 * @param       -
 * @retval      FLASH_OK        : Flash Access successed
 * @retval      FLASH_NG        : Flash Access Failed 
 * @note        Call this function before the command
 *              for disabling flash write and erase operation.
 *              Call flash_open before calling 
 *              this function for successful operation. 
 *
 */
int8_t flash_close( void )
{
	int8_t ret = FLASH_OK;

	if((s_facp1==(uint32_t)FLASH_ACCEPTER_FLAG1_SET)&&(s_facp2==(uint32_t)FLASH_ACCEPTER_FLAG2_SET)&&
	  (s_fself_en==(uint32_t)FLASH_ENABLE_SELF_PROGRAMMING)&&(s_fself_dis==(uint32_t)FLASH_DISABLE_SELF_PROGRAMMING)){
		s_facp1 = FLASH_ACCEPTER_FLAG1_INIT;
		s_facp2 = FLASH_ACCEPTER_FLAG2_INIT;
		s_fself_en = 0x00000000UL;
		s_fself_dis = 0x00000000UL;
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Get flash flag variable 1 value
 *
 * Returns the value of the flash flag variable 1 
 *
 * @param       -
 * @return      s_facp1        : value of flash flag variable 1 
 *
 */
uint32_t flash_getFacp1( void )
{
	return s_facp1;
}

/**
 * Get flash flag variable 2 value
 *
 * Returns the value of the flash flag variable 2 
 *
 * @param       -
 * @return      s_facp2        : value of flash flag variable 2 
 *
 */
uint32_t flash_getFacp2( void )
{
	return s_facp2;
}

/**
 * Get flash control variable for 
 * enabling flash memory programming
 *
 * Returns the value for enabling flash memory programming 
 *
 * @param       -
 * @return      s_fself_en : value of flash control variable for enabling flash programming 
 *
 */
uint32_t   flash_getFselfEn( void )
{
	return s_fself_en;
}

/**
 * Get flash control variable for
 * disabling flash memory programming
 *
 * Returns the value for disabling flash memory programming
 *
 * @param       -
 * @return      s_fself_dis : value of flash control variable for disabling flash programming 
 *
 */
uint32_t    flash_getFselfDis( void )
{
	return s_fself_dis;
}

/**
 * Block Erase Flash Data Memory
 *
 * Erase the flash memory space by a block
 * according to the address parameter if flash flag variables 1 and 2 have correct values.
 * If flash control variables 1 and 2 have incorrect values,
 * flash_error function is called.
 * Returns a flag to indicate success or failure of erase.
 *
 * @param[in]   *addr  Erase Block Address 
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : block erase was processed 
 * @retval      FLASH_BUSY_ERR      : block erase was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init,
 *              flash_open and enable self programming, in order, before
 *              calling this function.
 *
 */
int8_t flash_eraseBlockDataMemory( void  *addr, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){

		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {

			/* set FLASH Accepter */
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp1&0xFFUL));
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, ((uint32_t)addr & FLASH_DATA_BLOCK_ADDR_MASK) );

			/* start FLASH Block Erase */
			write_reg32( FLASHCNT->FLASHCON, FLC_ERA_BLOCK_ERASE );

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Block Erase Program Memory
 *
 * Erase the program memory space by a block according to the address parameter
 * if flash flag variables 1 and 2 have correct values.                   
 * If flash control variables 1 and 2 have incorrect values,
 * flash_error function is called.                          
 *
 * @param[in]   *addr  Erase Block Address 
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : block erase was processed 
 * @retval      FLASH_BUSY_ERR      : block erase was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init, flash_open
 *              and enable self programming, in order, before 
 *              calling this function.
 *
 */
int8_t flash_eraseBlockProgramMemory( void *addr, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){
		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {
		
			/* set FLASH Accepter */
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp1&0xFFUL));
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, ((uint32_t)addr & FLASH_PRGM_BLOCK_ADDR_MASK) );

			/* start FLASH Block Erase */
			write_reg32( FLASHCNT->FLASHCON, FLC_ERA_BLOCK_ERASE );

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Sector Erase Flash Data Memory
 *
 * Erase the flash memory space by a sector according to the address parameter
 * if flash flag variables 1 and 2 have correct values.                   
 * If flash control variables 1 and 2 have incorrect values,
 * flash_error function is called.
 * Returns a flag to indicate success or failure of erase.
 * 
 * @param[in]   *addr  Erase Sector Address
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : sector erase was processed 
 * @retval      FLASH_BUSY_ERR      : sector erase was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init, flash_open
 *              self programming, in order, before
 *              calling this function.
 *
 */
int8_t flash_eraseSectorDataMemory( void *addr, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){

		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {

			/* set FLASH Accepter */
			write_reg8( FLASHCNT->FLASHACP, (uint8_t)(facp1&0xFFUL));
			write_reg8( FLASHCNT->FLASHACP, (uint8_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, ((uint32_t)addr & FLASH_DATA_SECTOR_ADDR_MASK) );

			/* start FLASH Sector Erase */
			write_reg8( FLASHCNT->FLASHCON, FLC_ERA_SECTOR_ERASE );

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Sector Erase Program Memory
 *
 * Erase the program memory space by a sector according to the address parameter
 * if flash flag variables 1 and 2 have correct values. 
 * If flash control variables 1 and 2 have incorrect values, flash_error function is called.
 * 
 * @param[in]   *addr  Erase Sector Address
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : sector erase was processed 
 * @retval      FLASH_BUSY_ERR      : sector erase was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init, flash_open and enable 
 *              self programming, in order, before
 *              calling this function.
 *
 */
int8_t flash_eraseSectorProgramMemory( void *addr, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){
		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {
		
			/* set FLASH Accepter */
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp1&0xFFUL));
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, ((uint32_t)addr & FLASH_PRGM_SECTOR_ADDR_MASK) );

			/* start FLASH Sector Erase */
			write_reg32( FLASHCNT->FLASHCON, FLC_ERA_SECTOR_ERASE );

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Write Flash Data Memory (1 byte)
 * Write the one byte data to the flash memory
 * if the flash flag variables have correct values.
 * If flash flag variables have incorrect values, flash_error is called.
 * Returns a flag to indicate success or failure of write.
 * 
 * @param[in]   *addr  Write Address
 * @param[in]   data   Write Data
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : flash write was processed 
 * @retval      FLASH_BUSY_ERR      : flash write was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init, flash_open and enable
 *              self programming, in order, before
 *              calling this function.
 *
 */
int8_t flash_writeDataMemory( void *addr, uint8_t data, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){

		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {

			/* set FLASH Accepter */
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp1&0xFFUL));
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, (uint32_t)addr );

			/* Write Data */
			write_reg32( FLASHCNT->FLASHD, (uint32_t)data );

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/**
 * Write Program Memory (4 bytes)
 *
 * Write the four bytes data to the program memory 
 * if the flash flag variables have correct values.
 * If flash flag variables have incorrect values,  
 * flash_error is called.                          
 * 
 * @param[in]   *addr  Write Address
 * @param[in]   data   Write Data
 * @param[in]   facp1  flash flag variable 1
 * @param[in]   facp2  flash flag variable 2
 * @retval      FLASH_OK            : flash write was processed 
 * @retval      FLASH_BUSY_ERR      : flash write was not processed
 * @retval      FLASH_NG            : Flash Access Failed 
 * @note        Call functions flash_init, flash_open and 
 *              enable self programming, in order, before 
 *              calling this function.
 *
 */
int8_t flash_writeProgramMemory( void *addr, uint32_t data, uint32_t facp1, uint32_t facp2 )
{
	int8_t ret = FLASH_OK;

	if((facp1 == (uint32_t) FLASH_ACCEPTER_FLAG1_SET )&&(facp2 == (uint32_t) FLASH_ACCEPTER_FLAG2_SET )){
		if(flash_checkFlashAccess()!=FLASH_MEMORY_ACCESSIBLE){
			ret = FLASH_BUSY_ERR;
		} else {

			/* set FLASH Accepter */
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp1&0xFFUL));
			write_reg32( FLASHCNT->FLASHACP, (uint32_t)(facp2&0xFFUL));

			/* set FLASH Address */
			write_reg32( FLASHCNT->FLASHA, (uint32_t)addr );

			/* Write Data */
			write_reg32( FLASHCNT->FLASHD, (uint32_t)data);

			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
	} else{
		flash_error();
		ret = FLASH_NG;
	}
	return ret;
}

/** @} */
/** @} */
