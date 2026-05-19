/*****************************************************************************
 * @file     test_main.c
 * @brief    Test binary main entry point
 * @version  1.0
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "ML63Q25x7.h"

#include "wdt.h"
#include "uart_print.h"
#include "device.h"

#include "test_framework.h"

/* External test registration functions */
extern void register_device_tests(void);

/**
 * @brief Main test program
 * @retval int
 */
int main(void)
{
    test_results_t results = {
        .passed = 0,
        .failed = 0,
        .total = 0
    };

    /* Initialize device */
    if (device_initialize() != 0) {
        /* If initialization fails, loop with watchdog clearing */
        while (1) {
            wdt_clear();
        }
        return -1;
    }

    /* Print test suite header */
    UART_PRINT_DEBUG("\n\n");
    UART_PRINT_DEBUG("=====================================");
    UART_PRINT_DEBUG("  ML63Q2537 Hardware Test Suite");
    UART_PRINT_DEBUG("=====================================");
    UART_PRINT_DEBUG("");

    /* Register all test suites */
    register_device_tests();
    /* Add more test suite registrations here as they are created:
     * register_uart_tests();
     * etc.
     */

    UART_PRINT_DEBUG("Registered %lu tests", g_test_registry.count);
    UART_PRINT_DEBUG("");

    /* Run all registered tests */
    test_run_all(&results);

    /* Print summary */
    UART_PRINT_DEBUG("");
    UART_PRINT_DEBUG("=====================================");
    UART_PRINT_DEBUG("  Test Summary");
    UART_PRINT_DEBUG("=====================================");
    UART_PRINT_DEBUG("Total:  %lu", results.total);
    UART_PRINT_DEBUG("Passed: %lu", results.passed);
    UART_PRINT_DEBUG("Failed: %lu", results.failed);
    UART_PRINT_DEBUG("");

    if (results.failed == 0) {
        UART_PRINT_DEBUG("*** ALL TESTS PASSED ***");
    } else {
        UART_PRINT_WARN("*** SOME TESTS FAILED ***");
    }

    /* Main loop - continue running with status indication */

    wdt_clear();

    if (results.failed == 0) {
      /* All passed - slow blink pattern */
      UART_PRINT_DEBUG("All tests passed - running...");
    } else {
      /* Some failed - fast blink pattern */
      UART_PRINT_WARN("Tests failed: %lu", results.failed);
    }

    while (1) {
      device_delay_ms(1000);
      wdt_clear();
    }

    return 0;
}
