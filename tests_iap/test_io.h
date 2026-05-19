/*****************************************************************************
 * @file    test_io.h
 * @brief   Minimal UART output used by tests_iap (synchronous, no IRQs)
 *
 * The IAP application uses uartf0_i in callback/interrupt mode. Tests run
 * stand-alone and do not need the state machine; they just print a few lines.
 * This module wraps uartf0_init + busy-wait writes so the test framework can
 * stay simple.
 *****************************************************************************/

#ifndef TEST_IO_H__
#define TEST_IO_H__

#include <stdint.h>

void test_io_init(void);
void test_io_puts(const char *s);
void test_io_print_u32(uint32_t v);
void test_io_print_u16_hex(uint16_t v);

#endif /* TEST_IO_H__ */
