/*****************************************************************************
 tbc.h

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
 * @file    tbc.h
 *
 * This module is 'TBC' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup TBC
 *  @{
 *
 *  @brief Time Base Counter register related
 *
 *  @file
 */

#ifndef TBC_H__
#define TBC_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "tbc_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Get value of low-speed time base counter(LTBR)
 *
 * @param       -
 * @return      low-speed time base counter value <br>
 *              (A value of LTBR register)
 */
#define         tbc_getLTBR()           read_reg32( LTBC->LTBR )

/**
 * Get value of low-speed time base counter for RTC(LTBRR)
 *
 * @param       -
 * @return      low-speed time base counter value for RTC <br>
 *              (A value of LTBRR register)
 */
#define         tbc_getLTBRR()          read_reg32( LTBC->LTBRR )

/**
 * Set frequency adjustment value of low-speed time base clock
 *
 * @param[in]   adj     frequency adjustment value
 * @return      None
 */
#define         tbc_setLtbAdj( adj )    write_reg32( LTBC->LTBADJ, (adj) )

/**
 * Get frequency adjustment value of low-speed time base clock
 *
 * @param       -
 * @return      frequency adjustment value <br>
 *              (A value of LTBADJ register)
 */
#define         tbc_getLtbAdj()         read_reg32( LTBC->LTBADJ )

/**
 * Get interrupt status of low-speed time base clock
 *
 * @param       -
 * @return      interrupt status <br>
 *              (A value of LTBCINTST register)
 */
#define         tbc_getIntStat()        read_reg32( LTBC->LTBCINTST )

/**
 * Clear interrupt status of low-speed time base clock
 *
 * @param[in]   ltbcint interrupt status <br>
 *              Specify the logical sum of the following items, or TBC_INTST_ALL_CLEAR.
 * @arg             TBC_INTST_LTBINT0
 * @arg             TBC_INTST_LTBINT1
 * @arg             TBC_INTST_LTBINT2
 * @return      None
 */
#define         tbc_clearIntStat( ltbcint ) write_reg32( LTBC->LTBCINTST, (ltbcint) )

void tbc_setLTI0S( uint8_t lti0s );
void tbc_setLTI1S( uint8_t lti1s );
void tbc_setLTI2S( uint8_t lti2s );
void tbc_setLTBINT( uint8_t lti0s, uint8_t lti1s, uint8_t lti2s, uint8_t ltco );
void tbc_clrLTBR( void );
void tbc_setOutclk( uint8_t select );

#endif /*TBC_H__*/

/** @} */
/** @} */
