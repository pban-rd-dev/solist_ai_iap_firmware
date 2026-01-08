
#include "device.h"

#include "ML63Q25x7.h"
#include "smpl_common.h"
#include "irq.h"
#include "wdt.h"

#include "debug_uart.h"

extern uint32_t SystemCoreClock;


uint16_t device_initialize(void)
{
  /* System initialization is done in SystemInit() called from startup code */

  /* Initialize watchdog timer (2 second timeout) */
  wdt_init(WDT_2S);
  wdt_clear();

  clk_setHsclk(CLK_SYSC_OSCLK, CLK_XSPEN_DIS, CLK_HXSPEN_DIS);
  /* Enable PLL */
  clk_enaPLLHsclk();

  /* Wait stables PLL oscillation */
  while( clk_checkPllStable() == 0) {
    wdt_clear();
  }

  /* Set SYSCLK */
  clk_setSysclk( CLK_SYSCLK_HSCLK );

  smpl_enablePeripheral(USR_PERI);

  debug_uart_init();
  return 0;
}

uint16_t device_deinitialize(void)
{
  debug_uart_deinit();
  return 0;
}

uint32_t device_get_sysclk(void)
{
  uint32_t div = 0;
  uint32_t clk = 0;
  uint32_t fcon01 = read_reg32(CLKCNT->FCON01);
  if ( (fcon01 & (0x01 << 8)) != 0x00 )  { // SYSCLK
    // Use HSCLK as SYSCLK
    uint32_t outc = (fcon01 & 0x000000e0) >> 5;

    switch (outc) {
    case 0x01:
      div = 2;
      break;
    case 0x02:
      div = 4;
      break;
    case 0x03:
      div = 8;
      break;
    case 0x04:
      div = 16;
      break;
    case 0x05:
      div = 32;
      break;
    case 0x06:
      div = 64;
      break;
    case 0x07:
      div = 128;
      break;
    default:
      // case 0x00:
      div = 1;
      break;
    }
    uint32_t oscm = (fcon01 & 0x00000018) >> 3;
    switch (oscm) {
    case 0x00:
      // PLL
      clk = 48000000 / div;
      break;
    case 0x01:
      // HIGH-Speed Crystal OSCILLATION
      clk = 20000000 / div;
      break;
    default:
      break;
    }

  } else {
    // Use LSCLK as SYSCLK
    clk = SystemCoreClock;
  }

  return clk;
}

uint32_t device_get_tick_ms(void) {
  uint32_t sysclk = device_get_sysclk();
  return sysclk / 1000;
}

#pragma GCC push_options
#pragma GCC optimize ("-O0")
uint16_t device_delay_ms(uint16_t ms)
{
  uint32_t tick_ms = device_get_tick_ms() * ms;
  do {
    __NOP();
  } while (tick_ms--);

  return 0;
}
#pragma GCC pop_options

/**
 * TODO: CPU counts on interruption
 */
uint16_t device_delay_us(uint16_t us)
{
  uint32_t tick_us = device_get_tick_ms() * us / 1000;
  do {
    __NOP();
  } while (tick_us--);
  return 0;
}

void device_irq_ssiof(bool enable)
{
  if (enable) {
    /* enable interrupt */
    irq_siof0_clearIRQ();
    irq_siof0_ena();

    __enable_irq();
  } else {
    /* disable interrupt */
    irq_siof0_dis();
  }
}
