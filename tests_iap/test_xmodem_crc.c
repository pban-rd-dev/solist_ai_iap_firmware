/*****************************************************************************
 * @file    test_xmodem_crc.c
 * @brief   CRC-16 (XMODEM) correctness checks against known test vectors
 *
 * Reference: XMODEM-CRC uses polynomial 0x1021, MSB-first, init 0. Common
 * test vectors:
 *   "123456789"                  -> 0x31C3
 *   single 0x00 byte             -> 0x0000
 *   single 0xA5 byte             -> 0xC1AB
 *****************************************************************************/

#include <stdint.h>

#include "xmodem_crc.h"
#include "test_io.h"

static uint32_t s_failures = 0;

static uint16_t compute_crc(const uint8_t *data, uint32_t len)
{
    uint16_t crc = 0;
    for (uint32_t i = 0; i < len; i++) {
        crc = UpdCRC16(data[i], crc);
    }
    return crc;
}

static void check(const char *label, uint16_t actual, uint16_t expected)
{
    test_io_puts("  ");
    test_io_puts(label);
    test_io_puts(" -> ");
    test_io_print_u16_hex(actual);
    if (actual == expected) {
        test_io_puts(" [PASS]\r\n");
    } else {
        test_io_puts(" [FAIL] expected ");
        test_io_print_u16_hex(expected);
        test_io_puts("\r\n");
        s_failures++;
    }
}

uint32_t run_xmodem_crc_tests(void)
{
    s_failures = 0;

    test_io_puts("xmodem_crc:\r\n");

    {
        const uint8_t buf[] = { '1','2','3','4','5','6','7','8','9' };
        check("\"123456789\"", compute_crc(buf, sizeof(buf)), 0x31C3);
    }
    {
        const uint8_t buf[] = { 0x00 };
        check("0x00",          compute_crc(buf, sizeof(buf)), 0x0000);
    }
    {
        const uint8_t buf[] = { 0xA5 };
        check("0xA5",          compute_crc(buf, sizeof(buf)), 0xC1AB);
    }
    {
        /* Empty input must leave the CRC at the initial value (0). */
        check("(empty)",       compute_crc((const uint8_t *)"", 0), 0x0000);
    }

    return s_failures;
}
