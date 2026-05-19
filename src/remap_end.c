/*****************************************************************************
 * @file    remap_end.c
 * @brief   REMAPCON write + system reset trampoline
 *
 * Linked into the .remap_end section, which has its load address in flash but
 * its execution VMA in RAM. Switching REMAPCON reconfigures address 0 under
 * the running code, so this routine must execute from RAM where its mapping
 * is not affected.
 *****************************************************************************/

#include "mcu.h"
#include "rdwr_reg.h"

void Remap_End(void)
{
    /* IAP path: REMAPCON = 0x10 — boot from user firmware at 0x10000000. */
    write_reg32(LSICNT->REMAPCON, 0x00000010);

    NVIC_SystemReset();
}
