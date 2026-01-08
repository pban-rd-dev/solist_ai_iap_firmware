/*****************************************************************************
 * @file     test_main.c
 * @brief    Test binary main entry point
 * @version  1.0
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "ML63Q25x7.h"

#include "wdt.h"
#include "debug_uart.h"
#include "device.h"

#include "test_framework.h"

/* External test registration functions */
extern void register_device_tests(void);
extern void register_spi_hal_tests(void);

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
    DEBUG_DEBUG("\n\n");
    DEBUG_DEBUG("=====================================");
    DEBUG_DEBUG("  ML63Q2537 Hardware Test Suite");
    DEBUG_DEBUG("=====================================");
    DEBUG_DEBUG("");

    /* Register all test suites */
    register_device_tests();
    register_spi_hal_tests();
    /* Add more test suite registrations here as they are created:
     * register_uart_tests();
     * etc.
     */

    DEBUG_DEBUG("Registered %lu tests", g_test_registry.count);
    DEBUG_DEBUG("");

    /* Run all registered tests */
    test_run_all(&results);

    /* Print summary */
    DEBUG_DEBUG("");
    DEBUG_DEBUG("=====================================");
    DEBUG_DEBUG("  Test Summary");
    DEBUG_DEBUG("=====================================");
    DEBUG_DEBUG("Total:  %lu", results.total);
    DEBUG_DEBUG("Passed: %lu", results.passed);
    DEBUG_DEBUG("Failed: %lu", results.failed);
    DEBUG_DEBUG("");

    if (results.failed == 0) {
        DEBUG_DEBUG("*** ALL TESTS PASSED ***");
    } else {
        DEBUG_WARN("*** SOME TESTS FAILED ***");
    }

    /* Main loop - continue running with status indication */

    wdt_clear();

    if (results.failed == 0) {
      /* All passed - slow blink pattern */
      DEBUG_DEBUG("All tests passed - running...");
    } else {
      /* Some failed - fast blink pattern */
      DEBUG_WARN("Tests failed: %lu", results.failed);
    }

    while (1) {
      device_delay_ms(1000);
      wdt_clear();
    }

    return 0;
}
