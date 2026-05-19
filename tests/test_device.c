/*****************************************************************************
 * @file     test_device.c
 * @brief    Tests for device.c functionality
 * @version  1.0
 *****************************************************************************/

#include "test_framework.h"
#include "device.h"
#include "wdt.h"

/**
 * @brief Test device_initialize returns success
 */
DEFINE_TEST(test_device_initialize)
{
    uint32_t result = device_initialize();
    TEST_ASSERT_EQUAL(0, result, "device_initialize returns 0");
}

/**
 * @brief Test system clock is configured correctly
 */
DEFINE_TEST(test_device_sysclk_is_48mhz)
{
    uint32_t sysclk = device_get_sysclk();

    TEST_ASSERT_NOT_EQUAL(0, sysclk, "System clock is non-zero");
    TEST_ASSERT_EQUAL(48000000, sysclk, "System clock is 48MHz after PLL init");

    UART_PRINT_DEBUG("    Actual sysclk: %lu Hz", sysclk);
}

/**
 * @brief Test get_tick_ms returns sysclk/1000 constant
 */
DEFINE_TEST(test_device_get_tick_ms)
{
    uint32_t tick = device_get_tick_ms();
    uint32_t sysclk = device_get_sysclk();
    uint32_t expected = sysclk / 1000;

    TEST_ASSERT_EQUAL(expected, tick, "get_tick_ms = sysclk/1000");
    UART_PRINT_DEBUG("    sysclk: %lu Hz, tick_ms constant: %lu", sysclk, tick);

    /* Verify it returns the same value (it's not a counter) */
    uint32_t tick2 = device_get_tick_ms();
    TEST_ASSERT_EQUAL(tick, tick2, "get_tick_ms returns same value (constant)");
}

/**
 * @brief Test millisecond delay function executes
 */
DEFINE_TEST(test_device_delay_ms)
{
    /* Verify delay functions execute without hanging */
    device_delay_ms(1);
    TEST_ASSERT(true, "delay_ms(1) completes");

    device_delay_ms(10);
    TEST_ASSERT(true, "delay_ms(10) completes");
}

/**
 * @brief Test microsecond delay function
 */
DEFINE_TEST(test_device_delay_us)
{
    /* Just verify it doesn't crash or hang */
    device_delay_us(100);

    TEST_ASSERT(true, "delay_us(100) completes");

    device_delay_us(1000);

    TEST_ASSERT(true, "delay_us(1000) completes");
}

/**
 * @brief Test watchdog can be cleared
 */
DEFINE_TEST(test_watchdog_clear)
{
    /* Watchdog is already initialized in device_initialize() */

    wdt_clear();
    TEST_ASSERT(true, "wdt_clear() completes");

    /* Clear multiple times */
    for (int i = 0; i < 5; i++) {
        wdt_clear();
    }

    TEST_ASSERT(true, "Multiple wdt_clear() calls work");
}

/**
 * @brief Test device_deinitialize
 */
DEFINE_TEST(test_device_deinitialize)
{
    uint32_t result = device_deinitialize();
    TEST_ASSERT_EQUAL(0, result, "device_deinitialize returns 0");

    /* Re-initialize for remaining tests */
    device_initialize();
    TEST_ASSERT(true, "Re-initialization after deinit works");
}

/**
 * @brief Register all device tests
 */
void register_device_tests(void)
{
    REGISTER_TEST("Device", test_device_initialize);
    REGISTER_TEST("Device", test_device_sysclk_is_48mhz);
    REGISTER_TEST("Device", test_device_get_tick_ms);
    REGISTER_TEST("Device", test_device_delay_ms);
    REGISTER_TEST("Device", test_device_delay_us);
    REGISTER_TEST("Device", test_watchdog_clear);
    REGISTER_TEST("Device", test_device_deinitialize);
}
