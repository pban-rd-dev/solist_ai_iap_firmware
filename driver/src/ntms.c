/*****************************************************************************
 ntms.c

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

#include "mcu.h"
#include "rdwr_reg.h"
#include "ntms.h"
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
/*#                              Subroutine                                  #*/
/*############################################################################*/
/*--- Table. ---*/
/**
 * For NTMS instance. 
 */
static TMR0_NTMSnm_Type *NTMS_BASE[3U] = {TMR0_NTMS00, TMR0_NTMS01, TMR0_NTMS02};

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize NTMS
 *
 * @param[in]   *NtmsInitType     Configuration of NTMS operation
 * @arg                             timer_mode                                
 *                                    - NTMS_MODE_SAWTOOTH                           
 *                                    - NTMS_MODE_TRI_BOTH_VLY                       
 *                                    - NTMS_MODE_TRI_BOTH_MOUVLY             
 * @arg                             buffer_tha                                
 *                                    - NTMS_BFF_NO_BUFFER                           
 *                                    - NTMS_BFF_SINGLE                       
 * @arg                             buffer_dead_time                          
 *                                    - NTMS_BFF_NO_BUFFER                           
 *                                    - NTMS_BFF_SINGLE                       
 * @arg                             buffer_period                             
 *                                    - NTMS_BFF_NO_BUFFER                           
 *                                    - NTMS_BFF_SINGLE                       
 * @arg                             buffer_adc_group_a                        
 *                                    - For Sawtooth Wave Mode                     
 *                                        - NTMS_ADBFF_SAW_DIS                       
 *                                        - NTMS_ADBFF_SAW_EXPIRED                   
 *                                    - For Triangle Wave Mode                
 *                                        - NTMS_ADBFF_TRI_DIS                       
 *                                        - NTMS_ADBFF_TRI_MOU                       
 *                                        - NTMS_ADBFF_TRI_VLY                       
 *                                        - NTMS_ADBFF_TRI_MOUVLY             
 * @arg                             buffer_adc_group_b                        
 *                                    - For Sawtooth Wave Mode                     
 *                                        - NTMS_ADBFF_SAW_DIS                       
 *                                        - NTMS_ADBFF_SAW_EXPIRED                   
 *                                    - For Triangle Wave Mode                
 *                                        - NTMS_ADBFF_TRI_DIS                       
 *                                        - NTMS_ADBFF_TRI_MOU                       
 *                                        - NTMS_ADBFF_TRI_VLY                       
 *                                        - NTMS_ADBFF_TRI_MOUVLY             
 * @arg                             counter_stop_mode                         
 *                                        - NTMS_STOP_IMMEDIATE                      
 *                                        - NTMS_STOP_EXPIRED                 
 * @arg                             stop_init                                 
 *                                        - NTMS_STOP_NO_INIT                        
 *                                        - NTMS_STOP_INIT                    
 * @arg                             swap_out_p                                
 *                                        - NTMS_OUTPUT_NORMAL                       
 *                                        - NTMS_OUTPUT_SWAP                  
 * @arg                             swap_out_n                                
 *                                        - NTMS_OUTPUT_NORMAL                       
 *                                        - NTMS_OUTPUT_SWAP                  
 * @arg                             trig_emergency                            
 *                                        - NTMS_TRG_HIGH_LEVEL                      
 *                                        - NTMS_TRG_DIS                      
 * @arg                             trig_sync_emg                             
 *                                        - NTMS_EXT_INPUT_ASYNC                     
 *                                        - NTMS_EXT_INPUT_SYNC               
 * @arg                             emergency_stop                            
 *                                        - NTMS_EMG_STOP_DIS                        
 *                                        - NTMS_EMG_STOP_ENA                 
 * @arg                             emergency_recovery_mode                   
 *                                        - NTMS_EMG_RECOVERY_IMMEDIATE              
 *                                        - NTMS_EMG_RECOVERY_EXPIRED         
 * @arg                             emergency_start_mode                      
 *                                        - NTMS_EMG_START_IMMEDIATE                
 *                                        - NTMS_EMG_START_EXPIRED            
 * @arg                             emergency_out_p                           
 *                                        - NTMS_EMG_LOW_FIXED_OUT                   
 *                                        - NTMS_EMG_HIGH_FIXED_OUT                  
 *                                        - NTMS_EMG_HIGH_Z_OUT               
 * @arg                             emergency_out_n                           
 *                                        - NTMS_EMG_LOW_FIXED_OUT                   
 *                                        - NTMS_EMG_HIGH_FIXED_OUT                  
 *                                        - NTMS_EMG_HIGH_Z_OUT               
 * @arg                             emergency_dead_time                       
 *                                        - NTMS_EMG_DT_DIS                          
 *                                        - NTMS_EMG_DT_ONLY_START                   
 *                                        - NTMS_EMG_DT_ONLY_STOP                    
 *                                        - NTMS_EMG_DT_START_STOP            
 * @arg                             emergency_dead_time_out_p                 
 *                                        - NTMS_EMG_DT_OUTPUT_LOW                   
 *                                        - NTMS_EMG_DT_OUTPUT_HIGH           
 * @arg                             emergency_dead_time_out_n                 
 *                                        - NTMS_EMG_DT_OUTPUT_LOW                   
 *                                        - NTMS_EMG_DT_OUTPUT_HIGH           
 * @arg                             adc_thin_group_a                          
 *                                        - NTMS_ADC_THIN_DIS                        
 *                                        - NTMS_ADC_THIN_ENA                 
 * @arg                             adc_thin_group_b                          
 *                                        - NTMS_ADC_THIN_DIS                        
 *                                        - NTMS_ADC_THIN_ENA                 
 * @arg                             adc_thin_num                              
 *                                        - NTMS_ADC_THIN_NUM_0                      
 *                                        - NTMS_ADC_THIN_NUM_1                      
 *                                        - NTMS_ADC_THIN_NUM_2                      
 *                                        - NTMS_ADC_THIN_NUM_3                      
 *                                        - NTMS_ADC_THIN_NUM_4                      
 *                                        - NTMS_ADC_THIN_NUM_5                      
 *                                        - NTMS_ADC_THIN_NUM_6                      
 *                                        - NTMS_ADC_THIN_NUM_7               
 * @arg                             adc_thin_timing                           
 *                                    - For Sawtooth Wave Mode                     
 *                                        - NTMS_ADC_TMG_SAW_DIS                     
 *                                        - NTMS_ADC_TMG_SAW_EXPIRED          
 *                                    - For Triangle Wave Mode                
 *                                        - NTMS_ADC_TMG_TRI_DIS                     
 *                                        - NTMS_ADC_TMG_TRI_MOU                     
 *                                        - NTMS_ADC_TMG_TRI_VLY                     
 *                                        - NTMS_ADC_TMG_TRI_MOUVLY           
 * @arg                             adc_thre_group_a                          
 *                                        - NTMS_ADC_THRE_DIS                        
 *                                        - NTMS_ADC_THRE_ENA                 
 * @arg                             adc_thre_group_b                          
 *                                        - NTMS_ADC_THRE_DIS                        
 *                                        - NTMS_ADC_THRE_ENA                 
 * @arg                             adc_direct_group_a                        
 *                                        - NTMS_ADC_DIRECT_DIS                      
 *                                        - NTMS_ADC_DIRECT_ENA               
 * @arg                             adc_direct_group_b                        
 *                                        - NTMS_ADC_DIRECT_DIS                      
 *                                        - NTMS_ADC_DIRECT_ENA               
 * @return      None
 */
void ntms_init(uint32_t TimerCh, const NtmsInitType *init)
{
    /* Set timer mode */
    write_reg32(NTMS_BASE[TimerCh]->TMD, ((uint32_t)(init->timer_mode         & 0x07U) << 1)  | 
                                         ((uint32_t)1U                                 << 4)  |
                                         ((uint32_t)(init->buffer_tha         & 0x01U) << 16) | 
                                         ((uint32_t)(init->buffer_dead_time   & 0x01U) << 18) | 
                                         ((uint32_t)(init->buffer_period      & 0x03U) << 19) | 
                                         ((uint32_t)(init->buffer_adc_group_a & 0x03U) << 21) | 
                                         ((uint32_t)(init->buffer_adc_group_b & 0x03U) << 23) );

    /* Set timer  mode control */
    write_reg32(NTMS_BASE[TimerCh]->TMCTL, ((uint32_t)(init->counter_stop_mode & 0x01U) << 1)  | 
                                           ((uint32_t)(init->stop_init         & 0x01U) << 4)  | 
                                           ((uint32_t)(init->swap_out_p        & 0x01U) << 16) | 
                                           ((uint32_t)(init->swap_out_n        & 0x01U) << 17) );
    
    /* Set timer  trigger */
    write_reg32(NTMS_BASE[TimerCh]->TMTG,  (uint32_t)(NTMS_TRG_DIS)                         | 
                                           (uint32_t)(NTMS_TRG_DIS << 4)                    | 
                                           (uint32_t)(NTMS_TRG_DIS << 8)                    | 
                                           (uint32_t)(NTMS_TRG_DIS << 12)                   | 
                                           ((uint32_t)(init->trig_emergency & 0x07U) << 16) | 
                                           ((uint32_t)(init->trig_sync_emg  & 0x01U) << 19) | 
                                           ((uint32_t)NTMS_TRG_DIS << 20) );
    
    /* Set timer emergency stop */
    write_reg32(NTMS_BASE[TimerCh]->TMGM,   (uint32_t)(init->emergency_stop            & 0x01U)        | 
                                           ((uint32_t)(init->emergency_recovery_mode   & 0x01U) << 1)  | 
                                           ((uint32_t)(init->emergency_start_mode      & 0x01U) << 2)  | 
                                           ((uint32_t)(init->emergency_out_n           & 0x03U) << 8)  | 
                                           ((uint32_t)(init->emergency_out_p           & 0x03U) << 10) | 
                                           ((uint32_t)(init->emergency_dead_time       & 0x03U) << 12) | 
                                           ((uint32_t)(init->emergency_dead_time_out_p & 0x01U) << 14) | 
                                           ((uint32_t)(init->emergency_dead_time_out_n & 0x01U) << 15) );
    
    /* Set AD Timer Conversion Control */
    write_reg32(NTMS_BASE[TimerCh]->TMADCT,   (uint32_t)(init->adc_thin_group_a   & 0x01U)        | 
                                             ((uint32_t)(init->adc_thin_group_b   & 0x01U) << 1)  | 
                                             ((uint32_t)(init->adc_thin_num       & 0x07U) << 4)  | 
                                             ((uint32_t)(init->adc_thin_timing    & 0x03U) << 7)  | 
                                             ((uint32_t)(init->adc_thre_group_a   & 0x01U) << 16) | 
                                             ((uint32_t)(init->adc_thre_group_b   & 0x01U) << 17) | 
                                             ((uint32_t)(init->adc_direct_group_a & 0x01U) << 24) | 
                                             ((uint32_t)(init->adc_direct_group_b & 0x01U) << 25) );
}

/**
 * Set the state of the 3-phase motor control PWM when the CPU is stopped by soft debug break, etc.
 *
 * @param[in]   *param     Setting parameters (The following logical OR)
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS0_ENA
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS0_DIS
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS1_ENA
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS1_DIS
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS2_ENA
 * @arg                               NTMS_TMBRK_CNTSTP_NTMS2_DIS 
 * @return      None
 */
void ntms_setTimerBreak(uint32_t param)
{
    write_reg32( TMR0_COMMON->TMBRK, (0x8000U | param) );
}

/**
 * Set the emergency stop trigger for the 3-phase motor control PWM.
 *
 * @param[in]   *param     Setting parameters
 * @arg                             trig_source_0
 *                                      - NTMS_EMGTRG_SRC_EXI0TGO
 *                                      - NTMS_EMGTRG_SRC_EXI1TGO
 *                                      - NTMS_EMGTRG_SRC_EXI2TGO
 *                                      - NTMS_EMGTRG_SRC_EXI3TGO
 *                                      - NTMS_EMGTRG_SRC_EXI4TGO
 *                                      - NTMS_EMGTRG_SRC_EXI5TGO
 *                                      - NTMS_EMGTRG_SRC_EXI6TGO
 *                                      - NTMS_EMGTRG_SRC_EXI7TGO
 *                                      - NTMS_EMGTRG_SRC_CMP0TRG
 *                                      - NTMS_EMGTRG_SRC_CMP1TRG
 *                                      - NTMS_EMGTRG_SRC_CMP2TRG 
 * @arg                             trig_source_1
 *                                      - NTMS_EMGTRG_SRC_EXI0TGO
 *                                      - NTMS_EMGTRG_SRC_EXI1TGO
 *                                      - NTMS_EMGTRG_SRC_EXI2TGO
 *                                      - NTMS_EMGTRG_SRC_EXI3TGO
 *                                      - NTMS_EMGTRG_SRC_EXI4TGO
 *                                      - NTMS_EMGTRG_SRC_EXI5TGO
 *                                      - NTMS_EMGTRG_SRC_EXI6TGO
 *                                      - NTMS_EMGTRG_SRC_EXI7TGO
 *                                      - NTMS_EMGTRG_SRC_CMP0TRG
 *                                      - NTMS_EMGTRG_SRC_CMP1TRG
 *                                      - NTMS_EMGTRG_SRC_CMP2TRG 
 * @arg                             trig_0
 *                                      - NTMS_EMGTRG_DIS
 *                                      - NTMS_EMGTRG_ENA
 * @arg                             trig_1
 *                                      - NTMS_EMGTRG_DIS
 *                                      - NTMS_EMGTRG_ENA
 * @arg                             trig_swap_0
 *                                      - NTMS_EMGTRG_SWAP_DIS
 *                                      - NTMS_EMGTRG_SWAP_ENA
 * @arg                             trig_swap_1
 *                                      - NTMS_EMGTRG_SWAP_DIS
 *                                      - NTMS_EMGTRG_SWAP_ENA
 * @arg                             trig_ctrl_mode_0
 *                                      - NTMS_EMGTRG_CTRL_LEVEL
 *                                      - NTMS_EMGTRG_CTRL_EDGE
 * @arg                             trig_ctrl_mode_1
 *                                      - NTMS_EMGTRG_CTRL_LEVEL
 *                                      - NTMS_EMGTRG_CTRL_EDGE
 * @return      None
 */
void ntms_setEmergencyStopTrigger(const NtmsEmgCfgType *param)
{
    write_reg32(TMR0_COMMON->TMGMSEL,  (uint32_t)((uint32_t)param->trig_source_0    & 0x0FU)        | 
                                      ((uint32_t)(param->trig_0                     & 0x01U) << 4)  | 
                                      ((uint32_t)(param->trig_swap_0                & 0x01U) << 5)  | 
                                      ((uint32_t)(param->trig_ctrl_mode_0           & 0x01U) << 6)  | 
                                      ((uint32_t)((uint32_t)param->trig_source_1    & 0x0FU) << 16) | 
                                      ((uint32_t)(param->trig_1                     & 0x01U) << 20) | 
                                      ((uint32_t)(param->trig_swap_1                & 0x01U) << 21) | 
                                      ((uint32_t)(param->trig_ctrl_mode_1           & 0x01U) << 22));
}

/**
 * Set the behavior of the AD conversion start request timing value buffer for 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   AdBffMode     Setting parameters
 * @arg                                 NTMS_BFFMODE_ENA
 * @arg                                 NTMS_BFFMODE_DIS
 * @return      None
 */
void ntms_setAdcBuffer(uint32_t TimerCh, uint32_t AdBffMode)
{
    uint32_t data = 0U;
    
    if(TimerCh == NTMS_TIMER_NTMS2) {
        data = (AdBffMode & 0x01U) << 11;
    }
    else if(TimerCh == NTMS_TIMER_NTMS1) {
        data = (AdBffMode & 0x01U) << 7;
    }
    else {
        data = (AdBffMode & 0x01U) << 3;
    }
    
    set_bit(TMR0_COMMON->TMBE, data);
}

/**
 * Set the behavior of the periodic buffer for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   AdBffMode     Setting parameters
 * @arg                                 NTMS_BFFMODE_ENA
 * @arg                                 NTMS_BFFMODE_DIS
 * @return      None
 */
void ntms_setPeriodBuffer(uint32_t TimerCh, uint32_t BffMode)
{
    uint32_t data = 0U;
    
    if(TimerCh == NTMS_TIMER_NTMS2) {
        data = (BffMode & 0x01U) << 8;
    }
    else if(TimerCh == NTMS_TIMER_NTMS1) {
        data = (BffMode & 0x01U) << 4;
    }
    else {
        data = (BffMode & 0x01U);
    }
    
    set_bit(TMR0_COMMON->TMBE, data);
}

/**
 * Set the behavior of the waveform threshold buffer for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   AdBffMode     Setting parameters
 * @arg                                 NTMS_BFFMODE_ENA
 * @arg                                 NTMS_BFFMODE_DIS
 * @return      None
 */
void ntms_setThreABBuffer(uint32_t TimerCh, uint32_t BffMode)
{
    uint32_t data = 0U;
    
    if(TimerCh == NTMS_TIMER_NTMS2) {
        data = (BffMode & 0x01U) << 9;
    }
    else if(TimerCh == NTMS_TIMER_NTMS1) {
        data = (BffMode & 0x01U) << 5;
    }
    else {
        data = (BffMode & 0x01U) << 1;
    }
    
    set_bit(TMR0_COMMON->TMBE, data);
}

/**
 * Set the behavior of the waveform dead time value buffer for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   AdBffMode     Setting parameters
 * @arg                                 NTMS_BFFMODE_ENA
 * @arg                                 NTMS_BFFMODE_DIS
 * @return      None
 */
void ntms_setDeadTimeBuffer(uint32_t TimerCh, uint32_t BffMode)
{
    uint32_t data = 0U;
    
    if(TimerCh == NTMS_TIMER_NTMS2) {
        data = (BffMode & 0x01U) << 10;
    }
    else if(TimerCh == NTMS_TIMER_NTMS1) {
        data = (BffMode & 0x01U) << 6;
    }
    else {
        data = (BffMode & 0x01U) << 2;
    }
    
    set_bit(TMR0_COMMON->TMBE, data);
}

/**
 * Set the period of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   Period        Setting parameters
 * @arg                                 0x0001 - 0xFFFF (Period: 2 - 65536)
 * @return      None
 */
void ntms_setTimerPeriod(uint32_t TimerCh, uint32_t Period)
{
    write_reg32(NTMS_BASE[TimerCh]->TMLEN, Period - 1U);
}

/**
 * Get the period setting value of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Timer period                            
 * @arg             (0x0001 - 0xFFFF (Period: 2 - 65536))
 */
uint32_t ntms_getTimerPeriod(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMLEN) + 1U;
}

/**
 * Set the period in the periodic buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   Period        Setting parameters
 * @arg                                 0x0001 - 0xFFFF (Period: 2 - 65536)
 * @return      None
 */
void ntms_setTimerPeriodBff(uint32_t TimerCh, uint32_t Period)
{
    write_reg32(NTMS_BASE[TimerCh]->TMLENB, Period - 1U);
}

/**
 * Get the period from the periodic buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Timer period
 * @arg             (0x0001 - 0xFFFF (Period: 2 - 65536))
 */
uint32_t ntms_getTimerPeriodBff(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMLENB) + 1U;
}

/**
 * Get the counter value of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Timer period                            
 * @arg             (0x0000 - 0xFFFF)
 */
uint32_t ntms_getTimerCount(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMMON);
}

/**
 * Clear the counter value of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      None
 */
void ntms_clearTimerCount(uint32_t TimerCh)
{
    /* Check the operation status of the timer */
    if((ntms_getTimerOprtStatus() & ntms_channelToBitPos(TimerCh)) != 0) {
        /* When the timer is in operation */
        ntms_start(TimerCh);
    }
    else {
        /* When the timer is not in operation */
        write_reg32(NTMS_BASE[TimerCh]->TMMON, 0U);
    }
}

/**
 * Get the status of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Timer status                            
 * @arg                Logical OR of the values indicating the following statuses:                      
 *                          - Up/down counter identification (NTMS_STATUS_UPCOUNT/NTMS_STATUS_DOWNCOUNT)
 *                          - AD conversion decimation count counter (0x0 - 0x7)                        
 */
uint32_t ntms_getTimerStatus(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMEST);
}

/**
 * Set the waveform threshold THA0 for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tha0          Waveform threshold THA0
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of THA0 and THA1 in the user's manual.
 * @return      None
 */
void ntms_setTHA0(uint32_t TimerCh, uint32_t tha0)
{
    write_reg32(NTMS_BASE[TimerCh]->TMTHA0, tha0);
}

/**
 * Set the waveform threshold THA1 for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tha1          Waveform threshold THA1
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of THA0 and THA1 in the user's manual.
 * @return      None
 */
void ntms_setTHA1(uint32_t TimerCh, uint32_t tha1)
{
    write_reg32(NTMS_BASE[TimerCh]->TMTHA1, tha1);
}

/**
 * Set the threshold THA0 for the waveform threshold buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tha0          Waveform threshold THA0
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of THA0 and THA1 in the user's manual.
 * @return      None
 */
void ntms_setTHA0Bff(uint32_t TimerCh, uint32_t tha0)
{
    write_reg32(NTMS_BASE[TimerCh]->TMTHA0B, tha0);
}

/**
 * Set the threshold THA1 for the waveform threshold buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tha1          Waveform threshold THA1
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of THA0 and THA1 in the user's manual of MCU.
 * @return      None
 */
void ntms_setTHA1Bff(uint32_t TimerCh, uint32_t tha1)
{
    write_reg32(NTMS_BASE[TimerCh]->TMTHA1B, tha1);
}

/**
 * Set the interrupt mask for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   interrupt     Interrupt permission settings (The following logical OR)
 * @arg                             For enable interrupts:  
 *                                    - NTMS_INT_CMP_ENA
 *                                    - NTMS_INT_MOU_ENA
 *                                    - NTMS_INT_TA0_ENA
 *                                    - NTMS_INT_TA1_ENA
 *                                    - NTMS_INT_TB0_ENA
 *                                    - NTMS_INT_TB1_ENA
 *                                    - NTMS_INT_DTB0_ENA
 *                                    - NTMS_INT_DTB1_ENA
 * @arg                              For disable interrupts:
 *                                    - NTMS_INT_CMP_DIS
 *                                    - NTMS_INT_MOU_DIS
 *                                    - NTMS_INT_TA0_DIS
 *                                    - NTMS_INT_TA1_DIS
 *                                    - NTMS_INT_TB0_DIS
 *                                    - NTMS_INT_TB1_DIS
 *                                    - NTMS_INT_DTB0_DIS
 *                                    - NTMS_INT_DTB1_DIS
 * @return      None
 */
void ntms_setIntMask(uint32_t TimerCh, uint32_t interrupt)
{
    uint32_t data = 0x7F80U | interrupt;
    
    write_reg32(NTMS_BASE[TimerCh]->TMIMSK, data);
}

/**
 * Get the interrupt status of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Interrupt status
 * @arg                          The following Logical OR      
 *                                    - NTMS_INT_STATUS_CMP
 *                                    - NTMS_INT_STATUS_MOU
 *                                    - NTMS_INT_STATUS_TA0
 *                                    - NTMS_INT_STATUS_TA1
 *                                    - NTMS_INT_STATUS_TB0
 *                                    - NTMS_INT_STATUS_TB1
 *                                    - NTMS_INT_STATUS_DTB0
 *                                    - NTMS_INT_STATUS_DTB1
 */
uint32_t ntms_getIntStatus(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMISTS);
}

/**
 * Clear the interrupt status of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   status        Interrupt status 
 * @arg                          The following logical OR
 *                                    - NTMS_INT_STATUS_CMP
 *                                    - NTMS_INT_STATUS_MOU
 *                                    - NTMS_INT_STATUS_TA0
 *                                    - NTMS_INT_STATUS_TA1
 *                                    - NTMS_INT_STATUS_TB0
 *                                    - NTMS_INT_STATUS_TB1
 *                                    - NTMS_INT_STATUS_DTB0
 *                                    - NTMS_INT_STATUS_DTB1
 * @return      None
 */
void ntms_clearIntStatus(uint32_t TimerCh, uint32_t status)
{
    write_reg32(NTMS_BASE[TimerCh]->TMISTR, status);
}

/**
 * Get the interrupt status of the 3-phase motor control PWM (before masking).
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Interrupt status
 * @arg                          The following Logical OR      
 *                                    - NTMS_INT_STATUS_CMP
 *                                    - NTMS_INT_STATUS_MOU
 *                                    - NTMS_INT_STATUS_TA0
 *                                    - NTMS_INT_STATUS_TA1
 *                                    - NTMS_INT_STATUS_TB0
 *                                    - NTMS_INT_STATUS_TB1
 *                                    - NTMS_INT_STATUS_DTB0
 *                                    - NTMS_INT_STATUS_DTB1
 */
uint32_t ntms_getRawIntStatus(uint32_t TimerCh)
{
    return read_reg32(NTMS_BASE[TimerCh]->TMISTR);
}

/**
 * Set the waveform dead time U for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   DeadTimeU     Waveform dead time U (TMDBU)
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of TMDBU, TMDBD, TMTHA0, TMTHA1 and TMLEN in the user's manual of MCU.
 * @return      None
 */
void ntms_setDeadTimeU(uint32_t TimerCh, uint32_t DeadTimeU)
{
    write_reg32(NTMS_BASE[TimerCh]->TMDBU, DeadTimeU);
}

/**
 * Set the waveform dead time D for the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   DeadTimeD     Waveform dead time D (TMDBD)
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of TMDBU, TMDBD, TMTHA0, TMTHA1 and TMLEN in the user's manual of MCU.
 * @return      None
 */

void ntms_setDeadTimeD(uint32_t TimerCh, uint32_t DeadTimeD)
{
    write_reg32(NTMS_BASE[TimerCh]->TMDBD, DeadTimeD);
}

/**
 * Set the waveform dead time U in the waveform dead time value buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   DeadTimeU     Waveform dead time U (TMDBU)
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of TMDBU, TMDBD, TMTHA0, TMTHA1 and TMLEN in the user's manual of MCU.
 * @return      None
 */
void ntms_setDeadTimeUBff(uint32_t TimerCh, uint32_t DeadTimeU)
{
    write_reg32(NTMS_BASE[TimerCh]->TMDBUB, DeadTimeU);
}

/**
 * Set the waveform dead time D in the waveform dead time value buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   DeadTimeD     Waveform dead time D (TMDBD)
 * @arg                                 0x0000 - 0xFFFF
 * @note                                Check the limitations of TMDBU, TMDBD, TMTHA0, TMTHA1 and TMLEN in the user's manual of MCU.
 * @return      None
 */
void ntms_setDeadTimeDBff(uint32_t TimerCh, uint32_t DeadTimeD)
{
    write_reg32(NTMS_BASE[TimerCh]->TMDBDB, DeadTimeD);
}

/**
 * Start the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      None
 */
void ntms_start(uint32_t TimerCh)
{
    write_reg32(TMR0_COMMON->TMON, ntms_channelToBitPos(TimerCh));
}

/**
 * Stop the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      None
 */
void ntms_stop(uint32_t TimerCh)
{
    write_reg32(TMR0_COMMON->TMOFF, ntms_channelToBitPos(TimerCh));
}

/**
 * Start multiple 3-phase motor control PWMs.
 *
 * @param[in]   TargetTimer   Target timer (bit specification)
 * @arg                                 NTMS_TIMER_BIT_NTMS0
 * @arg                                 NTMS_TIMER_BIT_NTMS1
 * @arg                                 NTMS_TIMER_BIT_NTMS2
 * @return      None
 */
void ntms_startBit(uint32_t TargetTimer)
{
    write_reg32(TMR0_COMMON->TMON, TargetTimer);
}

/**
 * Stop multiple 3-phase motor control PWMs.
 *
 * @param[in]   TargetTimer   Target timer (bit specification)
 * @arg                                 NTMS_TIMER_BIT_NTMS0
 * @arg                                 NTMS_TIMER_BIT_NTMS1
 * @arg                                 NTMS_TIMER_BIT_NTMS2
 * @return      None
 */
void ntms_stopBit(uint32_t TargetTimer)
{
    write_reg32(TMR0_COMMON->TMOFF, TargetTimer);
}

/**
 * Get the operating status of the 3-phase motor control PWM (the value of the TMON register).
 *
 * @param       -
 * @return      Timer operating status
 * @arg                       The following Logical OR
 *                                    - NTMS_TIMER_BIT_NTMS0
 *                                    - NTMS_TIMER_BIT_NTMS1
 *                                    - NTMS_TIMER_BIT_NTMS2
 */
uint32_t ntms_getTimerOprtStatus(void)
{
    return read_reg32(TMR0_COMMON->TMON);
}

/**
 * Start emergency processing of 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      None
 */
void ntms_startEmgProc(uint32_t TimerCh)
{
    write_reg32(TMR0_COMMON->TMES, ntms_channelToBitPos(TimerCh));
}

/**
 * Stop emergency processing of 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      None
 */
void ntms_stopEmgProc(uint32_t TimerCh)
{
    write_reg32(TMR0_COMMON->TMEE, ntms_channelToBitPos(TimerCh));
}

/**
 * Start emergency processing of multiple 3-phase motor control PWMs.
 *
 * @param[in]   TargetTimer   Target timer (bit specification)
 * @arg                                 NTMS_TIMER_BIT_NTMS0
 * @arg                                 NTMS_TIMER_BIT_NTMS1
 * @arg                                 NTMS_TIMER_BIT_NTMS2
 * @return      None
 */
void ntms_startBitEmgProc(uint32_t TargetTimer)
{
    write_reg32(TMR0_COMMON->TMES, TargetTimer);
}

/**
 * Stop emergency processing of multiple 3-phase motor control PWMs.
 *
 * @param[in]   TargetTimer   Target timer (bit specification)
 * @arg                                 NTMS_TIMER_BIT_NTMS0
 * @arg                                 NTMS_TIMER_BIT_NTMS1
 * @arg                                 NTMS_TIMER_BIT_NTMS2
 * @return      None
 */
void ntms_stopBitEmgProc(uint32_t TargetTimer)
{
    write_reg32(TMR0_COMMON->TMEE, TargetTimer);
}

/**
 * Get the emergency processing state of the 3-phase motor control PWM (the value of the TMES register).
 *
 * @param       -
 * @return      Timer operating status
 * @arg                       The following Logical OR
 *                                    - NTMS_TIMER_BIT_NTMS0
 *                                    - NTMS_TIMER_BIT_NTMS1
 *                                    - NTMS_TIMER_BIT_NTMS2
 *                                    .
 *                            "1": Emergency stop in progress
 */
uint32_t ntms_getEmgProcStatus(void)
{
    return read_reg32(TMR0_COMMON->TMES);
}

/**
 * Get the emergency stop trigger state of the 3-phase motor control PWM.
 *
 * @param       -
 * @return      Trigger signal level or emergency stop request status                                                                                                           
 *                         - Bit0:                                                                                                                                              
 *                            - When "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger(), indicate that the status of SEL0 is at the Emergency Stop trigger level.
 *                                    - NTMS_EMGTRG_ST_SEL0_LEVEL_LOW
 *                                    - NTMS_EMGTRG_ST_SEL0_LEVEL_HIGH
 *                            - When "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger(), indicate that the status of SEL0 is Emergency Stop Request.              
 *                                    - NTMS_EMGTRG_ST_SEL0_NO_REQUEST
 *                                    - NTMS_EMGTRG_ST_SEL0_REQUESTED
 *                         - Bit1:                                                                                                                                              
 *                            - When "NTMS_EMGTRG_CTRL_LEVEL" is set in ntms_setEmergencyStopTrigger(), indicate that the status of SEL1 is at the Emergency Stop trigger level.
 *                                    - NTMS_EMGTRG_ST_SEL1_LEVEL_LOW
 *                                    - NTMS_EMGTRG_ST_SEL1_LEVEL_HIGH
 *                            - When "NTMS_EMGTRG_CTRL_EDGE" is set in ntms_setEmergencyStopTrigger(), indicate that the status of SEL1 is Emergency Stop Request.              
 *                                    - NTMS_EMGTRG_ST_SEL1_NO_REQUEST
 *                                    - NTMS_EMGTRG_ST_SEL1_REQUESTED
 */
uint32_t ntms_getEmgTrigStatus(void)
{
    return read_reg32(TMR0_COMMON->TMGMC);
}

/**
 * Clear the emergency stop trigger for the 3-phase motor control PWM.
 * When the emergency stop trigger is cleared, the emergency stop condition is released.
 *
 * @param       EtsNum       Emergency Stop Trigger (The following logical OR)
 * @arg                                 NTMS_EMGTRG_ST_SEL0_REQUESTED
 * @arg                                 NTMS_EMGTRG_ST_SEL1_REQUESTED
 * @return      None
 */
void ntms_clearEmgTrigStatus(uint32_t EtsNum)
{
    write_reg32(TMR0_COMMON->TMGMC, EtsNum);
}

/**
 * Set the threshold of the 3-phase motor control PWM for the AD conversion start request of group A.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tmada         Threshold "A" for AD conversion start request
 * @arg                                 0x0000 - 0xFFFF
 * @return      None
 */
void ntms_setTMADA(uint32_t TimerCh, uint32_t tmada)
{
    write_reg32(NTMS_BASE[TimerCh]->TMADA, tmada);
}

/**
 * Set the threshold of the 3-phase motor control PWM for the AD conversion start request of group B.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tmadb         Threshold "B" for AD conversion start request
 * @arg                                 0x0000 - 0xFFFF
 * @return      None
 */
void ntms_setTMADB(uint32_t TimerCh, uint32_t tmadb)
{
    write_reg32(NTMS_BASE[TimerCh]->TMADB, tmadb);
}

/**
 * Set the threshold value for the AD conversion start request of group A to the AD conversion start request timing buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tmada         Threshold "A" for AD conversion start request
 * @arg                                 0x0000 - 0xFFFF
 * @return      None
 */
void ntms_setTMADABff(uint32_t TimerCh, uint32_t tmada)
{
    write_reg32(NTMS_BASE[TimerCh]->TMADAB, tmada);
}

/**
 * Set the threshold value for the AD conversion start request of group B to the AD conversion start request timing buffer of the 3-phase motor control PWM.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @param[in]   tmada         Threshold "B" for AD conversion start request
 * @arg                                 0x0000 - 0xFFFF
 * @return      None
 */
void ntms_setTMADBBff(uint32_t TimerCh, uint32_t tmadb)
{
    write_reg32(NTMS_BASE[TimerCh]->TMADBB, tmadb);
}

/**
 * Convert the channel number of the timer to a bit pattern.
 *
 * @param[in]   TimerCh       Timer channel
 * @arg                                 NTMS_TIMER_NTMS0
 * @arg                                 NTMS_TIMER_NTMS1
 * @arg                                 NTMS_TIMER_NTMS2
 * @return      Bit Pattern   The following bit pattern
 *                                    - NTMS_TIMER_BIT_NTMS0
 *                                    - NTMS_TIMER_BIT_NTMS1
 *                                    - NTMS_TIMER_BIT_NTMS2
 */
uint32_t ntms_channelToBitPos(uint32_t TimerCh)
{
    return (uint32_t)(1U << (TimerCh + 16U));
}

/** @} */
/** @} */
