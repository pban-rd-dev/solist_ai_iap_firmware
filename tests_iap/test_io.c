#include "test_io.h"

#include "mcu.h"
#include "rdwr_reg.h"
#include "uartf0_i.h"

/* Match the IAP main's UART config (P32/P33, 115200 @ 48 MHz). */
#define UARTF_PARAM_MODE  ( UARTF_LG_8BIT | UARTF_STP_1BIT | UARTF_PT_NON | \
                            UARTF_BC_DIS  | UARTF_DLAB_RBR_THR | \
                            UARTF_RFR_KEEP | UARTF_TFR_KEEP | UARTF_FTL_2BYTE )
#define UARTF_PARAM_CAJ   ( UARTF_RMV_ENA | 0x0019U )
#define UARTF_PARAM_DLR   ( 0x0019U )

static void s_putc(char c)
{
    while ( uartf0_checkWriteBusy() != 0 ) {}
    uartf0_putc((uint8_t)c);
}

void test_io_init(void)
{
    /* P32 = UARTF0_RX, P33 = UARTF0_TX */
    uint32_t p3mod0 = read_reg32(PORT3->P3MOD0);
    p3mod0 &= ~((0x3FUL << 16) | (0x3FUL << 24));
    p3mod0 |=  ((0x21UL << 16) | (0x23UL << 24));
    write_reg32(PORT3->P3MOD0, p3mod0);
    set_bit(PORT3->P3DO, (1U << 3));

    uartf0_init((uint16_t)UARTF_PARAM_MODE, (uint16_t)UARTF_PARAM_CAJ, (uint16_t)UARTF_PARAM_DLR);
}

void test_io_puts(const char *s)
{
    while (*s) {
        s_putc(*s++);
    }
}

void test_io_print_u32(uint32_t v)
{
    char buf[11];
    int  i = 10;
    buf[i--] = '\0';
    if (v == 0) {
        s_putc('0');
        return;
    }
    while (v > 0 && i >= 0) {
        buf[i--] = '0' + (char)(v % 10);
        v /= 10;
    }
    test_io_puts(&buf[i + 1]);
}

void test_io_print_u16_hex(uint16_t v)
{
    static const char hex[] = "0123456789ABCDEF";
    char buf[7] = "0x0000";
    buf[2] = hex[(v >> 12) & 0xF];
    buf[3] = hex[(v >>  8) & 0xF];
    buf[4] = hex[(v >>  4) & 0xF];
    buf[5] = hex[ v        & 0xF];
    test_io_puts(buf);
}
