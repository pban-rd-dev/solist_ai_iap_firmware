/*****************************************************************************
 * @file     main.c
 * @brief    Hello World template for ML63Q2537 Solist-AI
 * @version  1.0
 *****************************************************************************/

#include <stdint.h>
#include <string.h>
#include "ML63Q25x7.h"

// driver layer
#include "smpl_common.h"
#include "wdt.h"

// spi_protocol layer
#include "spi_hal.h"
#include "spi_protocol.h"

// app layer
#include "main.h"
#include "device.h"
#include "debug_uart.h"


spi_protocol_ctx_t ctx;

/**
 * @brief  Main program
 * @retval int
 */
int main(void)
{
  uint16_t len = 0;
  /* */
  if (device_initialize() != 0) {
    // Device initialization Error
    return -1;
  }

  spi_protocol_init(&ctx, false); // slave
  if (spi_proto_hal_init(false) != SPI_OK) {
    // SPI_PROTOCOL HAL Error
    return -1;
  }

  /* Buffer for SPI transactions (1024 bytes) */
  uint8_t rx_buffer[TRANSACTION_SIZE_MAX];

  while (true) {
    memset(rx_buffer, 0x00, TRANSACTION_SIZE_MAX);
    /* Receive data from master */
    spi_error_t result = spi_protocol_receive_data(&ctx, rx_buffer, sizeof(rx_buffer), &len, 10);
    if (result == SPI_OK) {
      DEBUG_WARN(".");

      /* Show first 32 bytes */
      /* for (uint8_t i = 0; i < 32; i++) { */
      /*   DEBUG_WARN("[%02d] 0x%02x", i, rx_buffer[i]); */
      /* } */
#ifdef EVENT_DEBUG
      /* Dump all captured events */
      uint16_t event_count = 0;
      const spi_event_t* events = spi_get_events(&event_count);
      DEBUG_WARN("=== SPI Event Log (%d events) ===", event_count);
      for (uint16_t i = 0; i < event_count; i++) {
        char tx_ascii = (events[i].tx_data >= 0x20 && events[i].tx_data <= 0x7E) ? events[i].tx_data : '.';
        char rx_ascii = (events[i].rx_data >= 0x20 && events[i].rx_data <= 0x7E) ? events[i].rx_data : '.';
        DEBUG_WARN("[%03d] t=%lu srr=0x%08x fsr=0x%08x cs=%d cnt=%d wcnt=%d tx=0x%02x('%c') rx=0x%02x('%c')",
                   i,
                   events[i].timestamp,
                   events[i].srr,
                   events[i].fsr,
                   events[i].cs_pin,
                   events[i].count,
                   events[i].wcount,
                   events[i].tx_data,
                   tx_ascii,
                   events[i].rx_data,
                   rx_ascii);
      }
      DEBUG_WARN("=== End of SPI Event Log ===");
#endif // EVENT_DEBUG
    } else {
      DEBUG_WARN("SPI error: %d", result);
      wdt_clear();
    }
  }
  return 0;
}
