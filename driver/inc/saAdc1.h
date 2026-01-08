/*****************************************************************************
 saAdc1.h

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
 * @file    saAdc1.h
 *
 * This module is 'SA-ADC1' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup SA-ADC
 *  @{
 *
 *  @brief Successive Approximation Type A/D Converter register related
 *
 *  @file
 */

#ifndef SAADC1_H__
#define SAADC1_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "saAdc_common.h"


/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/

/**
 * Enable A/D conversion start trigger
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaTrigger()              set_bit( SADC1->SADCON, (1U << 1U) )

/**
 * Disable A/D conversion start trigger
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disTrigger()              clear_bit( SADC1->SADCON, (1U << 1U) )

/**
 * Start A/D conversion
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_start()                   set_bit( SADC1->SADCON, (1U << 0U) )

/**
 * Stop A/D conversion
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_stop()                    clear_bit( SADC1->SADCON, (1U << 0U) )

/**
 * Getting A/D conversion running
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Running
 */
#define         saAdc1_getRunning()              get_bit( SADC1->SADCON, (1U << 0U) )

/**
 * Set A/D start trigger source
 *
 * @param[in]   trigger   Trigger source
 *                          - SAADC_SASTS_TM0INT
 *                          - SAADC_SASTS_TM1INT
 *                          - SAADC_SASTS_TM2INT
 *                          - SAADC_SASTS_TM3INT
 *                          - SAADC_SASTS_FTM0TGO
 *                          - SAADC_SASTS_FTM1TGO
 *                          - SAADC_SASTS_NTMS0TGO0
 *                          - SAADC_SASTS_NTMS0TGO1
 *                          - SAADC_SASTS_NTMS1TGO0
 *                          - SAADC_SASTS_NTMS1TGO1
 *                          - SAADC_SASTS_NTMS2TGO0
 *                          - SAADC_SASTS_NTMS2TGO1
 * @return      None
 */
#define         saAdc1_setTrigger( trigger )     write_reg32( SADC1->SADTRG, (trigger) )

/**
 * Enable A/D channel 0
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh0()                  set_bit( SADC1->SADEN, (1U << 0U) )

/**
 * Disable A/D channel 0
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh0()                  clear_bit( SADC1->SADEN, (1U << 0U) )

/**
 * Enable A/D channel 1
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh1()                  set_bit( SADC1->SADEN, (1U << 1U) )

/**
 * Disable A/D channel 1
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh1()                  clear_bit( SADC1->SADEN, (1U << 1U) )

/**
 * Enable A/D channel 2
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh2()                  set_bit( SADC1->SADEN, (1U << 2U) )

/**
 * Disable A/D channel 2
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh2()                  clear_bit( SADC1->SADEN, (1U << 2U) )

/**
 * Enable A/D channel 3
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh3()                  set_bit( SADC1->SADEN, (1U << 3U) )

/**
 * Disable A/D channel 3
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh3()                  clear_bit( SADC1->SADEN, (1U << 3U) )

/**
 * Enable A/D channel 4
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh4()                  set_bit( SADC1->SADEN, (1U << 4U) )

/**
 * Disable A/D channel 4
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh4()                  clear_bit( SADC1->SADEN, (1U << 4U) )

/**
 * Enable A/D channel 5
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh5()                  set_bit( SADC1->SADEN, (1U << 5U) )

/**
 * Disable A/D channel 5
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh5()                  clear_bit( SADC1->SADEN, (1U << 5U) )

/**
 * Enable A/D channel 6
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh6()                  set_bit( SADC1->SADEN, (1U << 6U) )

/**
 * Disable A/D channel 6
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh6()                  clear_bit( SADC1->SADEN, (1U << 6U) )

/**
 * Enable A/D channel 7
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh7()                  set_bit( SADC1->SADEN, (1U << 7U) )

/**
 * Disable A/D channel 7
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh7()                  clear_bit( SADC1->SADEN, (1U << 7U) )

/**
 * Enable A/D channel 8
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh8()                  set_bit( SADC1->SADEN, (1U << 8U) )

/**
 * Disable A/D channel 8
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh8()                  clear_bit( SADC1->SADEN, (1U << 8U) )

/**
 * Enable A/D channel 9
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh9()                  set_bit( SADC1->SADEN, (1U << 9U) )

/**
 * Disable A/D channel 9
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh9()                  clear_bit( SADC1->SADEN, (1U << 9U) )

/**
 * Enable A/D channel 10
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh10()                 set_bit( SADC1->SADEN, (1U << 10U) )

/**
 * Disable A/D channel 10
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh10()                 clear_bit( SADC1->SADEN, (1U << 10U) )

/**
 * Enable A/D channel 11
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh11()                 set_bit( SADC1->SADEN, (1U << 11U) )

/**
 * Disable A/D channel 11
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh11()                 clear_bit( SADC1->SADEN, (1U << 11U) )

/**
 * Enable A/D channel 17
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaCh17()                 set_bit( SADC1->SADEN, (1U << 17U) )

/**
 * Disable A/D channel 17
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disCh17()                 clear_bit( SADC1->SADEN, (1U << 17U) )

/**
 * Enable A/D result limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_enaLimit()                set_bit( SADC1->SADLMOD, (1U << 0U) )

/**
 * Disable A/D result limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_disLimit()                clear_bit( SADC1->SADLMOD, (1U << 0U) )

/**
 * Set A/D result upper limit
 *
 * @param[in]   limit   upper limit value
 * @return      None
 */
#define         saAdc1_setUpperLimit( limit )    write_reg32( SADC1->SADUPL, (limit) )

/**
 * Set A/D result lower limit
 *
 * @param[in]   limit   lower limit value
 * @return      None
 */
#define         saAdc1_setLowerLimit( limit )    write_reg32( SADC1->SADLOL, (limit) )

/**
 * Getting status of limit
 *
 * @param   -
 * @return  A status of limit <br>
 *          (A value of SADULS register)
 */
#define         saAdc1_getLimitStatus()          read_reg32( SADC1->SADULS )

/**
 * Getting status0 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus0()         get_bit( SADC1->SADULS, (1U << 0U) )

/**
 * Getting status1 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus1()         get_bit( SADC1->SADULS, (1U << 1U) )

/**
 * Getting status2 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus2()         get_bit( SADC1->SADULS, (1U << 2U) )

/**
 * Getting status3 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus3()         get_bit( SADC1->SADULS, (1U << 3U) )

/**
 * Getting status4 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus4()         get_bit( SADC1->SADULS, (1U << 4U) )

/**
 * Getting status5 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus5()         get_bit( SADC1->SADULS, (1U << 5U) )

/**
 * Getting status6 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus6()         get_bit( SADC1->SADULS, (1U << 6U) )

/**
 * Getting status7 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus7()         get_bit( SADC1->SADULS, (1U << 7U) )

/**
 * Getting status8 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus8()         get_bit( SADC1->SADULS, (1U << 8U) )

/**
 * Getting status9 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus9()         get_bit( SADC1->SADULS, (1U << 9U) )

/**
 * Getting status10 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus10()        get_bit( SADC1->SADULS, (1U << 10U) )

/**
 * Getting status11 of limit
 *
 * @param       -
 * @retval      0               inside of a limit
 * @retval      "other than 0"  outside of a limit
 */
#define         saAdc1_getLimitStatus11()        get_bit( SADC1->SADULS, (1U << 11U) )

/**
 * Clear status of limit
 *
 * @param[in]   clr     Specify the logical sum of the following channels, or SAADC_SAULC_ALL_CLR.
 * @arg                     0x001:channel 0
 * @arg                     0x002:channel 1
 * @arg                     (...)
 * @arg                     0x400:channel 10
 * @arg                     0x800:channel 11
 * @return      None
 */
#define         saAdc1_clrLimitStatus( clr )     write_reg32( SADC1->SADULC, (clr) )

/**
 * Clear All status of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrAllLimitStatus()       write_reg32( SADC1->SADULC, SAADC_SAULC_ALL_CLR )

/**
 * Clear status0 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus0()         set_bit( SADC1->SADULC, (1U << 0U) )

/**
 * Clear status1 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus1()         set_bit( SADC1->SADULC, (1U << 1U) )

/**
 * Clear status2 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus2()         set_bit( SADC1->SADULC, (1U << 2U) )

/**
 * Clear status3 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus3()         set_bit( SADC1->SADULC, (1U << 3U) )

/**
 * Clear status4 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus4()         set_bit( SADC1->SADULC, (1U << 4U) )

/**
 * Clear status5 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus5()         set_bit( SADC1->SADULC, (1U << 5U) )

/**
 * Clear status6 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus6()         set_bit( SADC1->SADULC, (1U << 6U) )

/**
 * Clear status7 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus7()         set_bit( SADC1->SADULC, (1U << 7U) )

/**
 * Clear status8 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus8()         set_bit( SADC1->SADULC, (1U << 8U) )

/**
 * Clear status9 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus9()         set_bit( SADC1->SADULC, (1U << 9U) )

/**
 * Clear status10 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus10()         set_bit( SADC1->SADULC, (1U << 10U) )

/**
 * Clear status11 of limit
 *
 * @param   -
 * @return  None
 */
#define         saAdc1_clrLimitStatus11()         set_bit( SADC1->SADULC, (1U << 11U) )

/**
 * Getting A/D conversion result
 *
 * @param   -
 * @return  A/D conversion result
 */
#define         saAdc1_getResult()               read_reg32( SADC1->SADR )

/**
 * Getting A/D conversion result0
 *
 * @param   -
 * @return  A/D conversion result0
 */
#define         saAdc1_getResult0()              read_reg32( SADC1->SADR0 )

/**
 * Getting A/D conversion result1
 *
 * @param   -
 * @return  A/D conversion result1
 */
#define         saAdc1_getResult1()              read_reg32( SADC1->SADR1 )

/**
 * Getting A/D conversion result2
 *
 * @param   -
 * @return  A/D conversion result2
 */
#define         saAdc1_getResult2()              read_reg32( SADC1->SADR2 )

/**
 * Getting A/D conversion result3
 *
 * @param   -
 * @return  A/D conversion result3
 */
#define         saAdc1_getResult3()              read_reg32( SADC1->SADR3 )

/**
 * Getting A/D conversion result4
 *
 * @param   -
 * @return  A/D conversion result4
 */
#define         saAdc1_getResult4()              read_reg32( SADC1->SADR4 )

/**
 * Getting A/D conversion result5
 *
 * @param   -
 * @return  A/D conversion result5
 */
#define         saAdc1_getResult5()              read_reg32( SADC1->SADR5 )

/**
 * Getting A/D conversion result6
 *
 * @param   -
 * @return  A/D conversion result6
 */
#define         saAdc1_getResult6()              read_reg32( SADC1->SADR6 )

/**
 * Getting A/D conversion result7
 *
 * @param   -
 * @return  A/D conversion result7
 */
#define         saAdc1_getResult7()              read_reg32( SADC1->SADR7 )

/**
 * Getting A/D conversion result8
 *
 * @param   -
 * @return  A/D conversion result8
 */
#define         saAdc1_getResult8()              read_reg32( SADC1->SADR8 )

/**
 * Getting A/D conversion result9
 *
 * @param   -
 * @return  A/D conversion result9
 */
#define         saAdc1_getResult9()              read_reg32( SADC1->SADR9 )

/**
 * Getting A/D conversion result10
 *
 * @param   -
 * @return  A/D conversion result10
 */
#define         saAdc1_getResult10()              read_reg32( SADC1->SADR10 )

/**
 * Getting A/D conversion result11
 *
 * @param   -
 * @return  A/D conversion result11
 */
#define         saAdc1_getResult11()              read_reg32( SADC1->SADR11 )

/**
 * Set A/D converter test mode
 *
 * @param[in]   mode   Test mode
 *                        - SAADC_SADTMOD_DISABLE
 *                        - SAADC_SADTMOD_FULLSCALE
 *                        - SAADC_SADTMOD_ZEROSCALE
 *                        - SAADC_SADTMOD_VREFSCALE
 * @return      None
 */
#define         saAdc1_setTestmode( mode )       write_reg32( SADC1->SADTMOD, (mode) )


void saAdc1_init( initAdc_t *st_initAdc );
void saAdc1_setEnableChannel( enableAdcChannel_t *st_enableChannel );

#endif /*SAADC1_H__*/

/** @} */
/** @} */
