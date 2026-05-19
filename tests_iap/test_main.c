/*****************************************************************************
 * @file    test_main.c
 * @brief   tests_iap entry point — smoke tests for the IAP sample
 *
 * On-target test binary. Boots like a normal app (uses the master linker
 * layout, not the IAP split layout), runs the registered tests, prints
 * results over UART (P32/P33 @ 115200), then idles with the WDT clearing.
 *****************************************************************************/

#include <stdint.h>

#include "ML63Q25x7.h"
#include "wdt.h"

#include "device.h"
#include "test_io.h"

extern uint32_t run_xmodem_crc_tests(void);   /* returns # of failures */

int main(void)
{
    if (device_initialize() != 0) {
        while (1) {}
    }
    test_io_init();

    test_io_puts("\r\n=== tests_iap ===\r\n");

    uint32_t failed = 0;
    failed += run_xmodem_crc_tests();

    test_io_puts("\r\nresult: ");
    if (failed == 0) {
        test_io_puts("ALL PASSED\r\n");
    } else {
        test_io_puts("FAILED (");
        test_io_print_u32(failed);
        test_io_puts(")\r\n");
    }

    while (1) {
        wdt_clear();
    }
}

/* Empty IRQ handlers — tests_iap does not use the interrupt-driven UART. */
void EXI_IRQHandler(void)  { }
void NMI_Handler(void)     { }
void UAF0_IRQHandler(void) { }
void LTBC_IRQHandler(void) { }
void SIOF0_IRQHandler(void){ }
