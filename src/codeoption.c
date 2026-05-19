/*****************************************************************************
 * @file     codeoption.c
 * @brief    Code option block placed at 0x1003FFC0
 *
 * The silicon reads this 64-byte block at power-on. Word 4 (offset 0x10)
 * holds the WDT-related bits; other words are kept in the erased (0xFFFFFFFF)
 * state. See codeoption.h for the bit layout and codeoption_config.h for the
 * configurable values.
 *****************************************************************************/

#include "codeoption_config.h"

const uint32_t codeop_area[CODEOPTION_AREA_SIZE] __attribute__((section(".codeoption"))) = {
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    ((0x1FFFFFFFUL << 3UL) | (CODEOPTION0_WDTPWMD0 << 2UL) | (0x1UL << 1UL) | (CODEOPTION0_WDTMD)),
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
};
