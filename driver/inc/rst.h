/*****************************************************************************
 rst.h

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
 * @file    rst.h
 *
 * This module is 'Reset' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Reset
 *  @{
 *
 *  @brief Reset Function register related
 *
 *  @file
 */

#ifndef RST_H__
#define RST_H__

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* RSTAT */
#define RSTAT_POR           ( 0x01 )        /**< RSTAT Register POR bit       */
#define RSTAT_XSTR          ( 0x02 )        /**< RSTAT Register XSTR bit      */
#define RSTAT_WDTR          ( 0x04 )        /**< RSTAT Register WDTR bit      */
#define RSTAT_VLSR          ( 0x08 )        /**< RSTAT Register VLSR bit      */
#define RSTAT_LOCKUP        ( 0x20 )        /**< RSTAT Register LOCKUP bit    */
#define RSTAT_INITE         ( 0x80 )        /**< RSTAT Register INITE bit     */

/* LOCKUPEN */
#define LOCKUPEN_ON         ( 0x01 )        /**< LOCKUP Enable Register       */
#define LOCKUPEN_OFF        ( 0x00 )        /**< LOCKUP Enable Register       */

/* REMAPRSTEN */
#define REMAPRSTEN_ON       ( 0x00000001 )  /**< REMAPCON and REMAPBASE register are initialized     */
#define REMAPRSTEN_OFF      ( 0x00000000 )  /**< REMAPCON and REMAPBASE register are not initialized */

/* CPURST */
#define CPURST_ON           ( 0x00000001 )  /**< CPU reset ON                 */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Get reset causes
 * 
 * Get causes which system-reset is generated.
 *
 * @param       -
 * @return      The value of RSTAT register
 * @see         MCU Users manual 'Reset Function'
 */

#define         rst_getResetCause()                 read_reg32( RESET->RSTAT )

/**
 * Clear reset causes
 * 
 * @param       -
 * @return      None
 */

#define         rst_clrResetCause()                 write_reg32( RESET->RSTAT, 0x00000000 )

/**
 * Enable lockup reset
 * 
 * Enable lockup reset.
 *
 * @param       -
 * @return      -
 * @see         MCU Users manual 'Reset Function'
 */

#define         lockup_enable()                     write_reg32( RESET->LOCKUPEN, LOCKUPEN_ON )

/**
 * Enable lockup reset
 * 
 * Enable lockup reset.
 *
 * @param       -
 * @return      -
 * @see         MCU Users manual 'Reset Function'
 */

#define         lockup_disable()                    write_reg32( RESET->LOCKUPEN, LOCKUPEN_OFF )

/**
 * Enable REMAP Register Reset
 * 
 * @param       -
 * @return      -
 */
#define         rst_remapRegisterReset_enable()     write_reg32( RESET->REMAPRSTEN, REMAPRSTEN_ON )

/**
 * Disable REMAP Register Reset
 * 
 * @param       -
 * @return      -
 */
#define         rst_remapRegisterReset_disable()    write_reg32( RESET->REMAPRSTEN, REMAPRSTEN_OFF )


/**
 * Execute CPU reset
 *
 * @param       -
 * @return      None
 */
void            rst_cpuReset( void );

#endif /*RST_H__*/

/** @} */
/** @} */
