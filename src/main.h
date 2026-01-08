#ifndef __MAIN_H__
#define __MAIN_H__

#define TRANSACTION_SIZE_MAX (1024)
#define EVENT_DEBUG

/* Event structure from spi_hal_solist_ai.c */
typedef struct {
  uint32_t timestamp;
  uint32_t srr;      /* SF0SRR - Status register */
  uint32_t fsr;      /* SF0FSR - FIFO status */
  uint32_t cs_pin;   /* CS pin state */
  uint16_t count;    /* Byte count */
  uint16_t wcount;   /* Write byte count */
  uint8_t tx_data;   /* Data written to FIFO */
  uint8_t rx_data;   /* Data read from FIFO */
} spi_event_t;

extern const spi_event_t* spi_get_events(uint16_t *count);

#endif // __MAIN_H__
