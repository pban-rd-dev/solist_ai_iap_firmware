/*****************************************************************************
 * @file     codeoption_config.h
 * @brief    Code option values written into the .codeoption flash sector
 *
 * Override these to change the hardware-level startup configuration.
 *****************************************************************************/

#ifndef CODEOPTION_CONFIG_H__
#define CODEOPTION_CONFIG_H__

#include "codeoption.h"

/* WDT enabled at power-on so device_initialize()'s wdt_init() sees a
 * consistent hardware state regardless of the flash erase pattern. */
#define CODEOPTION0_WDTMD                       (CODEOPTION0_WDTMD_ENABLED)
#define CODEOPTION0_WDTPWMD0                    (CODEOPTION0_WDTPWMD0_ENABLED)

#endif /* CODEOPTION_CONFIG_H__ */
