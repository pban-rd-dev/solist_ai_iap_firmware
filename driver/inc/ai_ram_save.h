/*****************************************************************************
 ai_ram_save.h

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
 * @file    ai_ram_save.h
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

#ifndef AI_RAM_SAVE_H__
#define AI_RAM_SAVE_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== API return value ===*/
#define AIRAM_OK                ( 0 )                               /**< Return Value: Writing flash or Erasing flash is OK     */
#define AIRAM_BUSY_ERR          ( 1 )                               /**< Return Value: Writing flash or Erasing flash is Busy   */
#define AIRAM_ADDR_ERR          ( 2 )                               /**< Return Value: Address Error                            */
#define AIRAM_NG                ( -1 )                              /**< Return Value: Writing flash or Erasing flash is Error  */


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
int8_t airam_save_Beta( void *dst, uint8_t instance, uint32_t offset );
int8_t airam_save_P( void *dst, uint8_t instance, uint32_t offset );
int8_t airam_restore_Beta( void *src, uint8_t instance, uint32_t offset );
int8_t airam_restore_P( void *src, uint8_t instance, uint32_t offset );

#endif /*AI_RAM_SAVE_H__*/

/** @} */
/** @} */
