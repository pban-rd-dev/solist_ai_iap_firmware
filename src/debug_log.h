/*****************************************************************************
 * @file    debug_log.h
 * @brief   In-RAM event ring buffer for IAP timing analysis (memory-only)
 *
 * Events are stamped with g_dbg_tick (128 Hz, incremented by LTBC IRQ).
 * No UART output — read s_log[] over SWD after halting the CPU.
 *****************************************************************************/

#ifndef DEBUG_LOG_H__
#define DEBUG_LOG_H__

#include <stdint.h>

typedef enum {
    DBG_EVT_NONE = 0,
    DBG_EVT_BANNER_SENT,
    DBG_EVT_C_SENT,
    DBG_EVT_HDR_SOH,
    DBG_EVT_HDR_STX,
    DBG_EVT_BLOCK_RX_DONE,
    DBG_EVT_CRC_FAIL,
    DBG_EVT_BLOCK_OK,
    DBG_EVT_ERASE_BEGIN,
    DBG_EVT_ERASE_END,
    DBG_EVT_WRITE_BEGIN,
    DBG_EVT_WRITE_END,
    DBG_EVT_VERIFY_FAIL,
    DBG_EVT_ACK_SENT,
    DBG_EVT_NAK_SENT,
    DBG_EVT_CAN_SENT,
    DBG_EVT_EOT_RX,
    DBG_EVT_TIMEOUT,
    DBG_EVT_RETRY,
} dbg_evt_t;

extern volatile uint32_t g_dbg_tick;

void dbg_log_init( void );
void dbg_log_evt( dbg_evt_t evt, uint16_t arg1, uint32_t arg2 );

#endif /* DEBUG_LOG_H__ */
