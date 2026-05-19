/*****************************************************************************
 * @file     test_framework.c
 * @brief    Test framework implementation
 * @version  1.0
 *****************************************************************************/

#include "test_framework.h"
#include <string.h>

/* Global test registry */
test_registry_t g_test_registry = {
    .count = 0
};

/**
 * @brief Register a test
 */
void test_register(const char *suite_name, const char *test_name, test_func_t test_func,
                   test_setup_func_t setup, test_setup_func_t teardown)
{
    if (g_test_registry.count >= TEST_MAX_TESTS) {
        UART_PRINT_WARN("Test registry full! Cannot register: %s::%s", suite_name, test_name);
        return;
    }

    test_descriptor_t *test = &g_test_registry.tests[g_test_registry.count];
    test->suite_name = suite_name;
    test->test_name = test_name;
    test->test_func = test_func;
    test->setup = setup;
    test->teardown = teardown;

    g_test_registry.count++;
}

/**
 * @brief Run all registered tests
 */
void test_run_all(test_results_t *results)
{
    const char *current_suite = NULL;

    UART_PRINT_DEBUG("");
    UART_PRINT_DEBUG("Running %lu registered tests...", g_test_registry.count);
    UART_PRINT_DEBUG("");

    for (uint32_t i = 0; i < g_test_registry.count; i++) {
        test_descriptor_t *test = &g_test_registry.tests[i];

        /* Print suite header when suite changes */
        if (current_suite == NULL || strcmp(current_suite, test->suite_name) != 0) {
            current_suite = test->suite_name;
            UART_PRINT_DEBUG("=== %s Tests ===", current_suite);
        }

        /* Run setup if provided */
        if (test->setup != NULL) {
            test->setup();
        }

        /* Run the test */
        UART_PRINT_DEBUG("Running: %s", test->test_name);
        test->test_func(results);

        /* Run teardown if provided */
        if (test->teardown != NULL) {
            test->teardown();
        }
    }
}
