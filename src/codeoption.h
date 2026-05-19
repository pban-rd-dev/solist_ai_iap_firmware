/*****************************************************************************
 * @file     codeoption.h
 * @brief    Code option area definition for ML63Q25x7
 *
 * The ML63Q25x7 reserves the top 64 bytes of flash (0x1003FFC0–0x1003FFFF)
 * for a hardware-level configuration block read by silicon at power-on.
 * It is analogous to AVR fuses, STM32 option bytes, or NXP FOPT.
 *****************************************************************************/

#ifndef CODEOPTION_H__
#define CODEOPTION_H__

#include <stdint.h>

#define CODEOPTION_AREA_SIZE                    (16U)

/* CODE OPTION 0 — bit 0: WDTMD, bit 2: WDTPWMD0 */
#define CODEOPTION0_WDTMD_DISABLED              (0UL)
#define CODEOPTION0_WDTMD_ENABLED               (1UL)

#define CODEOPTION0_WDTPWMD0_DISABLED           (0UL)
#define CODEOPTION0_WDTPWMD0_ENABLED            (1UL)

extern const uint32_t codeop_area[CODEOPTION_AREA_SIZE];

#endif /* CODEOPTION_H__ */
