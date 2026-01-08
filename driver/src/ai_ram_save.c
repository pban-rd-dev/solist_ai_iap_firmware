/*****************************************************************************
 ai_ram_save.c

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
 * @file    ai_ram_save.c
 *
 * This module is 'Save and Restore AI RAM data' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup AI-RAM_Save
 *  @{
 *
 *  @brief Save and Restore AI RAM data
 *
 *  @file
 */

#include "mcu.h"
#include "rdwr_reg.h"
#include "cmp_common.h"
#include "ai_ram_save.h"
#include "solistAi.h"
#include "FlashPrg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                Variable                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                               Prototype                                  #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Store weight Beta in program memory
 * 4 bytes are copied
 *
 * @param[in]   *dst      Write destination address in program memory
 * @param[in]   instance  Index of AI model
 * @param[in]   offset    Offset address from the beginning of the weight data
 * @retval      AIRAM_OK            : Data write was processed 
 * @retval      AIRAM_BUSY_ERR      : Data write was not processed
 * @retval      AIRAM_ADDR_ERR      : Address error
 * @retval      AIRAM_NG            : Flash access failed
 */
int8_t airam_save_Beta( void *dst, uint8_t instance, uint32_t offset )
{
	uint32_t data = 0U;
	uint32_t to_addr = (uint32_t)dst;
	int8_t flash_ret = (int8_t)0;
	
	if( (to_addr % 4U) != 0U ) {
		return (int8_t)AIRAM_ADDR_ERR;
	}
	
	if( (to_addr & (uint32_t)0x7FF) == 0U ) {
		flash_ret = flash_eraseSectorProgramMemory( dst, flash_getFacp1(), flash_getFacp2() );
		
		if( flash_ret == FLASH_BUSY_ERR ) {
			return (int8_t)AIRAM_BUSY_ERR;
		}
		if( flash_ret == FLASH_NG ) {
			return (int8_t)AIRAM_NG;
		}
	}
	
	ODL_GetWeightBeta( (void*)&data, instance, offset, (uint16_t)4U );
	
	flash_ret = flash_writeProgramMemory( dst, data, flash_getFacp1(), flash_getFacp2() );
	
	if( flash_ret == FLASH_BUSY_ERR ) {
		return (int8_t)AIRAM_BUSY_ERR;
	}
	if( flash_ret == FLASH_NG ) {
		return (int8_t)AIRAM_NG;
	}
	
    return (int8_t)AIRAM_OK;
}

/**
 * Store weight P in program memory
 * 4 bytes are copied
 *
 * @param[in]   *dst      Write destination address in program memory
 * @param[in]   instance  Index of AI model
 * @param[in]   offset    Offset address from the beginning of the weight data
 * @retval      AIRAM_OK            : Data write was processed 
 * @retval      AIRAM_BUSY_ERR      : Data write was not processed
 * @retval      AIRAM_ADDR_ERR      : Address error
 * @retval      AIRAM_NG            : Flash access failed
 */
int8_t airam_save_P( void *dst, uint8_t instance, uint32_t offset )
{
	uint32_t data = 0U;
	uint32_t to_addr = (uint32_t)dst;
	int8_t flash_ret = (int8_t)0;
	
	if( (to_addr % 4U) != 0U ) {
		return (int8_t)AIRAM_ADDR_ERR;
	}
	
	if( (to_addr & (uint32_t)0x7FF) == 0U ) {
		flash_ret = flash_eraseSectorProgramMemory( dst, flash_getFacp1(), flash_getFacp2() );
		
		if(flash_ret == FLASH_BUSY_ERR) {
			return (int8_t)AIRAM_BUSY_ERR;
		}
		if(flash_ret == FLASH_NG) {
			return (int8_t)AIRAM_NG;
		}
	}
	
	ODL_GetWeightP( (void*)&data, instance, offset, (uint16_t)4U );
	
	flash_ret = flash_writeProgramMemory( dst, data, flash_getFacp1(), flash_getFacp2() );
	
	if( flash_ret == FLASH_BUSY_ERR ) {
		return (int8_t)AIRAM_BUSY_ERR;
	}
	if( flash_ret == FLASH_NG ) {
		return (int8_t)AIRAM_NG;
	}
	
    return (int8_t)AIRAM_OK;
}

/**
 * Restore the weight Beta to AI memory
 * 4 bytes are copied
 *
 * @param[in]   *src      The address of the source in the program memory
 * @param[in]   instance  Index of AI model
 * @param[in]   offset    Offset address from the beginning of the weight data
 * @retval      AIRAM_OK            : Restore was processed 
 * @retval      AIRAM_ADDR_ERR      : Address error
 */
int8_t airam_restore_Beta( void *src, uint8_t instance, uint32_t offset )
{
	uint32_t data = 0U;
	uint32_t from_addr = (uint32_t)src;
	
	if((from_addr % 4U) != 0U) {
		return (int8_t)AIRAM_ADDR_ERR;
	}
	
	data = *(uint32_t*)from_addr;
	
	ODL_SetWeightBeta( (void*)&data, instance, offset, (uint16_t)4U );
	
	
    return (int8_t)AIRAM_OK;
}

/**
 * Restore the weight P to AI memory
 * 4 bytes are copied
 *
 * @param[in]   *src      The address of the source in the program memory
 * @param[in]   instance  Index of AI model
 * @param[in]   offset    Offset address from the beginning of the weight data
 * @retval      AIRAM_OK            : Restore was processed 
 * @retval      AIRAM_ADDR_ERR      : Address error
 */
int8_t airam_restore_P( void *src, uint8_t instance, uint32_t offset )
{
	uint32_t data = 0U;
	uint32_t from_addr = (uint32_t)src;
	
	if((from_addr % 4U) != 0U) {
		return (int8_t)AIRAM_ADDR_ERR;
	}
	
	data = *(uint32_t*)from_addr;
	
	ODL_SetWeightP( (void*)&data, instance, offset, (uint16_t)4U );
	
	
    return (int8_t)AIRAM_OK;
}

/** @} */
/** @} */
