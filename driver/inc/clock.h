/*****************************************************************************
 clock.h

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
 * @file    clock.h
 *
 * This module is 'Clock' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup Clock
 *  @{
 *
 *  @brief Clock Generation Circuit register related 
 *
 *  @file
 */

#ifndef CLOCK_H__
#define CLOCK_H__

#include "mcu.h"
#include "rdwr_reg.h"

/*############################################################################*/
/*#                                  Macro                                   #*/
/*############################################################################*/
/*=== bit field of registers ===*/
/* FCON0 */
#define FCON0_SYSCO         ( 0x01 )        /**< FCON0 Register SYSCO bit     */
#define FCON0_SYSC1         ( 0x02 )        /**< FCON0 Register SYSC1 bit     */
#define FCON0_SYSC2         ( 0x04 )        /**< FCON0 Register SYSC2 bit     */
#define FCON0_OSCM0         ( 0x08 )        /**< FCON0 Register OSCMO bit     */
#define FCON0_OSCM1         ( 0x10 )        /**< FCON0 Register OSCM1 bit     */
#define FCON0_OUTC0         ( 0x20 )        /**< FCON0 Register OUTCO bit     */
#define FCON0_OUTC1         ( 0x40 )        /**< FCON0 Register OUTC1 bit     */
#define FCON0_OUTC2         ( 0x80 )        /**< FCON0 Register OUTC2 bit     */

/* FCON2 */
#define FCON2_XTMO          ( 0x01 )        /**< FCON2 Register XTMO bit      */
#define FCON2_XTM1          ( 0x02 )        /**< FCON2 Register XTM1 bit      */

/* FSTAT */
#define FSTAT_LOSCS         ( 0x04 )        /**< FSTAT Register LOSCS bit     */
#define FSTAT_L0RC          ( 0x0100U )     /**< FSTAT Register L0RC bit      */
#define FSTAT_L0XT          ( 0x0200U )     /**< FSTAT Register L0XT bit      */

/*=== API parameters value ===*/
/* HSCLK frequency                                          */
/*     Initial configuration parameters for clk_setHsclk()  */
#define CLK_SYSC_OSCLK          (                                          0 )      /**< HSCLK frequency : OSCLK        */
#define CLK_SYSC_OSCLK_DIV2     (            0 |            0 |  FCON0_SYSCO )      /**< HSCLK frequency : 1/2 OSCLK    */
#define CLK_SYSC_OSCLK_DIV4     (            0 |  FCON0_SYSC1 |            0 )      /**< HSCLK frequency : 1/4 OSCLK    */
#define CLK_SYSC_OSCLK_DIV8     (            0 |  FCON0_SYSC1 |  FCON0_SYSCO )      /**< HSCLK frequency : 1/8 OSCLK    */
#define CLK_SYSC_OSCLK_DIV16    (  FCON0_SYSC2 |            0 |            0 )      /**< HSCLK frequency : 1/16 OSCLK   */
#define CLK_SYSC_OSCLK_DIV32    (  FCON0_SYSC2 |            0 |  FCON0_SYSCO )      /**< HSCLK frequency : 1/32 OSCLK   */

/* HCKO frequency                                          */
/*     Initial configuration parameters for clk_setHsclk()  */
#define CLK_OUTC_OSCLK_DIV2     (            0 |            0 |  FCON0_OUTC0 )      /**< HCKO frequency : 1/2 OSCLK     */
#define CLK_OUTC_OSCLK_DIV4     (            0 |  FCON0_OUTC1 |            0 )      /**< HCKO frequency : 1/4 OSCLK     */
#define CLK_OUTC_OSCLK_DIV8     (            0 |  FCON0_OUTC1 |  FCON0_OUTC0 )      /**< HCKO frequency : 1/8 OSCLK     */
#define CLK_OUTC_OSCLK_DIV16    (  FCON0_OUTC2 |            0 |            0 )      /**< HCKO frequency : 1/16 OSCLK    */
#define CLK_OUTC_OSCLK_DIV32    (  FCON0_OUTC2 |            0 |  FCON0_OUTC0 )      /**< HCKO frequency : 1/32 OSCLK    */
#define CLK_OUTC_OSCLK_DIV64    (  FCON0_OUTC2 |  FCON0_OUTC1 |            0 )      /**< HCKO frequency : 1/64 OSCLK    */
#define CLK_OUTC_OSCLK_DIV128   (  FCON0_OUTC2 |  FCON0_OUTC1 |  FCON0_OUTC0 )      /**< HCKO frequency : 1/128 OSCLK   */

/* HSCLK oscillation mode                                   */
/*     Initial configuration parameters for clk_setHsclk()  */
#define CLK_OSCM_PLL            (           0 )                                     /**< HSCLK oscillation mode : PLL oscillation                */
#define CLK_OSCM_HSOSC          ( FCON0_OSCM0 )                                     /**< HSCLK oscillation mode : High speed crystal oscillation */

/* LSCLK oscillation mode                                   */
/*     Initial configuration parameters for clk_setLsclk()  */
#define CLK_XTM_CRYSTAL         ( FCON2_XTMO )                                      /**< LSCLK oscillation mode : Crystal oscillation   */
#define CLK_XTM_RC              ( FCON2_XTM1 )                                      /**< LSCLK oscillation mode : RC oscillation        */

#define CLK_LFLTSEL_DIS         ( 0x00U )                                           /**< Low-speed clock noise filter : disable */
#define CLK_LFLTSEL_ENA         ( 0x10U )                                           /**< Low-speed clock noise filter : enaable */

#define CLK_LSTPSEL_RESET       ( 0x0000U )                                         /**< Reset when the low-speed crystal oscillation stop is detected. */
#define CLK_LSTPSEL_FLAG        ( 0x2000U )                                         /**< Set Flag when the low-speed crystal oscillation stop is detected. */
#define CLK_LSTPSEL_INT         ( 0x3000U )                                         /**< Issue the interrupt when the low-speed crystal oscillation stop is detected. */

/* System clock mode                                        */
/*     Initial configuration parameters for clk_setSysclk() */
#define CLK_SYSCLK_LSCLK        ( 0 )                                               /**< System clock : LSCLK         */
#define CLK_SYSCLK_HSCLK        ( 1 )                                               /**< System clock : HSCLK         */

/* LSCLK crystal oscillation */
/*     Initial configuration parameters for clk_setHsclk()  */
#define CLK_XSPEN_DIS          ( 0 )                                                /**< Disables the low-speed crystal oscillation stop detection reset function. */
#define CLK_XSPEN_ENA          ( 1 )                                                /**< Enables the low-speed crystal oscillation stop detection reset function.  */

/* HSCLK crystal oscillation */
/*     Initial configuration parameters for clk_setHsclk()  */
#define CLK_HXSPEN_DIS         ( 0 )                                                /**< Disables the high-speed crystal oscillation stop detection reset function. */
#define CLK_HXSPEN_ENA         ( 1 )                                                /**< Enables the high-speed crystal oscillation stop detection reset function.  */

/* LSCLK crystal circuit */
/*     Initial configuration parameters for clk_setLsclkCircuit()  */
#define CLK_LOSCMD_XT32K       ( 0x00U )                                            /**< Crystal circuit source : XT32K  */
#define CLK_LOSCMD_EXT32K      ( 0x10U )                                            /**< Crystal circuit source : EXT32K */

#define CLK_LMOD_STANDARD      ( 0x00U )                                            /**< Crystal circuit mode : STANDARD */
#define CLK_LMOD_LOWPOWER      ( 0x01U )                                            /**< Crystal circuit mode : LOWPOWER */
#define CLK_LMOD_TOUGH         ( 0x02U )                                            /**< Crystal circuit mode : TOUGH    */

/* High-speed clock wakeup settings */
/*     Initial configuration parameters for clk_setHsclkControl()  */
#define CLK_FPUT_WAIT_LOCK     ( 0x0000 )                                           /**< Wait for the lock before supplying                                                   */
#define CLK_FPUT_NOT_WAIT_LOCK ( 0x0001 )                                           /**< Supply without waiting for locks                                                     */

#define CLK_FPDT_NO_TIMEOUT    ( 0x0000 )                                           /**< No timeout after allowing the PLL to oscillate                                       */
#define CLK_FPDT_TIMEOUT256    ( 0x0010 )                                           /**< 256 counts from allowing the PLL to oscillate                                        */
#define CLK_FPDT_TIMEOUT384    ( 0x0020 )                                           /**< 384 counts from allowing the PLL to oscillate                                        */
#define CLK_FPDT_TIMEOUT512    ( 0x0030 )                                           /**< 512 counts from allowing the PLL to oscillate                                        */

#define CLK_FHUT_COUNT16       ( 0x0000 )                                           /**< 16 counts from allowing the high-speed crystal to oscillate                          */
#define CLK_FHUT_COUNT256      ( 0x0100 )                                           /**< 256 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHUT_COUNT1024     ( 0x0200 )                                           /**< 1024 counts from allowing the high-speed crystal to oscillate                        */
#define CLK_FHUT_COUNT2048     ( 0x0300 )                                           /**< 2048 counts from allowing the high-speed crystal to oscillate                        */
#define CLK_FHUT_COUNT4096     ( 0x0400 )                                           /**< 4096 counts from allowing the high-speed crystal to oscillate                        */
#define CLK_FHUT_COUNT8192     ( 0x0500 )                                           /**< 8192 counts from allowing the high-speed crystal to oscillate                        */
#define CLK_FHUT_COUNT16384    ( 0x0600 )                                           /**< 16384 counts from allowing the high-speed crystal to oscillate                       */
#define CLK_FHUT_COUNT32768    ( 0x0700 )                                           /**< 32768 counts from allowing the high-speed crystal to oscillate                       */
#define CLK_FHDT_NO_TIMEOUT    ( 0x0000 )                                           /**< No timeout after allowing the high-speed crystal to oscillate                        */

#define CLK_FHDT_TIMEOUT128    ( 0x1000 )                                           /**< 128 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT192    ( 0x2000 )                                           /**< 192 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT256    ( 0x3000 )                                           /**< 256 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT384    ( 0x4000 )                                           /**< 384 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT512    ( 0x5000 )                                           /**< 512 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT768    ( 0x6000 )                                           /**< 768 counts from allowing the high-speed crystal to oscillate                         */
#define CLK_FHDT_TIMEOUT1024   ( 0x7000 )                                           /**< 1024 counts from allowing the high-speed crystal to oscillate                        */

/* High speed crystal oscillator circuit settings */
/*     Initial configuration parameters for clk_setHsclkCircuit()  */
#define CLK_HOSCF_40MHZ        ( 0x0000 )                                           /**< Set the frequency of the high-speed crystal to 40 MHz                                */
#define CLK_HOSCF_20MHZ        ( 0x0001 )                                           /**< Set the frequency of the high-speed crystal to 20 MHz                                */

#define CLK_HFLTSEL_DIS        ( 0x0000 )                                           /**< Disable the high-speed crystal filter                                                */
#define CLK_HFLTSEL_ENA_20MHZ  ( 0x0010 )                                           /**< Set the high-speed crystal filter to enabled (for 20 MHz)                            */
#define CLK_HFLTSEL_ENA_40MHZ  ( 0x0030 )                                           /**< Set the high-speed crystal filter to enabled (for 40 MHz)                            */

#define CLK_HSTPT_1COUNT       ( 0x0100 )                                           /**< Set the time of high-speed crystal stop detection detection to 1 count               */
#define CLK_HSTPT_2COUNT       ( 0x0200 )                                           /**< Set the time of high-speed crystal stop detection detection to 2 count               */
#define CLK_HSTPT_4COUNT       ( 0x0300 )                                           /**< Set the time of high-speed crystal stop detection detection to 4 count               */
#define CLK_HSTPT_8COUNT       ( 0x0400 )                                           /**< Set the time of high-speed crystal stop detection detection to 8 count               */
#define CLK_HSTPT_16COUNT      ( 0x0500 )                                           /**< Set the time of high-speed crystal stop detection detection to 16 count              */
#define CLK_HSTPT_32COUNT      ( 0x0600 )                                           /**< Set the time of high-speed crystal stop detection detection to 32 count              */

#define CLK_HSTPSEL_RESET      ( 0x0000 )                                           /**< Set the operation when the high-speed crystal is stopped to reset                    */
#define CLK_HSTPSEL_FLAG       ( 0x2000 )                                           /**< Set the behavior when detecting the stop of the high-speed crystal to only the flag  */
#define CLK_HSTPSEL_INT        ( 0x3000 )                                           /**< Set the operation when the high-speed crystal is stopped to interrupt                */

/* CANCLK settings */
/*     Initial configuration parameters for clk_setCanClk()  */
#define CLK_CANCLK_HSCLK       ( 0x00000002 )                                       /**< Selecting HSCLK as the CANCLK Clock Source                                           */
#define CLK_CANCLK_HXTCLK      ( 0x00000008 )                                       /**< Selecting HXTCLK as the CANCLK Clock Source                                          */
#define CLK_CANCLK_DIV1        ( 0x00010000 )                                       /**< Not divide CANCLK                                                                    */
#define CLK_CANCLK_DIV2        ( 0x00020000 )                                       /**< Set CANCLK to divide by 2                                                            */
#define CLK_CANCLK_DIV4        ( 0x00030000 )                                       /**< Set CANCLK to divide by 4                                                            */
#define CLK_CANCLK_DIV8        ( 0x00040000 )                                       /**< Set CANCLK to divide by 8                                                            */
#define CLK_CANCLK_DIV16       ( 0x00050000 )                                       /**< Set CANCLK to divide by 16                                                           */
#define CLK_CANCLK_DIV32       ( 0x00060000 )                                       /**< Set CANCLK to divide by 32                                                           */

/* NTMSCLK settings */
/*     Initial configuration parameters for clk_setNtmsClk()  */
#define CLK_NTMSCLK_HSCLK      ( 0x00000002 )                                       /**< Selecting HSCLK as the NTMSCLK Clock Source                                          */
#define CLK_NTMSCLK_OSCLK      ( 0x00000004 )                                       /**< Selecting OSCLK as the NTMSCLK Clock Source                                          */
#define CLK_NTMSCLK_DIV1       ( 0x00010000 )                                       /**< Not divide NTMSCLK                                                                   */
#define CLK_NTMSCLK_DIV2       ( 0x00020000 )                                       /**< Set NTMSCLK to divide by 2                                                           */
#define CLK_NTMSCLK_DIV4       ( 0x00030000 )                                       /**< Set NTMSCLK to divide by 4                                                           */
#define CLK_NTMSCLK_DIV8       ( 0x00040000 )                                       /**< Set NTMSCLK to divide by 8                                                           */
#define CLK_NTMSCLK_DIV16      ( 0x00050000 )                                       /**< Set NTMSCLK to divide by 16                                                          */
#define CLK_NTMSCLK_DIV32      ( 0x00060000 )                                       /**< Set NTMSCLK to divide by 32                                                          */

/*############################################################################*/
/*#                                  API                                     #*/
/*############################################################################*/
/**
 * Enable high-speed PLL oscillation (oscillation start)
 *
 * @param   -
 * @return  None
 */
#define         clk_enaPLLHsclk()          set_bit( CLKCNT->FCON01, (1 << 10) )

/**
 * Disable high-speed PLL oscillation (oscillation stop)
 *
 * @param   -
 * @return  None
 */
#define         clk_disPLLHsclk()          clear_bit( CLKCNT->FCON01, (1 << 10) )

/**
 * Getting status of high-speed oscillation
 *
 * @param   -
 * @return  Status of high-speed oscillation
 *          (The value of FSTAT2 register)
 */
#define         clk_checkHsOscStable()  get_bit( CLKCNT->FSTAT2, (1 << 13) )

/**
 * Getting status of low-speed oscillation
 *
 * @param   -
 * @return  Status of low-speed oscillation
 *          (The value of FSTAT register)
 * @note If the low-speed oscillation is stable, "1 (True)" is returned.
 */
#define         clk_checkLsOscStable()  (get_bit( CLKCNT->FSTAT, (1 << 2) ) == 0)

/**
 * Getting status of PLL
 *
 * @param   -
 * @return  Status of PLL
 *          (The value of FSTAT2 register)
 */
#define         clk_checkPllStable()  get_bit( CLKCNT->FSTAT2, (1 << 14) )

/**
 * Getting status of clock generation circuit
 *
 * @param   -
 * @return  Status of clock generation circuit
 *          (The value of FSTAT register)
 */
#define         clk_getClkStatus()      read_reg32( CLKCNT->FSTAT )

/**
 * Getting status of clock generation circuit
 *
 * @param   -
 * @return  Status and IRQ of clock generation circuit
 *          (The value of FSTAT2 register)
 */
#define         clk_getClkStatus2()      read_reg32( CLKCNT->FSTAT2 )

/**
 * Setting HSCLK configration
 * 
 * Configration of high-speed oscillation.                                      <br>
 * (A parameter is setting value of FCON01 register.)
 * 
 * @param[in]   fcon0   Configration of high-speed oscillation.                                                                   <br>
 *                      (Setting value of FCON01 register)                                                                        <br>
 *                      Specify the logical sum of the following items.
 * @arg                     HSCLK frequency           : CLK_SYSC_OSCLK, or CLK_SYSC_OSCLK_DIV2, or (...), or CLK_SYSC_OSCLK_DIV32         <br>
 *                          HCKO frequency            : CLK_OUTC_OSCLK_DIV2, or CLK_OUTC_OSCLK_DIV4, or (...), or CLK_OUTC_OSCLK_DIV128   <br>
 *                          High-speed clock select   : CLK_OSCM_PLL, or CLK_OSCM_HSOSC
 *
 * @param[in]   xspen   Configration of low-speed crystal oscillation stop detection reset function                               <br>
 *                          CLK_XSPEN_DIS           : Disable low-speed crystal oscillation if RC oscillation enable.             <br>
 *                          CLK_XSPEN_ENA           : Enable low-speed crystal oscillation if RC oscillation enable.              <br>
 *
 * @param[in]   hxspen   Configration of high-speed crystal oscillation stop detection reset function                             <br>
 *                          CLK_HXSPEN_DIS           : Disable high-speed crystal oscillation if RC oscillation enable.           <br>
 *                          CLK_HXSPEN_ENA           : Enable high-speed crystal oscillation if RC oscillation enable.            <br>
 *
 * @return      None
 * @note        This macro clears the ENPLL and SYSCLK bits of the FCON1 register to 0.                                           <br>
 *              Please use clk_setSysclk() and clk_enaPLLHsclk().
 */
 
#define         clk_setHsclk( fcon0, xspen, hxspen )  do {                                                                                                                 \
                                                                write_reg32( CLKCNT->FCON01, ((CLKCNT->FCON01 & 0xFFFF3000) | (fcon0)) );                  /* set configration of HSCLK */ \
                                                                if((xspen & 0x1) == 1) {                                                                                  \
                                                                    set_bit( CLKCNT->FCON01, (1 << 12) );                         /* on LSCLK crystal oscillation stop detection */\
                                                                }                                                                                                          \
                                                                else {                                                                                                     \
                                                                    clear_bit( CLKCNT->FCON01, (1 << 12) );                      /* off LSCLK crystal oscillation stop detection */\
                                                                }                                                                                                          \
                                                                if((hxspen & 0x1) == 1) {                                                                                  \
                                                                    set_bit( CLKCNT->FCON01, (1 << 13) );                         /* on HSCLK crystal oscillation stop detection */\
                                                                }                                                                                                          \
                                                                else {                                                                                                     \
                                                                    clear_bit( CLKCNT->FCON01, (1 << 13) );                      /* off HSCLK crystal oscillation stop detection */\
                                                                }                                                                                                          \
                                                         } while(0)
/**
 * Setting LSCLK configration
 *
 * Configration of low-speed oscillation.                                       <br>
 * (A parameter is setting value of FCON23 register.)
 *
 * @param[in]   fcon2   Configration of low-speed oscillation.                                                                   <br>
 *                      (Setting value of FCON23 register)                                                                       <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_XTM_CRYSTAL   : Crystal oscillation                                                                <br>
 * @arg                     CLK_XTM_RC        : RC oscillation                                                                     <br>
 * @arg                     CLK_LFLTSEL_DIS   : Disables the low-speed clock noise filter                                          <br>
 * @arg                     CLK_LFLTSEL_ENA   : Enables the low-speed clock noise filter                                           <br>
 * @arg                     CLK_LSTPSEL_RESET : Reset when the low-speed crystal oscillation stop is detected                                           <br>
 * @arg                     CLK_LSTPSEL_FLAG  : Set Flag when the low-speed crystal oscillation stop is detected                                           <br>
 * @arg                     CLK_LSTPSEL_INT   : Issue the interrupt when the low-speed crystal oscillation stop is detected                                           <br>
 * 
 * @return      None
 */
#define         clk_setLsclk( fcon2 )   write_reg32( CLKCNT->FCON23, fcon2 )

/**
 * Setting system clock
 * 
 * @note    --- How to switching HSCLK ---
 * 
 *          (1)Configration of HSCLK (Setting FCON0/FCON1 registers)
 *             Disables HSCLK (Setting ENPLL=0)
 *              clk_setHsclk(...);
 *          (2)Enables HSCLK (Setting ENOSC=1, or ENPLL=1)
 *              clk_enaPLLHsclk();
 *          (3)Waiting stables high-speed oscillation
 *              ex.
 *                  while( clk_checkHsOscStable() == 0 ) {}
 *          (4)Setting SYSCLK=1
 *              clk_setSysclk( CLK_SYSCLK_HSCLK );
 * 
 * @see     MCU Users manual 'Description of Operation' in chapter 'Clock Generation Circuit'.
 * 
 * @param[in]   sysclk  CLK_SYSCLK_LSCLK : LSCLK(low-speed oscillation)         <br>
 *                      CLK_SYSCLK_HSCLK : HSCLK(high-speed oscillation)
 * @return      None
 */
#define         clk_setSysclk( sysclk )   do {                                                                                                               \
                                                  if((sysclk & 0x1) == 1) {                                                                                  \
                                                      set_bit( CLKCNT->FCON01, (1 << 8) );                           /* set SYSCLK */                        \
                                                  }                                                                                                          \
                                                  else {                                                                                                     \
                                                      clear_bit( CLKCNT->FCON01, (1 << 8) );                         /* clear SYSCLK */                      \
                                                  }                                                                                                          \
                                             } while(0)

/**
 * Getting LSCLK configration
 *
 * @param   -
 * @return  configration of low-speed oscillation
 *          (The value of FCON23 register)
 */
#define         clk_getLsclk()          read_reg32( CLKCNT->FCON23 )

/**
 * Configration of crystal circuit
 *
 * @param[in]   fcon6   Configration of crystal circuit.                                                                         <br>
 *                      (Setting value of FCON67 register)                                                                       <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_LOSCMD_XT32K  : Crystal circuit source : XT32K                                                   <br>
 * @arg                     CLK_LOSCMD_EXT32K : Crystal circuit source : EXT32K                                                  <br>
 * @arg                     CLK_LMOD_STANDARD : Crystal circuit mode : STANDARD                                                  <br>
 * @arg                     CLK_LMOD_LOWPOWER : Crystal circuit mode : LOWPOWER                                                  <br>
 * @arg                     CLK_LMOD_TOUGH    : Crystal circuit mode : TOUGH                                                     <br>
 * 
 * @return      None
 */
#define         clk_setLsclkCircuit( fcon6 )    write_reg32( CLKCNT->FCON67, fcon6 )

/**
 * Enable high-speed Crystal oscillation (oscillation start)
 *
 * @param   -
 * @return  None
 */
#define         clk_enaHsOsc()    set_bit( CLKCNT->FCON01, (1 << 9) )

/**
 * Disable high-speed Crystal oscillation (oscillation stop)
 *
 * @param   -
 * @return  None
 */
#define         clk_disHsOsc()    clear_bit( CLKCNT->FCON01, (1 << 9) )

/**
 * Configration of HSCLK wakup control
 *
 * @param[in]   fhwupt  Configration of  HSCLK wakup control.                                                                    <br>
 *                      (Setting value of FHWUPT register)                                                                       <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_FPUT_WAIT_LOCK     : Wait for the lock before supplying                                          <br>
 * @arg                     CLK_FPUT_NOT_WAIT_LOCK : Supply without waiting for locks                                            <br>
 * 
 * @arg                     CLK_FPDT_NO_TIMEOUT : No timeout after allowing the PLL to oscillate                                 <br>
 * @arg                     CLK_FPDT_TIMEOUT256 : 256 counts from allowing the PLL to oscillate                                  <br>
 * @arg                     CLK_FPDT_TIMEOUT384 : 384 counts from allowing the PLL to oscillate                                  <br>
 * @arg                     CLK_FPDT_TIMEOUT512 : 512 counts from allowing the PLL to oscillate                                  <br>
 * 
 * @arg                     CLK_FHUT_COUNT16    : 16 counts from allowing the high-speed crystal to oscillate                    <br>
 * @arg                     CLK_FHUT_COUNT256   : 256 counts from allowing the high-speed crystal to oscillate                   <br>
 * @arg                     CLK_FHUT_COUNT1024  : 1024 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHUT_COUNT2048  : 2048 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHUT_COUNT4096  : 4096 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHUT_COUNT8192  : 8192 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHUT_COUNT16384 : 16384 counts from allowing the high-speed crystal to oscillate                 <br>
 * @arg                     CLK_FHUT_COUNT32768 : 32768 counts from allowing the high-speed crystal to oscillate                 <br>
 * 
 * @arg                     CLK_FHDT_NO_TIMEOUT  : No timeout after allowing the high-speed crystal to oscillate                 <br>
 * @arg                     CLK_FHDT_TIMEOUT128  : 128 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT192  : 192 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT256  : 256 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT384  : 384 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT512  : 512 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT768  : 768 counts from allowing the high-speed crystal to oscillate                  <br>
 * @arg                     CLK_FHDT_TIMEOUT1024 : 1024 counts from allowing the high-speed crystal to oscillate                 <br>
 * 
 * @return      None
 */
#define         clk_setHsclkControl( fhwupt )   write_reg32( CLKCNT->FHWUPT, fhwupt )

/**
 * Configration of high-speed crystal oscillation circuit
 *
 * @param[in]   fcon8  Configration of  high-speed crystal oscillation circuit                                                      <br>
 *                      (Setting value of FCON89 register)                                                                          <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_HOSCF_40MHZ       : Set the frequency of the high-speed crystal to 40 MHz                           <br>
 * @arg                     CLK_HOSCF_20MHZ       : Set the frequency of the high-speed crystal to 20 MHz                           <br>
 * 
 * @arg                     CLK_HFLTSEL_DIS        : Disable the high-speed crystal filter                                          <br>
 * @arg                     CLK_HFLTSEL_ENA__20MHZ : Set the high-speed crystal filter to enabled (for 20 MHz)                      <br>
 * @arg                     CLK_HFLTSEL_ENA__40MHZ : Set the high-speed crystal filter to enabled (for 40 MHz)                      <br>
 * 
 * @arg                     CLK_HSTPT_1COUNT  : Set the time of high-speed crystal stop detection detection to 1 count              <br>
 * @arg                     CLK_HSTPT_2COUNT  : Set the time of high-speed crystal stop detection detection to 2 count              <br>
 * @arg                     CLK_HSTPT_4COUNT  : Set the time of high-speed crystal stop detection detection to 4 count              <br>
 * @arg                     CLK_HSTPT_8COUNT  : Set the time of high-speed crystal stop detection detection to 8 count              <br>
 * @arg                     CLK_HSTPT_16COUNT : Set the time of high-speed crystal stop detection detection to 16 count             <br>
 * @arg                     CLK_HSTPT_32COUNT : Set the time of high-speed crystal stop detection detection to 32 count             <br>
 * 
 * @arg                     CLK_HSTPSEL_RESET : Set the operation when the high-speed crystal is stopped to reset                   <br>
 * @arg                     CLK_HSTPSEL_FLAG  : Set the behavior when detecting the stop of the high-speed crystal to only the flag <br>
 * @arg                     CLK_HSTPSEL_INT   : Set the operation when the high-speed crystal is stopped to interrupt               <br>
 * 
 * @return      None
 */
#define         clk_setHsclkCircuit( fcon8 )   write_reg32( CLKCNT->FCON89, fcon8 )

/**
 * Configration of CANCLK
 *
 * @param[in]   syscanmod  Configration of  CANCLK                                                                                  <br>
 *                      (Setting value of SYSCANMOD register)                                                                       <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_CANCLK_HSCLK  : Selecting HSCLK as the CANCLK Clock Source                                          <br>
 * @arg                     CLK_CANCLK_HXTCLK : Selecting HXTCLK as the CANCLK Clock Source                                         <br>
 * 
 * @arg                     CLK_CANCLK        : Not divide CANCLK                                                                   <br>
 * @arg                     CLK_CANCLK_DIV2   : Set CANCLK to divide by 2                                                           <br>
 * @arg                     CLK_CANCLK_DIV4   : Set CANCLK to divide by 4                                                           <br>
 * @arg                     CLK_CANCLK_DIV8   : Set CANCLK to divide by 8                                                           <br>
 * @arg                     CLK_CANCLK_DIV16  : Set CANCLK to divide by 16                                                          <br>
 * @arg                     CLK_CANCLK_DIV32  : Set CANCLK to divide by 32                                                          <br>
 * 
 * @return      None
 */
#define         clk_setCanClk( syscanmod )   write_reg32( CLKCNT->SYSCANMOD, syscanmod )

/**
 * Configration of NTMSCLK
 *
 * @param[in]   syscanmod  Configration of  NTMSCLK                                                                                  <br>
 *                      (Setting value of SYSNTMSMOD register)                                                                       <br>
 *                      Specify the logical sum of the following items.
 * @arg                     CLK_NTMSCLK_HSCLK : Selecting HSCLK as the NTMSCLK Clock Source                                          <br>
 * @arg                     CLK_NTMSCLK_OSCLK : Selecting OSCLK as the NTMSCLK Clock Source                                          <br>
 * @arg                     CLK_NTMSCLK       : Not divide NTMSCLK                                                                   <br>
 * @arg                     CLK_NTMS_DIV2     : Set NTMSCLK to divide by 2                                                           <br>
 * @arg                     CLK_NTMS_DIV4     : Set NTMSCLK to divide by 4                                                           <br>
 * @arg                     CLK_NTMS_DIV8     : Set NTMSCLK to divide by 8                                                           <br>
 * @arg                     CLK_NTMS_DIV16    : Set NTMSCLK to divide by 16                                                          <br>
 * @arg                     CLK_NTMS_DIV32    : Set NTMSCLK to divide by 32                                                          <br>
 * 
 * @return      None
 */
#define         clk_setNtmsClk( sysntmsmod )   write_reg32( CLKCNT->SYSNTMSMOD, sysntmsmod )


#endif /* CLOCK_H__ */

/** @} */
/** @} */
