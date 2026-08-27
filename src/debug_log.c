/*****************************************************************************
 * @file    debug_log.c
 * @brief   In-RAM event ring buffer for IAP timing analysis (memory-only)
 *
 * Pure memory-based — events are timestamped (via g_dbg_tick, incremented by
 * LTBC IRQ at 128 Hz) and stored in s_log[]. No UART output: analysis is
 * done by halting the CPU through openocd/SWD and reading s_log[] /
 * s_log_idx / s_log_wrapped directly. See openocd/tools/dump_via_dap.tcl
 * for a DAP-direct read helper that does not require CPU halt.
 *****************************************************************************/

#include "debug_log.h"

#define DBG_LOG_SIZE 32

typedef struct {
    uint32_t tick;
    uint8_t  evt;
    uint8_t  _pad;
    uint16_t arg1;
    uint32_t arg2;
} dbg_record_t;

volatile uint32_t g_dbg_tick = 0;

static dbg_record_t s_log[DBG_LOG_SIZE];
static uint16_t     s_log_idx     = 0;
static uint8_t      s_log_wrapped = 0;

void dbg_log_init( void )
{
    s_log_idx = 0;
    s_log_wrapped = 0;
    g_dbg_tick = 0;
}

void dbg_log_evt( dbg_evt_t evt, uint16_t arg1, uint32_t arg2 )
{
    dbg_record_t *r = &s_log[s_log_idx];
    r->tick = g_dbg_tick;
    r->evt  = (uint8_t)evt;
    r->_pad = 0;
    r->arg1 = arg1;
    r->arg2 = arg2;

    s_log_idx++;
    if ( s_log_idx >= DBG_LOG_SIZE ) {
        s_log_idx = 0;
        s_log_wrapped = 1;
    }
}
