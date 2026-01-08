/*****************************************************************************
 smpl_common.c

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
 * @file    smpl_common.c
 *
 * This module is commmon process for sample programs.
 *
 */
#include "mcu.h"
#include "clock.h"
#include "wdt.h"
#include "smpl_common.h"

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
/**
 * Set LSCLK setting (Crystal oscillation 32KHz)
 * 
 * @param           -
 * @return          None
 */
void smpl_setLsCrystal32Khz( void )
{
    /* Set SYSCLK to LSCLK */
    clk_setSysclk( CLK_SYSCLK_LSCLK );

    if( (clk_getLsclk() & 0x03U ) != 0x01U ) {
        /* Set low-speed clock generation circuit */
        clk_setLsclkCircuit( CLK_LOSCMD_XT32K | CLK_LMOD_STANDARD );

        /* Set reset function of crystal oscillation stop detection */
        clk_setHsclk( CLK_SYSC_OSCLK_DIV8 | CLK_OUTC_OSCLK_DIV8, CLK_XSPEN_ENA, CLK_HXSPEN_DIS );

        /* Set LSCLK oscillation mode */
        clk_setLsclk( CLK_LFLTSEL_DIS | CLK_XTM_CRYSTAL );
    } else {
        /* no process */
    }

    /* Wait stables crystal oscillation */
    while( clk_checkLsOscStable() == 0 ) {
        wdt_clear();
    }
}

/**
 * Set HSCLK setting (Crystal oscillation 20MHz)
 * 
 * @param           -
 * @return          None
 */
void smpl_setHsCrystal20Mhz( void )
{
    /* Set high-speed crystal oscillation circuit */
    clk_setHsclkCircuit( CLK_HOSCF_20MHZ | CLK_HFLTSEL_ENA_20MHZ | CLK_HSTPT_2COUNT );

    /* Set high-speed clock control */
    clk_setHsclkControl( CLK_FHUT_COUNT16 | CLK_FHDT_NO_TIMEOUT );

    /* Set to high-speed crystal oscillation */
    clk_setHsclk( CLK_SYSC_OSCLK | CLK_OSCM_HSOSC | CLK_OUTC_OSCLK_DIV8, CLK_XSPEN_DIS, CLK_HXSPEN_ENA );

    /* Enable high-speed crystal oscillation */
    clk_enaHsOsc();

    /* Wait stables crystal oscillation */
    while( clk_checkHsOscStable() == 0 ) {
        wdt_clear();
    }

    /* Set SYSCLK */
    clk_setSysclk( CLK_SYSCLK_HSCLK );
}

/**
 * Set HSCLK setting (Crystal oscillation 40MHz)
 * 
 * @param           -
 * @return          None
 */
void smpl_setHsCrystal40Mhz( void )
{
    /* Set high-speed crystal oscillation circuit */
    clk_setHsclkCircuit( CLK_HOSCF_40MHZ | CLK_HFLTSEL_ENA_40MHZ | CLK_HSTPT_2COUNT );

    /* Set high-speed clock control */
    clk_setHsclkControl( CLK_FHUT_COUNT16 | CLK_FHDT_NO_TIMEOUT );

    /* Set to high-speed crystal oscillation */
    clk_setHsclk( CLK_SYSC_OSCLK | CLK_OSCM_HSOSC | CLK_OUTC_OSCLK_DIV8, CLK_XSPEN_DIS, CLK_HXSPEN_ENA );

    /* Enable high-speed crystal oscillation */
    clk_enaHsOsc();

    /* Wait stables crystal oscillation */
    while( clk_checkHsOscStable() == 0 ) {
        wdt_clear();
    }

    /* Set SYSCLK */
    clk_setSysclk( CLK_SYSCLK_HSCLK );
}

/**
 * Set HSCLK setting (PLL oscillation 48MHz)
 * 
 * @param[in]       xspen               Configration of reset function of crystal oscillation stop detection.  <br>
 *                                      See also, clk_setHsclk().
 * @return          None
 * @note            Division of OSCLK frequency is 1/1 OSCLK.
 */
void smpl_setHsPll48Mhz( uint32_t xspen, uint32_t hxspen )
{
    /* Set HSCLK mode */
    clk_setHsclk( CLK_SYSC_OSCLK | CLK_OUTC_OSCLK_DIV8, xspen, hxspen );

    /* Enable PLL */
    clk_enaPLLHsclk();

    /* Wait stables PLL oscillation */
    while( clk_checkPllStable() == 0) {
        wdt_clear();
    }

    /* Set SYSCLK */
    clk_setSysclk( CLK_SYSCLK_HSCLK );
}

/**
 * Enable each peripheral block
 * 
 * @param[in]       on use peripheral  reset off the peripherals.                 <br>
 *                      Specify the logical sum of the following items.
 * @arg             use peripheral  : TM0_PERI, or TM1_PERI, or (...), or CRC_PERI
 * @return          None
 */
void smpl_enablePeripheral( uint32_t on )
{
    set_bit(LSICNT->CLKCON, on);
    clear_bit(LSICNT->RSTCON, on);
}

/**
 * Disable each peripheral block
 * 
 * @param[in]       off use peripheral  reset on the peripherals.                 <br>
 *                      Specify the logical sum of the following items.
 * @arg             use peripheral  : TM0_PERI, or TM1_PERI, or (...), or CRC_PERI
 * @return          None
 */
void smpl_disablePeripheral( uint32_t off )
{
    set_bit(LSICNT->RSTCON, off);
    clear_bit(LSICNT->CLKCON, off);
}

/**
 * Enable MCU status interrupt
 * 
 * @param[in]       on use interrupt                                              <br>
 *                      Specify the logical sum of the following items.
 * @arg                 interrupt  : MCS_INT_FLCINT, or MCS_INT_RAM_PAR, or (...), or MCS_INT_CAN_BUS_ERR
 * @return          None
 */
void smpl_enableMcsInterrupt( uint32_t on )
{
    set_bit(LSICNT->MCUINTEN, on);
}

/**
 * Disable MCU status interrupt
 * 
 * @param[in]       off use interrupt                                             <br>
 *                      Specify the logical sum of the following items.
 * @arg                 interrupt  : MCS_INT_FLCINT, or MCS_INT_RAM_PAR, or (...), or MCS_INT_CAN_BUS_ERR
 * @return          None
 */
void smpl_disableMcsInterrupt( uint32_t off )
{
    clear_bit(LSICNT->MCUINTEN, off);
}

/**
 * Get the status of MCU status interrupt
 * 
 * @param           -
 * @return          the value of MCUISTAT register
 */
uint32_t smpl_getMcsInterruptStatus( void )
{
    return read_reg32(LSICNT->MCUISTAT);
}

/**
 * Clear MCU status interrupt
 * 
 * @param[in]       clear use interrupt                                           <br>
 *                      Specify the logical sum of the following items.
 * @arg                 interrupt  : MCS_INT_FLCINT, or MCS_INT_RAM_PAR, or (...), or MCS_INT_CAN_BUS_ERR
 * @return          None
 */
void smpl_clearMcsInterruptStatus( uint32_t clear )
{
    set_bit(LSICNT->MCUISTAT, clear);
}
