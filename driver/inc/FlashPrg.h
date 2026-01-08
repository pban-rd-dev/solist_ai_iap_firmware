/*****************************************************************************
 FlashPrg.h

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
 * @file    FlashPrg.h
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

#ifndef FLASHPRG_H__
#define FLASHPRG_H__


#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/* Return for operation success or failure */
#define FLASH_OK                          ( 0 )           /**< Write or erase operation was processed successfully */
#define FLASH_BUSY_ERR                    ( 1 )           /**< Write or erase operation was not processed because of ongoing flash operation */
#define FLASH_NG                          ( -1 )          /**< Write or erase operation was processed failure */

#define FLASH_R_OK                        ( 0 )           /**< Write or erase operation was processed successfully */
#define FLASH_R_NG                        ( 1 )           /**< Write or erase operation was processed failure */


/* Return for access check of the flash memory */
#define FLASH_MEMORY_ACCESSIBLE           ( 0x0U )        /**< No operation in flash memory - flash memory is accessible */
#define FLASH_MEMORY_ERASING              ( 0x1U )        /**< Ongoing erasing operation in Data flash memory */
#define FLASH_MEMORY_WRITING              ( 0x2U )        /**< Ongoing writing operation in Data flash memory */
#define FLASH_PROGRAM_ERASING             ( 0x4U )        /**< Ongoing erasing operation in Program flash memory */
#define FLASH_PROGRAM_WRITING             ( 0x8U )        /**< Ongoing writing operation in Program flash memory */

/* === mask for addresses === */
/* data address */
#define FLASH_DATA_SECTOR_ADDR_MASK       ( 0xFFFFFF00UL )    /**< Address mask constant: data memory sector erase 256 bytes */
#define FLASH_DATA_BLOCK_ADDR_MASK        ( 0xFFFFE000UL )    /**< Address mask constant: data memory block erase 8K bytes */
#define FLASH_PRGM_SECTOR_ADDR_MASK       ( 0xFFFFF800UL )    /**< Address mask constant: program memory sector erase 2K bytes */
#define FLASH_PRGM_BLOCK_ADDR_MASK        ( 0xFFFF8000UL )    /**< Address mask constant: program memory block erase 32K bytes */


/**
 * Checks if the flash memory is accessible or not
 *
 * @param       -
 * @retval      FLASH_MEMORY_ACCESSIBLE   Flash memory is accessible 
 * @retval      FLASH_MEMORY_ERASING      Ongoing erase in flash memory(data flash memory),flash memory is not accessible 
 * @retval      FLASH_MEMORY_WRITING      Ongoing write in flash memory(data flash memory),flash memory is not accessible
 * @retval      FLASH_PROGRAM_ERASING     Ongoing erase in flash memory(program flash memory),flash memory is not accessible 
 * @retval      FLASH_PROGRAM_WRITING     Ongoing write in flash memory(program flash memory),flash memory is not accessible
 */
#define flash_checkFlashAccess()          read_reg8(FLASHCNT->FLASHSTA)


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

void            flash_init( void );
int8_t          flash_open( void );
int8_t          flash_close( void );
uint32_t        flash_getFacp1( void );
uint32_t        flash_getFacp2( void );
uint32_t        flash_getFselfEn( void );
uint32_t        flash_getFselfDis( void );
int8_t          flash_eraseBlockDataMemory( void  *addr, uint32_t facp1, uint32_t facp2 );
int8_t          flash_eraseBlockProgramMemory( void  *addr, uint32_t facp1, uint32_t facp2 );
int8_t          flash_eraseSectorDataMemory( void  *addr, uint32_t facp1, uint32_t facp2 );
int8_t          flash_eraseSectorProgramMemory( void  *addr, uint32_t facp1, uint32_t facp2 );
int8_t          flash_writeDataMemory( void  *addr, uint8_t data, uint32_t facp1, uint32_t facp2 );
int8_t          flash_writeProgramMemory( void  *addr, uint32_t data, uint32_t facp1, uint32_t facp2 );
int8_t          flash_controlSelfProg( uint32_t fselfFlag );


#endif	/* FLASHPRG_H__ */

/** @} */
/** @} */
