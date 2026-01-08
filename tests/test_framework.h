/*****************************************************************************
 * @file     test_framework.h
 * @brief    Test framework with automatic test registration
 * @version  1.0
 *****************************************************************************/

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "debug_uart.h"

/* Maximum number of tests that can be registered */
#define TEST_MAX_TESTS 64

/* Test result counters */
typedef struct {
    uint32_t passed;
    uint32_t failed;
    uint32_t total;
} test_results_t;

/* Test function pointer type */
typedef void (*test_func_t)(test_results_t *results);

/* Setup/teardown function pointer type */
typedef void (*test_setup_func_t)(void);

/* Test descriptor */
typedef struct {
    const char *suite_name;    /* Test suite name (e.g., "Device") */
    const char *test_name;     /* Individual test name */
    test_func_t test_func;     /* Test function */
    test_setup_func_t setup;   /* Optional setup function (called before test) */
    test_setup_func_t teardown;/* Optional teardown function (called after test) */
} test_descriptor_t;

/* Test registry */
typedef struct {
    test_descriptor_t tests[TEST_MAX_TESTS];
    uint32_t count;
} test_registry_t;

/* Global test registry */
extern test_registry_t g_test_registry;

/* Test registration function */
void test_register(const char *suite_name, const char *test_name, test_func_t test_func,
                   test_setup_func_t setup, test_setup_func_t teardown);

/* Test runner */
void test_run_all(test_results_t *results);

/* Test assertion macros */
#define TEST_ASSERT(condition, description) \
    do { \
        if (condition) { \
            results->passed++; \
            DEBUG_DEBUG("  [PASS] %s", description); \
        } else { \
            results->failed++; \
            DEBUG_WARN("  [FAIL] %s", description); \
        } \
        results->total++; \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, description) \
    do { \
        if ((expected) == (actual)) { \
            results->passed++; \
            DEBUG_DEBUG("  [PASS] %s", description); \
        } else { \
            results->failed++; \
            DEBUG_WARN("  [FAIL] %s (expected: %lu, actual: %lu)", \
                       description, (uint32_t)(expected), (uint32_t)(actual)); \
        } \
        results->total++; \
    } while(0)

#define TEST_ASSERT_NOT_EQUAL(expected, actual, description) \
    do { \
        if ((expected) != (actual)) { \
            results->passed++; \
            DEBUG_DEBUG("  [PASS] %s", description); \
        } else { \
            results->failed++; \
            DEBUG_WARN("  [FAIL] %s (both are: %lu)", \
                       description, (uint32_t)(actual)); \
        } \
        results->total++; \
    } while(0)

#define TEST_ASSERT_GREATER(value, threshold, description) \
    do { \
        if ((value) > (threshold)) { \
            results->passed++; \
            DEBUG_DEBUG("  [PASS] %s", description); \
        } else { \
            results->failed++; \
            DEBUG_WARN("  [FAIL] %s (value: %lu, threshold: %lu)", \
                       description, (uint32_t)(value), (uint32_t)(threshold)); \
        } \
        results->total++; \
    } while(0)

#define TEST_ASSERT_IN_RANGE(value, min, max, description) \
    do { \
        if ((value) >= (min) && (value) <= (max)) { \
            results->passed++; \
            DEBUG_DEBUG("  [PASS] %s", description); \
        } else { \
            results->failed++; \
            DEBUG_WARN("  [FAIL] %s (value: %lu, range: %lu-%lu)", \
                       description, (uint32_t)(value), (uint32_t)(min), (uint32_t)(max)); \
        } \
        results->total++; \
    } while(0)

/* Helper macros for test definition */
#define DEFINE_TEST(test_name) \
    static void test_name(test_results_t *results)

/* Register test without setup/teardown */
#define REGISTER_TEST(suite, test) \
    test_register(suite, #test, test, NULL, NULL)

/* Register test with setup function */
#define REGISTER_TEST_WITH_SETUP(suite, test, setup_func) \
    test_register(suite, #test, test, setup_func, NULL)

/* Register test with setup and teardown */
#define REGISTER_TEST_WITH_SETUP_TEARDOWN(suite, test, setup_func, teardown_func) \
    test_register(suite, #test, test, setup_func, teardown_func)

#endif /* TEST_FRAMEWORK_H */
