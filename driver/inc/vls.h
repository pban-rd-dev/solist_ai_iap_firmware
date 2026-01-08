/*****************************************************************************
 vls.h

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
 * @file    vls.h
 *
 * This module is 'VLS' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup VLS
 *  @{
 *
 *  @brief Voltage Level Supervisor register related
 *
 *  @file
 */

#ifndef VLS_H__
#define VLS_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
#define VLSnLV_VLSLV0           ( 0x00000001UL )                    /**< VLS0LV Register VLS0LV[0] bit */
#define VLSnLV_VLSLV1           ( 0x00000002UL )                    /**< VLS0LV Register VLS0LV[1] bit */
#define VLSnLV_VLSLV2           ( 0x00000004UL )                    /**< VLS0LV Register VLS0LV[2] bit */
#define VLSnLV_VLSLV3           ( 0x00000008UL )                    /**< VLS0LV Register VLS0LV[3] bit */
#define VLSnLV_VLSLV_MASK       ( VLSnLV_VLSLV3 | VLSnLV_VLSLV2 | VLSnLV_VLSLV1 | VLSnLV_VLSLV0 ) /**< VLS0LV Register VLS0LV[3:0] */

#define VLSnMOD_VLSnSEL0        ( 0x00000001UL )                    /**< VLS0MOD Register VLS0SEL[0] bit */
#define VLSnMOD_VLSnSEL1        ( 0x00000002UL )                    /**< VLS0MOD Register VLS0SEL[1] bit */
#define VLSnMOD_VLSnAMD0        ( 0x00000010UL )                    /**< VLS0MOD Register VLS0AMD[0] bit */
#define VLSnMOD_VLSnAMD1        ( 0x00000020UL )                    /**< VLS0MOD Register VLS0AMD[1] bit */
#define VLSnSMP_VLSnSM1         ( 0x00000008UL )                    /**< VLS0SMP Register VLS0SM1 bit    */
#define VLSnSMP_VLSnDIV0        ( 0x00000010UL )                    /**< VLS0SMP Register VLS0DIV[0] bit */
#define VLSnSMP_VLSnDIV1        ( 0x00000020UL )                    /**< VLS0SMP Register VLS0DIV[1] bit */
#define VLSnSTS_VLSnENS         ( 0x00000001UL )                    /**< VLS0STS Register VLS0ENS bit    */
#define VLSnSTS_VLSnFS          ( 0x00000002UL )                    /**< VLS0STS Register VLS0FS bit     */
#define VLSnSTS_VLSnRFS         ( 0x00000004UL )                    /**< VLS0STS Register VLS0RFS bit    */
#define VLSnSTC_VLSnFC          ( 0x00000002UL )                    /**< VLS0STC Register VLS0FC bit     */

/*=== API parameters value ===*/
#define VLS_AMD_SINGLE_1        ( 0 )                               /**< VLS mode : Single mode 1     */
#define VLS_AMD_SINGLE_2        ( VLSnMOD_VLSnAMD0 )                /**< VLS mode : Single mode 2     */
#define VLS_AMD_SUPERVISOR      ( VLSnMOD_VLSnAMD1 )                /**< VLS mode : Supervisor mode   */

#define VLS_SAMPLING_DIS        ( 0 )                               /**< Sampling : Without sampling  */
#define VLS_SAMPLING_ENA        ( VLSnSMP_VLSnSM1 )                 /**< Sampling : With sampling     */

#define VLS_DIV_DIV1            ( 0 )                               /**< dividing ratio for the sampling clock:1/1 */
#define VLS_DIV_DIV2            ( VLSnSMP_VLSnDIV0 )                /**< dividing ratio for the sampling clock:1/2 */
#define VLS_DIV_DIV4            ( VLSnSMP_VLSnDIV1 )                /**< dividing ratio for the sampling clock:1/4 */
#define VLS_DIV_DIV8            ( VLSnSMP_VLSnDIV1 | VLSnSMP_VLSnDIV0 ) /**< dividing ratio for the sampling clock:1/8 */

#define VLS_RST_DIS_INT_DIS     ( 0 )                               /**< Reset and interrupt : disable          */
#define VLS_RST_ENA_INT_DIS1    ( VLSnMOD_VLSnSEL0 )                /**< Reset : enable,  Interrupt : disable   */
#define VLS_RST_DIS_INT_ENA     ( VLSnMOD_VLSnSEL1 )                /**< Reset : disable, Interrupt : enable    */
#define VLS_RST_ENA_INT_DIS2    ( VLSnMOD_VLSnSEL1 | VLSnMOD_VLSnSEL0 )  /**< Reset : enable,  Interrupt : disable   */

#define VLS_LV_399              ( 0 )                               /**< threshold voltage:3.99 V     */
#define VLS_LV_353              ( VLSnLV_VLSLV0 )                   /**< threshold voltage:3.53 V     */
#define VLS_LV_305              ( VLSnLV_VLSLV1 )                   /**< threshold voltage:3.05 V     */
#define VLS_LV_296              ( VLSnLV_VLSLV1 | VLSnLV_VLSLV0 )   /**< threshold voltage:2.96 V     */
#define VLS_LV_284              ( VLSnLV_VLSLV2 )                   /**< threshold voltage:2.84 V     */
#define VLS_LV_276              ( VLSnLV_VLSLV2 | VLSnLV_VLSLV0 )   /**< threshold voltage:2.76 V     */
#define VLS_LV_266              ( VLSnLV_VLSLV2 | VLSnLV_VLSLV1 )   /**< threshold voltage:2.66 V     */
#define VLS_LV_254              ( VLSnLV_VLSLV2 | VLSnLV_VLSLV1 | VLSnLV_VLSLV0 ) /**< threshold voltage:2.54 V     */
#define VLS_LV_245              ( VLSnLV_VLSLV3 )                   /**< threshold voltage:2.45 V     */
#define VLS_LV_235              ( VLSnLV_VLSLV3 | VLSnLV_VLSLV0 )   /**< threshold voltage:2.35 V     */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Activates VLS (ON)
 *
 * @param       -
 * @return      None
 */
#define         vls_on()                    set_bit( VLS->VLS0CON, (1U << 0U) )

/**
 * Checking VLS result
 *
 * Getting the result of compare voltage.
 *
 * @param       -
 * @retval      0              : VDD > threshold voltage
 * @retval      "other than 0" : otherwise
 */
#define         vls_check()                 ( get_bit( VLS->VLS0STS, (1U << 1U) ) )

/**
 * Deactivates VLS (OFF)
 *
 * @param       -
 * @return      None
 */
#define         vls_off()                   clear_bit( VLS->VLS0CON, (1U << 0U) )

/**
 * Getting VLS status
 *
 * Getting the status of VLS result.
 *
 * @param       -
 * @retval      0              : VLS result is invalid (VLS-OFF or Judgment is in progress)
 * @retval      "other than 0" : VLS result is valid
 */
#define         vls_getResultStatus()       ( get_bit( VLS->VLS0STS, (1U << 2U) ) )

/**
 * Set threshold voltage
 *
 * @param[in]   vlsv        threshold voltage (VLS_LV_235 ~ VLS_LV_284)
 * @return      None
 */
#define         vls_setTh( vlsv )           write_reg32( VLS->VLS0LV, (vlsv) )

/**
 * Get status of VLS
 *
 * @param       -
 * @retval      0               Stopped
 * @retval      "other than 0"  Running
 */
#define         vls_getRunning()            ( get_bit( VLS->VLS0STS, (1U << 0U) ) )

/**
 * Clear VLS result
 *
 * @param       -
 * @return      None
 */
#define         vls_clrResult()             set_bit( VLS->VLS0STC, (1U << 1U) )


void            vls_init( uint8_t vlslv, uint8_t vlsmod, uint8_t vlssmp );

#endif /*VLS_H__*/

/** @} */
/** @} */
