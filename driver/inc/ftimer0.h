/*****************************************************************************
 ftimer0.h

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
 * @file    ftimer0.h
 *
 * This module is 'FTimer0' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup FTimer
 *  @{
 *
 *  @brief Functional Timer register related
 *
 *  @file
 */

#ifndef FTIMER0_H__
#define FTIMER0_H__

#include "mcu.h"
#include "rdwr_reg.h"
#include "ftimer_common.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Initialize FTM
 *
 * @param[in]   ftnmod      Configuration FTM mode                              <br>
 *                          (Setting value of FT0MOD register)                  <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Mode                                : FTM_MD_TIMER, or FTM_MD_CAPTURE, or FTM_MD_PWM1, or FTM_MD_PWM2
 * @arg                         Dead time disable/enable            : FTM_DTEN_DIS, or FTM_DTEN_ENA
 * @arg                         One-shout mode                                                                        <br>
 *                                  [for Timer/PWM]                 : FTM_OST_REROAD, or FTM_OST_ONESHOT              <br>
 *                                  [for Capture]                   : FTM_OST_CAP_AUTO, or FTM_OST_CAP_SINGLE
 * @arg                         Output signal when FTM is stoped : FTM_STPO_LOW, or FTM_STPO_KEEP
 *
 * @param[in]   ftnclk      Configuration clock                                 <br>
 *                          (Setting value of FT0CLK register)                  <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Clock source                        : FTM_CK_LSCLK, or FTM_CK_OSCLK, or FTM_CK_HSCLK, or FTM_CK_EXTCLK
 * @arg                         Division ratio                      : FTM_CKD_DIV1, or FTM_CKD_DIV2, ( ... ), or FTM_CKD_DIV32, or FTM_CKD_DIV64
 * @arg                         External clock source               : FTM_XCK_TMCKI0, or FTM_XCK_TMCKI1,( ... ), or FTM_XCK_TMCKI6, or FTM_XCK_TMCKI7
 * @return      None
 *
 */
#define         ftimer0_init( ftnmod, ftnclk )                  do {                                                    \
                                                                    write_reg32( FTIMER->FT0MOD, ftnmod );    \
                                                                    write_reg32( FTIMER->FT0CLK, ftnclk );    \
                                                                } while (0)

/**
 * Setting trigger parameters
 *
 * @param[in]   ftntrg0     Trigger parameter (1)                               <br>
 *                          (Setting value of FT0TRG0 register)                 <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Counter start                         : FTM_ST0_DIS,  or FTM_ST0_ENA
 * @arg                         Counter stop                          : FTM_ST1_DIS,  or FTM_ST1_ENA
 * @arg                         Counter clear                         : FTM_EXCL_DIS, or FTM_EXCL_ENA
 * @arg                         Counter start when counting is stoped : FTM_CST_DIS,  or FTM_CST_ENA
 * @arg                         Trigger event source                  : FTM_STS_EXI0, or FTM_STS_EXI1, ( ... ), or FTM_STS_FTM0, or FTM_STS_FTM1
 *
 * @param[in]   ftntrg1     Trigger parameter (2)                               <br>
 *                          (Setting value of FT0TRG1 register)                 <br>
 *                          Specify the logical sum of the following items.
 * @arg                         Trigger event edge                  : FTM_TRM_EDGE_RISE_RISE, or FTM_TRM_EDGE_FALL_RISE, or FTM_TRM_EDGE_RISE_FALL, or FTM_TRM_EDGE_FALL_FALL
 * @arg                         Emergency trigger source            : FTM_EST_EXI0, or FTM_EST_EXI4, or FTM_EST_CMP0, or FTM_EST_CMP1
 * @return      None
 */
#define         ftimer0_setTriggerParam( ftntrg0, ftntrg1 )     do {                                                    \
                                                                    write_reg32( FTIMER->FT0TRG, (ftntrg0) | (ftntrg1) );\
                                                                } while (0)

/**
 * Setting interrupt
 *
 * @param[in]   ftninte     Disable/enable interupt                             <br>
 *                          (Setting value of FT0INTE register)                 <br>
 *                          Specify the logical sum of the following items.
 * @arg                         FTnIEP  : FTM_IEP_DIS,  or FTM_IEP_ENA
 * @arg                         FTnIEA  : FTM_IEA_DIS,  or FTM_IEA_ENA
 * @arg                         FTnIEB  : FTM_IEB_DIS,  or FTM_IEB_ENA
 * @arg                         FTnIETS : FTM_IETS_DIS, or FTM_IETS_ENA
 * @arg                         FTnIETR : FTM_IETR_DIS, or FTM_IETR_ENA
 * @arg                         FTnIOP  : FTM_IOP_DIS, or FTM_IOP_ENA
 * @arg                         FTnIOA  : FTM_IOA_DIS, or FTM_IOA_ENA
 * @arg                         FTnIOB  : FTM_IOB_DIS, or FTM_IOB_ENA
 *
 * @return      None
 */
#define         ftimer0_setInt( ftninte )       write_reg32( FTIMER->FT0INTE, ftninte )

/**
 * Setting cycle counter
 *
 * @param[in]   ftnp        Cycle of timer counting                             <br>
 *                          (Setting value of FT0P register)                    <br>
 *                          Set this value form 1 to 0xFFFF.                    <br>
 * 
 * @return      None
 */
#define         ftimer0_setCycle( ftnp )        write_reg32( FTIMER->FT0P, ftnp )

/**
 * Setting event counter A
 *
 * @param[in]   ftnea       Setting value of FT0EA register                     <br>
 *                              [for Timer]    : the count of interrupt trigger <br>
 *                              [for PWM]      : the duty of TMOUT output       <br>
 *                              [for Capture]  : invalid
 * 
 * @return      None
 */
#define         ftimer0_setEventCntA( ftnea )   write_reg32( FTIMER->FT0EA, ftnea )

/**
 * Setting event counter B
 *
 * @param[in]   ftneb       Setting value of FT0EB register                         <br>
 *                              [for Timer]     : the count of interrupt trigger    <br>
 *                              [for PWM]       : the duty of TMOUT output          <br>
 *                              [for Capture]   : invalid
 * 
 * @return      None
 */
#define         ftimer0_setEventCntB( ftneb )   write_reg32( FTIMER->FT0EB, ftneb )

/**
 * Setting DeadTime
 *
 * @param[in]   ftndt       Setting value of FT0DT register                                     <br>
 *                              [for Timer/PWM] : the count of DeadTime ( 0 : Not-DeadTime )    <br>
 *                              [for Capture]   : invalid
 * @return      None
 */
#define         ftimer0_setDeadtime( ftndt )    write_reg32( FTIMER->FT0DT, ftndt )

/**
 * Start FTM
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_start()                 set_bit( FTIMER->FT0CON0, (1 << 0) )

/**
 * Stop FTM
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_stop()                  clear_bit( FTIMER->FT0CON0, (1 << 0) )

/**
 * Getting timer counter value
 *
 * @param       -
 * @return      The value of FTM counter     <br>
 *              (A value of FT0C register)      <br>
 */
#define         ftimer0_getCnt()                read_reg32( FTIMER->FT0C )

/**
 * Clear timer counter value
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_clrCnt()                write_reg32( FTIMER->FT0C, 0UL )

/**
 * Getting event (A) counter value
 *
 * @param       -
 * @return      The value of capture result     <br>
 *              (A value of FT0EA register)
 */
#define         ftimer0_getEventCntA()          read_reg32( FTIMER->FT0EA )

/**
 * Getting event (B) counter value
 *
 * @param       -
 * @return      The value of capture result     <br>
 *              (A value of FT0EB register)
 */
#define         ftimer0_getEventCntB()          read_reg32( FTIMER->FT0EB )

/**
 * Trig to update FTM parameters
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_trigUpdateDutyCycle()   set_bit( FTIMER->FT0CON1, (1 << 0) )

/**
 * Checking to update FTM parameters
 *
 * @param       -
 * @retval      0               status : Normal(able to update)
 * @retval      "other than 0"  status : Busy(invalid updating)
 */
#define         ftimer0_checkEnaUpdate()        ( get_bit( FTIMER->FT0CON1, (1 << 0) ) )

/**
 * Getting FTM status(FTnCON0)
 *
 * @param       -
 * @return      Status                          <br>
 *              (A value of FT0CON0 register)
 */
#define         ftimer0_getCon0Status()         read_reg32( FTIMER->FT0CON0 )

/**
 * Getting FTM status(FTnCON1)
 *
 * @param       -
 * @return      Status                          <br>
 *              (A value of FT0CON1 register)
 */
#define         ftimer0_getCon1Status()         read_reg32( FTIMER->FT0CON1 )

/**
 * Getting interrupt cause
 *
 * Get causes which FTM interrupt is requested.
 *
 * @param       -
 * @return      Interrupt causes <br>
 *              (A value of FT0INTS register)
 */
#define         ftimer0_getIntCause()           read_reg32( FTIMER->FT0INTS )

/**
 * Clear interrupt cause
 *
 * @param[in]   ftnintc     Mask value to clear interupt cause                  <br>
 *                          (Setting value of FT0INTC register)                 <br>
 *                          Specify the logical sum of the following items, or logical sum of FTnINTC_ALL_CLEAR and FTnINTC_FTnIR.
 * @arg                         FTnICP  : FTnINTC_FTnICP
 * @arg                         FTnICA  : FTnINTC_FTnICA
 * @arg                         FTnICB  : FTnINTC_FTnICB
 * @arg                         FTnICTS : FTnINTC_FTnICTS
 * @arg                         FTnICTR : FTnINTC_FTnICTR
 * @arg                         FTnICES : FTnINTC_FTnICES
 * @arg                         FTnIR   : FTnINTC_FTnIR
 *
 * @return      None
 */
#define         ftimer0_clearIntCause( ftnintc ) write_reg32( FTIMER->FT0INTC, ftnintc )

/**
 * Enable trigger
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_enaTrigger()            set_bit( FTIMER->FT0CON0, (1 << 4) )

/**
 * Disble trigger
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_disTrigger()            clear_bit( FTIMER->FT0CON0, (1 << 4) )

/**
 * Enable emergency stop
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_enaEmergencyStop()      set_bit( FTIMER->FT0CON0, (1 << 8) )

/**
 * Disble emergency stop
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_disEmergencyStop()      clear_bit( FTIMER->FT0CON0, (1 << 8) )

/**
 * Enable output(Disable mask the FTM output)
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_enaOutput()             clear_bit( FTIMER->FT0CON0, (1 << 12) )

/**
 * Disable output(Enable mask the FTM output)
 *
 * @param       -
 * @return      None
 */
#define         ftimer0_disOutput()             set_bit( FTIMER->FT0CON0, (1 << 12) )


#endif /*FTIMER0_H__*/

/** @} */
/** @} */
