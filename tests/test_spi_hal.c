/*****************************************************************************
 * @file     test_spi_hal.c
 * @brief    Tests for SPI HAL implementation
 * @version  1.0
 *****************************************************************************/

#include "test_framework.h"
#include "spi_hal.h"
#include "spi_config.h"

/* Track initialization state to avoid redundant init */
static bool spi_hal_is_setup = false;

/**
 * @brief Setup function for SPI HAL tests
 *
 * DEPENDENCIES:
 * - device_initialize() must be called first (done in test_main.c)
 *
 * This function is idempotent - safe to call multiple times.
 */
static void setup_spi_hal(void)
{
    if (spi_hal_is_setup) {
        return;  /* Already initialized */
    }

    spi_error_t result = spi_proto_hal_init(false);  /* false = slave mode */
    if (result == SPI_OK) {
        spi_hal_is_setup = true;
    }
}

/**
 * @brief Teardown function for SPI HAL tests
 *
 * Deinitializes SPI HAL so each test starts with a clean state.
 */
static void teardown_spi_hal(void)
{
    spi_proto_hal_deinit();
    spi_hal_is_setup = false;
}

/**
 * @brief Test SPI HAL initialization returns success
 */
DEFINE_TEST(test_spi_hal_init_success)
{
    /* Setup function already called spi_proto_hal_init */
    TEST_ASSERT(spi_hal_is_setup, "SPI HAL initialization succeeded");
}

/**
 * @brief Test SPI HAL rejects master mode
 */
DEFINE_TEST(test_spi_hal_rejects_master_mode)
{
    spi_error_t result = spi_proto_hal_init(true);  /* true = master mode */
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "init(master) returns error");
}

/**
 * @brief Test SPI HAL init is idempotent
 */
DEFINE_TEST(test_spi_hal_init_idempotent)
{
    /* Call init again - should return OK (already initialized) */
    spi_error_t result = spi_proto_hal_init(false);
    TEST_ASSERT_EQUAL(SPI_OK, result, "Second init call returns SPI_OK");
}

/**
 * @brief Test transmit validates NULL data pointer
 */
DEFINE_TEST(test_spi_hal_transmit_null_data)
{
    spi_error_t result = spi_proto_hal_transmit(NULL, 10, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transmit with NULL data returns error");
}

/**
 * @brief Test transmit validates zero length
 */
DEFINE_TEST(test_spi_hal_transmit_zero_length)
{
    uint8_t buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transmit(buffer, 0, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transmit with zero length returns error");
}

/**
 * @brief Test transmit validates oversized length
 */
DEFINE_TEST(test_spi_hal_transmit_oversized)
{
    uint8_t buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transmit(buffer, SPI_MAX_PAYLOAD_SIZE + 100, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transmit with oversized length returns error");
}

/**
 * @brief Test receive validates NULL buffer pointer
 */
DEFINE_TEST(test_spi_hal_receive_null_buffer)
{
    spi_error_t result = spi_proto_hal_receive(NULL, 10, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Receive with NULL buffer returns error");
}

/**
 * @brief Test receive validates zero length
 */
DEFINE_TEST(test_spi_hal_receive_zero_length)
{
    uint8_t buffer[10] = {0};
    spi_error_t result = spi_proto_hal_receive(buffer, 0, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Receive with zero length returns error");
}

/**
 * @brief Test receive validates oversized length
 */
DEFINE_TEST(test_spi_hal_receive_oversized)
{
    uint8_t buffer[10] = {0};
    spi_error_t result = spi_proto_hal_receive(buffer, SPI_MAX_PAYLOAD_SIZE + 100, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Receive with oversized length returns error");
}

/**
 * @brief Test transfer validates NULL tx_data pointer
 */
DEFINE_TEST(test_spi_hal_transfer_null_tx)
{
    uint8_t rx_buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transfer(NULL, rx_buffer, 10, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transfer with NULL tx_data returns error");
}

/**
 * @brief Test transfer validates NULL rx_buffer pointer
 */
DEFINE_TEST(test_spi_hal_transfer_null_rx)
{
    uint8_t tx_buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transfer(tx_buffer, NULL, 10, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transfer with NULL rx_buffer returns error");
}

/**
 * @brief Test transfer validates zero length
 */
DEFINE_TEST(test_spi_hal_transfer_zero_length)
{
    uint8_t tx_buffer[10] = {0};
    uint8_t rx_buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transfer(tx_buffer, rx_buffer, 0, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transfer with zero length returns error");
}

/**
 * @brief Test transfer validates oversized length
 */
DEFINE_TEST(test_spi_hal_transfer_oversized)
{
    uint8_t tx_buffer[10] = {0};
    uint8_t rx_buffer[10] = {0};
    spi_error_t result = spi_proto_hal_transfer(tx_buffer, rx_buffer, SPI_MAX_PAYLOAD_SIZE + 100, 1000);
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "Transfer with oversized length returns error");
}

/**
 * @brief Test READY signal initialization for slave
 */
DEFINE_TEST(test_spi_hal_ready_init_slave)
{
    /* READY signal is already initialized by setup_spi_hal() */
    /* Re-initializing should work */
    spi_error_t result = spi_proto_hal_ready_init(false);  /* slave mode */
    TEST_ASSERT_EQUAL(SPI_OK, result, "ready_init(slave) returns SPI_OK");
}

/**
 * @brief Test READY signal rejects master mode
 */
DEFINE_TEST(test_spi_hal_ready_rejects_master)
{
    spi_error_t result = spi_proto_hal_ready_init(true);  /* master mode */
    TEST_ASSERT_EQUAL(SPI_ERROR_HAL_ERROR, result, "ready_init(master) returns error");
}

/**
 * @brief Test READY signal write (slave function)
 */
DEFINE_TEST(test_spi_hal_ready_write)
{
    /* Test writing READY signal states */
    /* Write LOW (not ready/busy) */
    spi_proto_hal_ready_write(false);
    TEST_ASSERT(true, "ready_write(false) completes");

    /* Write HIGH (ready) */
    spi_proto_hal_ready_write(true);
    TEST_ASSERT(true, "ready_write(true) completes");

    /* Note: spi_proto_hal_ready_read() is for MASTER only, not tested here */
}

/**
 * @brief Test HAL get_tick_ms returns constant value
 */
DEFINE_TEST(test_spi_hal_get_tick_ms)
{
    uint32_t tick = spi_proto_hal_get_tick_ms();

    TEST_ASSERT_NOT_EQUAL(0, tick, "get_tick_ms returns non-zero");
    DEBUG_DEBUG("    SPI HAL tick_ms: %lu", tick);

    /* Should return same value (it's a constant) */
    uint32_t tick2 = spi_proto_hal_get_tick_ms();
    TEST_ASSERT_EQUAL(tick, tick2, "get_tick_ms returns same value");
}

/**
 * @brief Test HAL delay_us function
 */
DEFINE_TEST(test_spi_hal_delay_us)
{
    spi_proto_hal_delay_us(100);
    TEST_ASSERT(true, "delay_us(100) completes");

    spi_proto_hal_delay_us(1000);
    TEST_ASSERT(true, "delay_us(1000) completes");
}

/**
 * @brief Test HAL delay_ms function
 */
DEFINE_TEST(test_spi_hal_delay_ms)
{
    spi_proto_hal_delay_ms(1);
    TEST_ASSERT(true, "delay_ms(1) completes");

    spi_proto_hal_delay_ms(10);
    TEST_ASSERT(true, "delay_ms(10) completes");
}

/**
 * @brief Test DMA callback functions exist
 */
DEFINE_TEST(test_spi_hal_callbacks)
{
    /* These are currently empty but should not crash */
    spi_proto_hal_tx_complete_callback();
    TEST_ASSERT(true, "tx_complete_callback completes");

    spi_proto_hal_rx_complete_callback();
    TEST_ASSERT(true, "rx_complete_callback completes");

    spi_proto_hal_error_callback();
    TEST_ASSERT(true, "error_callback completes");
}

/**
 * @brief Register all SPI HAL tests
 *
 * Tests that require SPI initialization use setup_spi_hal and teardown_spi_hal.
 * The setup function is idempotent and tracks state to avoid redundant initialization.
 */
void register_spi_hal_tests(void)
{
    /* Initialization tests - use setup and teardown */
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_init_success, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST("SPI_HAL", test_spi_hal_rejects_master_mode);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_init_idempotent, setup_spi_hal, teardown_spi_hal);

    /* Parameter validation tests - require SPI init */
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transmit_null_data, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transmit_zero_length, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transmit_oversized, setup_spi_hal, teardown_spi_hal);

    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_receive_null_buffer, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_receive_zero_length, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_receive_oversized, setup_spi_hal, teardown_spi_hal);

    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transfer_null_tx, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transfer_null_rx, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transfer_zero_length, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_transfer_oversized, setup_spi_hal, teardown_spi_hal);

    /* READY signal tests - require SPI init */
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_ready_init_slave, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_ready_rejects_master, setup_spi_hal, teardown_spi_hal);
    REGISTER_TEST_WITH_SETUP_TEARDOWN("SPI_HAL", test_spi_hal_ready_write, setup_spi_hal, teardown_spi_hal);

    /* Utility function tests - no SPI init required */
    REGISTER_TEST("SPI_HAL", test_spi_hal_get_tick_ms);
    REGISTER_TEST("SPI_HAL", test_spi_hal_delay_us);
    REGISTER_TEST("SPI_HAL", test_spi_hal_delay_ms);
    REGISTER_TEST("SPI_HAL", test_spi_hal_callbacks);
}
