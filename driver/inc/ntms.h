/*****************************************************************************
 ntms.h

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
 * @file    ntms.h
 *
 * This module is 'NTMS' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup NTMS
 *  @{
 *
 *  @brief 3-Phase Motor Control PWM register related
 *
 *  @file
 */

#ifndef NTMS_H__
#define NTMS_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
#define NTMS_TIMER_BIT_NTMS0           ( 1U << 16U )        /**< NTMS0  */
#define NTMS_TIMER_BIT_NTMS1           ( 1U << 17U )        /**< NTMS1  */
#define NTMS_TIMER_BIT_NTMS2           ( 1U << 18U )        /**< NTMS2  */


/*=== API parameters value ===*/
/* Initial configuration parameters for ntms_init() */
#define NTMS_MODE_SAWTOOTH             ( 0x1U )             /**< Sawtooth Wave Mode                                                                    */
#define NTMS_MODE_TRI_BOTH_VLY         ( 0x6U )             /**< Triangle Wave Mode 3. Rising and falling same threshold (trough) transfer.            */
#define NTMS_MODE_TRI_BOTH_MOUVLY      ( 0x7U )             /**< Triangle Wave Mode 4. Rising and falling same threshold (peaks and valleys) transfer. */
#define NTMS_BFF_NO_BUFFER             ( 0x0U )             /**< No Buffer                                                                             */
#define NTMS_BFF_SINGLE                ( 0x1U )             /**< Single buffer                                                                         */
#define NTMS_ADBFF_SAW_DIS             ( 0x0U )             /**< No transfer (Sawtooth Wave Mode)                                                      */
#define NTMS_ADBFF_SAW_EXPIRED         ( 0x1U )             /**< Transfer at Expiration (Sawtooth Wave Mode)                                           */
#define NTMS_ADBFF_TRI_DIS             ( 0x0U )             /**< No transfer (Triangle Wave Mode)                                                      */
#define NTMS_ADBFF_TRI_MOU             ( 0x1U )             /**< Transfer at top (Triangle Wave Mode)                                                  */
#define NTMS_ADBFF_TRI_VLY             ( 0x2U )             /**< Transfer at bottom (Triangle Wave Mode)                                               */
#define NTMS_ADBFF_TRI_MOUVLY          ( 0x3U )             /**< Transfer at bottom and tpo (Triangle Wave Mode)                                       */
#define NTMS_STOP_IMMEDIATE            ( 0x0U )             /**< Count stop condition: Immediately after stop setting                                  */
#define NTMS_STOP_EXPIRED              ( 0x1U )             /**< Count stop condition: At the expiration after the stop setting                        */
#define NTMS_STOP_NO_INIT              ( 0x0U )             /**< Do not initialize the counter when the count is stopped.                              */
#define NTMS_STOP_INIT                 ( 0x1U )             /**< Initialize the counter when the count is stopped.                                     */
#define NTMS_OUTPUT_NORMAL             ( 0x0U )             /**< Output as it is                                                                       */
#define NTMS_OUTPUT_SWAP               ( 0x1U )             /**< Invert and output                                                                     */
#define NTMS_TRG_HIGH_LEVEL            ( 0x4U )             /**< Emergency Stop External Input Condition: High Level                                   */
#define NTMS_TRG_DIS                   ( 0x7U )             /**< Emergency Stop External Input Condition: Invalid                                      */
#define NTMS_EXT_INPUT_ASYNC           ( 0x0U )             /**< Trigger output blocks and timers run at different clocks                              */
#define NTMS_EXT_INPUT_SYNC            ( 0x1U )             /**< Trigger output blocks and timers run with synchronous clocks                          */
#define NTMS_EMG_STOP_DIS              ( 0x0U )             /**< Emergency stop function OFF                                                           */
#define NTMS_EMG_STOP_ENA              ( 0x1U )             /**< Emergency stop function ON                                                            */
#define NTMS_EMG_RECOVERY_IMMEDIATE    ( 0x0U )             /**< Immediate return                                                                      */
#define NTMS_EMG_RECOVERY_EXPIRED      ( 0x1U )             /**< In the case of a sawtooth wave, it returns after the timer expires. In the case of a triangle wave, it returns after the "bottom" occurs. */
#define NTMS_EMG_START_IMMEDIATE       ( 0x0U )             /**< Immediate start                                                                       */
#define NTMS_EMG_START_EXPIRED         ( 0x1U )             /**< In the case of a sawtooth wave, it starts after the timer expires. In the case of a triangle wave, it starts after the "bottom" occurs.   */
#define NTMS_EMG_LOW_FIXED_OUT         ( 0x0U )             /**< Fixed output "0"                                                                      */
#define NTMS_EMG_HIGH_FIXED_OUT        ( 0x1U )             /**< Fixed output "1"                                                                      */
#define NTMS_EMG_HIGH_Z_OUT            ( 0x2U )             /**< Output Hi-Z                                                                           */
#define NTMS_EMG_DT_DIS                ( 0x0U )             /**< No Emergency Stop Start Dead Time, No Emergency Stop End Dead Time                    */
#define NTMS_EMG_DT_ONLY_START         ( 0x1U )             /**< With Emergency Stop Start Dead Time, No Emergency Stop End Dead Time                  */
#define NTMS_EMG_DT_ONLY_STOP          ( 0x2U )             /**< No Emergency Stop Start Dead Time, Emergency Stop End Dead Time                       */
#define NTMS_EMG_DT_START_STOP         ( 0x3U )             /**< With Emergency Stop Start Dead Time, With Emergency Stop End Dead Time                */
#define NTMS_EMG_DT_OUTPUT_LOW         ( 0x0U )             /**< Dead Time "L" Output for Emergency Stop                                               */
#define NTMS_EMG_DT_OUTPUT_HIGH        ( 0x1U )             /**< Dead Time "H" Output for Emergency Stop                                               */
#define NTMS_ADC_THIN_DIS              ( 0x0U )             /**< Turn off A/D conversion thinning function                                             */
#define NTMS_ADC_THIN_ENA              ( 0x1U )             /**< Turn on A/D conversion thinning function                                              */
#define NTMS_ADC_THIN_NUM_0            ( 0x0U )             /**< Number of A/D conversion decimations: 0 times                                         */
#define NTMS_ADC_THIN_NUM_1            ( 0x1U )             /**< Number of A/D conversion decimations: 1 time                                          */
#define NTMS_ADC_THIN_NUM_2            ( 0x2U )             /**< Number of A/D conversion decimations: 2 times                                         */
#define NTMS_ADC_THIN_NUM_3            ( 0x3U )             /**< Number of A/D conversion decimations: 3 times                                         */
#define NTMS_ADC_THIN_NUM_4            ( 0x4U )             /**< Number of A/D conversion decimations: 4 times                                         */
#define NTMS_ADC_THIN_NUM_5            ( 0x5U )             /**< Number of A/D conversion decimations: 5 times                                         */
#define NTMS_ADC_THIN_NUM_6            ( 0x6U )             /**< Number of A/D conversion decimations: 6 times                                         */
#define NTMS_ADC_THIN_NUM_7            ( 0x7U )             /**< Number of A/D conversion decimations: 7 times                                         */
#define NTMS_ADC_TMG_SAW_DIS           ( 0x0U )             /**< No thinning (Sawtooth Wave Mode)                                                      */
#define NTMS_ADC_TMG_SAW_EXPIRED       ( 0x1U )             /**< Thinning on expiration (Sawtooth Wave Mode)                                           */
#define NTMS_ADC_TMG_TRI_DIS           ( 0x0U )             /**< No thinning (Triangle Wave Mode)                                                      */
#define NTMS_ADC_TMG_TRI_MOU           ( 0x1U )             /**< Thinning based on "top" (Triangle Wave Mode)                                          */
#define NTMS_ADC_TMG_TRI_VLY           ( 0x2U )             /**< Thinning based on "bottom" (Triangle Wave Mode)                                       */
#define NTMS_ADC_TMG_TRI_MOUVLY        ( 0x3U )             /**< Thinning based on "bottom/top" (Triangle Wave Mode)                                   */
#define NTMS_ADC_THRE_DIS              ( 0x0U )             /**< Disable A/D Conversion Threshold                                                      */
#define NTMS_ADC_THRE_ENA              ( 0x1U )             /**< Enable A/D Conversion Threshold                                                       */
#define NTMS_ADC_DIRECT_DIS            ( 0x0U )             /**< Disable A/D Conversion Direct Control                                                 */
#define NTMS_ADC_DIRECT_ENA            ( 0x1U )             /**< Enable A/D Conversion Direct Control                                                  */

/* Configuration parameters for ntms_setTimerBreak() */
#define NTMS_TMBRK_CNTSTP_NTMS0_ENA    ( 0U << 16U )        /**< Enable NTMS0 Stop Counting                                                            */
#define NTMS_TMBRK_CNTSTP_NTMS0_DIS    ( 1U << 16U )        /**< Disable NTMS0 Stop Counting                                                           */
#define NTMS_TMBRK_CNTSTP_NTMS1_ENA    ( 0U << 17U )        /**< Enable NTMS1 Stop Counting                                                            */
#define NTMS_TMBRK_CNTSTP_NTMS1_DIS    ( 1U << 17U )        /**< Disable NTMS1 Stop Counting                                                           */
#define NTMS_TMBRK_CNTSTP_NTMS2_ENA    ( 0U << 18U )        /**< Enable NTMS2 Stop Counting                                                            */
#define NTMS_TMBRK_CNTSTP_NTMS2_DIS    ( 1U << 18U )        /**< Disable NTMS2 Stop Counting                                                           */

/* Initial configuration parameters for ntms_setEmergencyStopTrigger() */
#define NTMS_EMGTRG_DIS                ( 0x0U )             /**< Disable the emergency stop trigger                                                    */
#define NTMS_EMGTRG_ENA                ( 0x1U )             /**< Enable the Emergency Stop Trigger                                                     */
#define NTMS_EMGTRG_SWAP_DIS           ( 0x0U )             /**< Do not invert the polarity of the trigger source                                      */
#define NTMS_EMGTRG_SWAP_ENA           ( 0x1U )             /**< Invert the polarity of the trigger source                                             */
#define NTMS_EMGTRG_CTRL_LEVEL         ( 0x0U )             /**< Level control (emergency stop at H level, release at L level)                         */
#define NTMS_EMGTRG_CTRL_EDGE          ( 0x1U )             /**< Edge control (emergency stop at the POS edge, clear by software to release)           */

/* Initial configuration parameters for ntms_setXXXXBuffer() */
#define NTMS_BFFMODE_ENA               ( 0x0U )             /**< With buffer operation                                                                 */
#define NTMS_BFFMODE_DIS               ( 0x1U )             /**< No buffer operation                                                                   */

/* Initial configuration parameters for ntms_getTimerStatus() */
#define NTMS_STATUS_UPCOUNT            ( 0U << 3U )         /**< Up Counter                                                                            */
#define NTMS_STATUS_DOWNCOUNT          ( 1U << 3U )         /**< Down Counter                                                                          */
#define NTMS_STATUS_MASK_COUNT         ( 1U << 3U )         /**< Mask for checking the identification status of the up and down counters               */
#define NTMS_STATUS_MASK_ADCNT         ( 7U << 0U )         /**< Mask for checking the A/D conversion thinning counter                                 */

/* Interrupt settinigs */
#define NTMS_INT_CMP_ENA               ( 0U << 0U )         /**< Enable timer expiration interrupt                                                     */
#define NTMS_INT_CMP_DIS               ( 1U << 0U )         /**< Disable timer expiration interrupt                                                    */
#define NTMS_INT_MOU_ENA               ( 0U << 2U )         /**< Enable peak interrupt (Triangle Wave Mode)                                            */
#define NTMS_INT_MOU_DIS               ( 1U << 2U )         /**< Disable peak interrupt (Triangle Wave Mode)                                           */
#define NTMS_INT_TA0_ENA               ( 0U << 3U )         /**< Enable value match TMTHA0 interrupt (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_TA0_DIS               ( 1U << 3U )         /**< Disable value match TMTHA0 interrupt (Sawtooth/Triangle Wave Mode)                    */
#define NTMS_INT_TA1_ENA               ( 0U << 4U )         /**< Enable value match TMTHA1 interrupt (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_TA1_DIS               ( 1U << 4U )         /**< Disable value match TMTHA1 interrupt (Sawtooth/Triangle Wave Mode)                    */
#define NTMS_INT_TB0_ENA               ( 0U << 5U )         /**< Enable value match TMTHB0 interrupt (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_TB0_DIS               ( 1U << 5U )         /**< Disable value match TMTHB0 interrupt (Sawtooth/Triangle Wave Mode)                    */
#define NTMS_INT_TB1_ENA               ( 0U << 6U )         /**< Enable value match TMTHB1 interrupt (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_TB1_DIS               ( 1U << 6U )         /**< Disable value match TMTHB1 interrupt (Sawtooth/Triangle Wave Mode)                    */
#define NTMS_INT_DTB0_ENA              ( 0U << 16U )        /**< Enable TMTHB0 abnormal interrupt (Sawtooth/Triangle Wave Mode)                        */
#define NTMS_INT_DTB0_DIS              ( 1U << 16U )        /**< Disable TMTHB0 abnormal interrupt (Sawtooth/Triangle Wave Mode)                       */
#define NTMS_INT_DTB1_ENA              ( 0U << 17U )        /**< Enable TMTHB1 abnormal interrupt (Sawtooth/Triangle Wave Mode)                        */
#define NTMS_INT_DTB1_DIS              ( 1U << 17U )        /**< Disable TMTHB1 abnormal interrupt (Sawtooth/Triangle Wave Mode)                       */

/* Interrupt status */
#define NTMS_INT_STATUS_CMP            ( 1U << 0U )         /**< Timer expiration interrupt status  */
#define NTMS_INT_STATUS_MOU            ( 1U << 2U )         /**< "Top" interrupt status (Triangle Wave Mode) <br> Note: This status will not become "1" while the emergency stop process is being executed.  */
#define NTMS_INT_STATUS_TA0            ( 1U << 3U )         /**< Value match TMTHA0 interrupt status (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_STATUS_TA1            ( 1U << 4U )         /**< Value match TMTHA1 interrupt status (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_STATUS_TB0            ( 1U << 5U )         /**< Value match TMTHB0 interrupt status (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_STATUS_TB1            ( 1U << 6U )         /**< Value match TMTHB1 interrupt status (Sawtooth/Triangle Wave Mode)                     */
#define NTMS_INT_STATUS_DTB0           ( 1U << 16U )        /**< TMTHB0 abnormal status (Sawtooth/Triangle Wave Mode)                                  */
#define NTMS_INT_STATUS_DTB1           ( 1U << 17U )        /**< TMTHB1 abnormal status (Sawtooth/Triangle Wave Mode)                                  */

/* Emergency stop trigger status related */
#define NTMS_EMGTRG_ST_SEL0_LEVEL_LOW  ( 0x00 << 0U )       /**< SEL0 Status: Emergency stop trigger level L  <br> Note: Used when "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger().        */
#define NTMS_EMGTRG_ST_SEL0_LEVEL_HIGH ( 0x01 << 0U )       /**< SEL0 Status: Emergency stop trigger level H  <br> Note: Used when "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger().        */
#define NTMS_EMGTRG_ST_SEL0_NO_REQUEST ( 0x00 << 0U )       /**< SEL0 Status: No emergency stop request       <br> Note: Used when "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger().         */
#define NTMS_EMGTRG_ST_SEL0_REQUESTED  ( 0x01 << 0U )       /**< SEL0 Status: Emergency stop Rrequest         <br> Note: Used when "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger().         */
#define NTMS_EMGTRG_ST_SEL1_LEVEL_LOW  ( 0x00 << 1U )       /**< SEL1 Status: Emergency stop trigger level L  <br> Note: Used when "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger().        */
#define NTMS_EMGTRG_ST_SEL1_LEVEL_HIGH ( 0x01 << 1U )       /**< SEL1 Status: Emergency stop trigger level H  <br> Note: Used when "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger().        */
#define NTMS_EMGTRG_ST_SEL1_NO_REQUEST ( 0x00 << 1U )       /**< SEL1 Status: No emergency stop request       <br> Note: Used when "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger().         */
#define NTMS_EMGTRG_ST_SEL1_REQUESTED  ( 0x01 << 1U )       /**< SEL1 Status: Emergency stop Rrequest         <br> Note: Used when "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger().         */

/**
 * @brief Select NTMS Number
 */
typedef enum {
    NTMS_TIMER_NTMS0,                                       /**< Select NTMS0                                                                          */
    NTMS_TIMER_NTMS1,                                       /**< Select NTMS1                                                                          */
    NTMS_TIMER_NTMS2,                                       /**< Select NTMS2                                                                          */
    NTMS_TIMER_TOTAL_NUM                                    /**< The number of all NTMS                                                                */
} NTMS_TimerType;

/**
 * @brief Select Emergency stop trigger source Number
 */
typedef enum {
    NTMS_EMGTRG_SRC_EXI0TGO,                                /**<  Emergency stop trigger source : EXI0TGO                                              */
    NTMS_EMGTRG_SRC_EXI1TGO,                                /**<  Emergency stop trigger source : EXI0TG1                                              */
    NTMS_EMGTRG_SRC_EXI2TGO,                                /**<  Emergency stop trigger source : EXI0TG2                                              */
    NTMS_EMGTRG_SRC_EXI3TGO,                                /**<  Emergency stop trigger source : EXI0TG3                                              */
    NTMS_EMGTRG_SRC_EXI4TGO,                                /**<  Emergency stop trigger source : EXI0TG4                                              */
    NTMS_EMGTRG_SRC_EXI5TGO,                                /**<  Emergency stop trigger source : EXI0TG5                                              */
    NTMS_EMGTRG_SRC_EXI6TGO,                                /**<  Emergency stop trigger source : EXI0TG6                                              */
    NTMS_EMGTRG_SRC_EXI7TGO,                                /**<  Emergency stop trigger source : EXI0TG7                                              */
    NTMS_EMGTRG_SRC_CMP0TRG,                                /**<  Emergency stop trigger source : CMP0TRG                                              */
    NTMS_EMGTRG_SRC_CMP1TRG,                                /**<  Emergency stop trigger source : CMP1TRG                                              */
    NTMS_EMGTRG_SRC_CMP2TRG                                 /**<  Emergency stop trigger source : CMP2TRG                                              */
} NTMS_EmgTrgSrcType;

/*=== Strunct define ===*/
/**
 * @brief Initial parameters for NTMS
 */
typedef struct {
    uint8_t timer_mode;                                     /**< Function Selection                                                                    */
    uint8_t buffer_tha;                                     /**< Waveform threshold THA buffer operation setting                                       */
    uint8_t buffer_dead_time;                               /**< Dead Time Value Buffer Operation Settings                                             */
    uint8_t buffer_period;                                  /**< Periodic buffer operation setting                                                     */
    uint8_t buffer_adc_group_a;                             /**< A/D Conversion Start Request Timing Value A Buffer Operation Setting                  */
    uint8_t buffer_adc_group_b;                             /**< A/D Conversion Start Request Timing Value B Buffer Operation Setting                  */
    uint8_t counter_stop_mode;                              /**< Count end conditions when timer enable is OFF                                         */
    uint8_t stop_init;                                      /**< Counter initialization setting when the timer is stopped                              */
    uint8_t swap_out_p;                                     /**< P output waveform inversion control                                                   */
    uint8_t swap_out_n;                                     /**< N output waveform inversion control                                                   */
    uint8_t trig_emergency;                                 /**< Emergency stop external input format selection: input conditions                      */
    uint8_t trig_sync_emg;                                  /**< Emergency stop external input format selection: synchronization setting               */
    uint8_t emergency_stop;                                 /**< Emergency stop                                                                        */
    uint8_t emergency_recovery_mode;                        /**< Recovery settings after emergency stop                                                */
    uint8_t emergency_start_mode;                           /**< Start timing settings of emergency stop                                               */
    uint8_t emergency_out_p;                                /**< Timer output P fixed setting in case of emergency stop                                */
    uint8_t emergency_out_n;                                /**< Timer output N fixed setting in case of emergency stop                                */
    uint8_t emergency_dead_time;                            /**< Dead time setting in case of emergency stop                                           */
    uint8_t emergency_dead_time_out_p;                      /**< Dead time output P in case of emergency stop                                          */
    uint8_t emergency_dead_time_out_n;                      /**< Dead time output N in case of emergency stop                                          */
    uint8_t adc_thin_group_a;                               /**< Group A A/D conversion thinning interlock setting                                     */
    uint8_t adc_thin_group_b;                               /**< Group B A/D conversion thinning interlock setting                                     */
    uint8_t adc_thin_num;                                   /**< A/D conversion thinning count selection setting                                       */
    uint8_t adc_thin_timing;                                /**< A/D conversion thinning timing function selection setting                             */
    uint8_t adc_thre_group_a;                               /**< Group A A/D conversion threshold valid selection setting                              */
    uint8_t adc_thre_group_b;                               /**< Group B A/D conversion threshold valid selection setting                              */
    uint8_t adc_direct_group_a;                             /**< Group A A/D conversion direct control selection                                       */
    uint8_t adc_direct_group_b;                             /**< Group B A/D conversion direct control selection                                       */
} NtmsInitType;

/**
 * @brief Initial Emergency stop configuration for NTMS
 */
typedef struct {
    NTMS_EmgTrgSrcType trig_source_0;                       /**< Emergency stop trigger source selection  #0                                           */
    NTMS_EmgTrgSrcType trig_source_1;                       /**< Emergency stop trigger source selection  #1                                           */
    uint8_t            trig_0;                              /**< Whether or not an emergency stop trigger is used  #0                                  */
    uint8_t            trig_1;                              /**< Whether or not an emergency stop trigger is used  #1                                  */
    uint8_t            trig_swap_0;                         /**< Trigger source polarity reversal #0                                                   */
    uint8_t            trig_swap_1;                         /**< Trigger source polarity reversal #1                                                   */
    uint8_t            trig_ctrl_mode_0;                    /**< Selection of emergency stop trigger control #0                                        */
    uint8_t            trig_ctrl_mode_1;                    /**< Selection of emergency stop trigger control #0                                        */
} NtmsEmgCfgType;

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
void     ntms_init( uint32_t TimerCh, const NtmsInitType *init);
void     ntms_setTimerBreak(uint32_t param);
void     ntms_setEmergencyStopTrigger(const NtmsEmgCfgType *param);
void     ntms_setAdcBuffer(uint32_t TimerCh, uint32_t AdBffMode);
void     ntms_setPeriodBuffer(uint32_t TimerCh, uint32_t BffMode);
void     ntms_setThreABBuffer(uint32_t TimerCh, uint32_t BffMode);
void     ntms_setDeadTimeBuffer(uint32_t TimerCh, uint32_t BffMode);
void     ntms_setTimerPeriod(uint32_t TimerCh, uint32_t Period);
uint32_t ntms_getTimerPeriod(uint32_t TimerCh);
void     ntms_setTimerPeriodBff(uint32_t TimerCh, uint32_t Period);
uint32_t ntms_getTimerPeriodBff(uint32_t TimerCh);
uint32_t ntms_getTimerCount(uint32_t TimerCh);
void     ntms_clearTimerCount(uint32_t TimerCh);
uint32_t ntms_getTimerStatus(uint32_t TimerCh);
void     ntms_setTHA0(uint32_t TimerCh, uint32_t tha0);
void     ntms_setTHA1(uint32_t TimerCh, uint32_t tha1);
void     ntms_setTHA0Bff(uint32_t TimerCh, uint32_t tha0);
void     ntms_setTHA1Bff(uint32_t TimerCh, uint32_t tha1);
void     ntms_setIntMask(uint32_t TimerCh, uint32_t interrupt);
uint32_t ntms_getIntStatus(uint32_t TimerCh);
void     ntms_clearIntStatus(uint32_t TimerCh, uint32_t status);
uint32_t ntms_getRawIntStatus(uint32_t TimerCh);
void     ntms_setDeadTimeU(uint32_t TimerCh, uint32_t DeadTimeU);
void     ntms_setDeadTimeD(uint32_t TimerCh, uint32_t DeadTimeD);
void     ntms_setDeadTimeUBff(uint32_t TimerCh, uint32_t DeadTimeU);
void     ntms_setDeadTimeDBff(uint32_t TimerCh, uint32_t DeadTimeD);
void     ntms_start(uint32_t TimerCh);
void     ntms_stop(uint32_t TimerCh);
void     ntms_startBit(uint32_t TargetTimer);
void     ntms_stopBit(uint32_t TargetTimer);
uint32_t ntms_getTimerOprtStatus(void);
void     ntms_startEmgProc(uint32_t TimerCh);
void     ntms_stopEmgProc(uint32_t TimerCh);
void     ntms_startBitEmgProc(uint32_t TargetTimer);
void     ntms_stopBitEmgProc(uint32_t TargetTimer);
uint32_t ntms_getEmgProcStatus(void);
uint32_t ntms_getEmgTrigStatus(void);
void     ntms_clearEmgTrigStatus(uint32_t EtsNum);
void     ntms_setTMADA(uint32_t TimerCh, uint32_t tmada);
void     ntms_setTMADB(uint32_t TimerCh, uint32_t tmadb);
void     ntms_setTMADABff(uint32_t TimerCh, uint32_t tmada);
void     ntms_setTMADBBff(uint32_t TimerCh, uint32_t tmadb);
uint32_t ntms_channelToBitPos(uint32_t TimerCh);

#endif /*NTMS_H__*/

/** @} */
/** @} */
