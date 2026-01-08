/**
 * @file spi_hal_solist_ai.c
 * @brief HAL Implementation for solist_ai MCU (SPI Slave)
 *
 * TODO: Implement SPI slave functionality for solist_ai MCU
 */

#include "spi_hal.h"

#include <string.h>

#include "ML63Q25x7.h"
#include "ssiof0.h"
#include "ssiof_common.h"
#include "rdwr_reg.h"
#include "wdt.h"
#include "irq.h"

#include "main.h"
#include "debug_uart.h"
#include "device.h"


static const char *TAG = "SPI_HAL_SOLIST_AI";

/* Event capture buffer for debugging */
#define EVENT_BUFFER_SIZE 128

static spi_event_t events[EVENT_BUFFER_SIZE];
static volatile uint16_t event_idx = 0;
static volatile uint32_t event_seq = 0;  /* Sequence counter for events */

#define USR_PERI                    (UAF0_PERI | SIOF0_PERI)

/* Port 4 configuration for SSIOF0 (SPI slave) */
#define SSIOF_PORT_CONFIG          ((0x11 << 24) | (0x11 << 16) | (0x12 << 8) | (0x11 << 0))

#define GPIO_READY      0   /* TODO: Set correct pin */


/* RX/TX buffers */
#define SPI_BUFFER_SIZE (SPI_MAX_PAYLOAD_SIZE + 16)
static uint8_t rx_buffer[SPI_BUFFER_SIZE];
static uint8_t tx_buffer[SPI_BUFFER_SIZE];
static bool is_initialized = false;

static spi_error_t s_spi_proto_hal_init_sync(bool is_master);
static void s_spi_proto_debug_register(void);
static void s_spi_proto_ssiof_start(void);
static uint16_t s_spi_proto_prepare_fifo(size_t size, uint8_t *tx_data);
static spi_error_t s_spi_proto_transmit(size_t size, uint8_t* tx_data, uint32_t timeout);
static spi_error_t s_spi_proto_receive(size_t size, uint8_t* rx_data, uint32_t timeout);

void s_spi_proto_debug_register(void)
{
  DEBUG_WARN("\tCTRL: 0x%08x, INTC: 0x%08x, TRAC: 0x%08x\n\tBRR: 0x%08x, SRR: 0x%08x, SRC: 0x%08x\n\tFSR: 0x%08x",
             read_reg32(SSIOF0->SF0CTRL), read_reg32(SSIOF0->SF0INTC), read_reg32(SSIOF0->SF0TRAC), read_reg32(SSIOF0->SF0BRR), read_reg32(SSIOF0->SF0SRR), read_reg32(SSIOF0->SF0SRC), read_reg32(SSIOF0->SF0FSR));
}

static void s_spi_proto_ssiof_start(void)
{
  uint32_t ctrl = read_reg32(SSIOF0->SF0CTRL);
  ctrl = ctrl | SFnCTRL_SFnSPE;

  write_reg32(SSIOF0->SF0CTRL, ctrl);
  // DEBUG_WARN("CTRL: 0x%08x", read_reg32(SSIOF0->SF0CTRL));
}

static uint16_t s_spi_proto_prepare_fifo(size_t size, uint8_t *tx_data)
{
  uint16_t cnt = 0;
  uint8_t space = SSIOF_WR_FIFO_MAX - ssiof0_getWriteFifoSize();
  for (uint8_t i = 0; i < space && cnt < size; i++) {
    write_reg8(SSIOF0->SF0DWR, *tx_data);
#ifdef EVENT_DEBUG
    /* Capture TX event */
    events[event_idx].timestamp = event_seq++;
    events[event_idx].srr = read_reg32(SSIOF0->SF0SRR);
    events[event_idx].fsr = read_reg32(SSIOF0->SF0FSR);
    events[event_idx].cs_pin = 0x01;
    events[event_idx].wcount = cnt;
    events[event_idx].tx_data = *tx_data;
    events[event_idx].rx_data = 0;
    event_idx = (event_idx + 1) % EVENT_BUFFER_SIZE;
#endif // EVENT_DEBUG
    tx_data++;
    cnt++;
  }

  s_spi_proto_ssiof_start();
  return cnt;
}


static spi_error_t s_spi_proto_transmit(size_t size, uint8_t* tx_data, uint32_t timeout)
{
  uint32_t wcount = 0;
  uint8_t *tx_ptr = tx_data;
  uint32_t status;
  uint32_t timeout_counter = 0;

  // s_spi_proto_ssiof_start();

  /* Wait for master to assert CS LOW to read our response */
  /* Use loop counter for timeout (rough ~1ms per 10000 iterations at 48MHz) */
  while (true) {
    if (((read_reg32(PORT4->P4DI) >> 3) & 0x01) == 0x00) {
      break;  /* CS is LOW, master is reading */
    }
    timeout_counter++;
    if (timeout_counter > (timeout * 10000)) {  /* Rough timeout approximation */
      ssiof0_stop();
      return SPI_ERROR_TIMEOUT;  /* Master didn't read our response */
    }
    /* Do NOT clear watchdog here - let it catch real hangs */
  }

  status = ssiof0_getStatus();
  ssiof0_clearStatus(status);

  /* Now master is clocking, keep feeding TX FIFO and reading RX FIFO */
  while (((read_reg32(PORT4->P4DI) >> 3) & 0x01) == 0x00) {
    /* Feed more TX data if available */
    if (tx_data != NULL && wcount < size && (status & SFnSRR_SFnSPIF) != 0) {
      uint8_t space = SSIOF_WR_FIFO_MAX - ssiof0_getWriteFifoSize();
      for (int i = space; i > 0; i--) {
        write_reg8(SSIOF0->SF0DWR, *tx_ptr);
#ifdef EVENT_DEBUG
        /* Capture TX event */
        events[event_idx].timestamp = event_seq++;
        events[event_idx].srr = read_reg32(SSIOF0->SF0SRR);
        events[event_idx].fsr = read_reg32(SSIOF0->SF0FSR);
        events[event_idx].cs_pin = 0;
        events[event_idx].count = 0;
        events[event_idx].wcount = wcount;
        events[event_idx].tx_data = *tx_ptr;
        events[event_idx].rx_data = 0;
        event_idx = (event_idx + 1) % EVENT_BUFFER_SIZE;
#endif // EVENT_DEBUG
        tx_ptr++;
        wcount++;
      }
    }
    status = ssiof0_getStatus();
  }
  ssiof0_stop();
  ssiof0_clearStatus(status);
  irq_siof0_clearIRQ();

  return SPI_OK;
}

static spi_error_t s_spi_proto_receive(size_t length, uint8_t *rx_data, uint32_t timeout)
{
  uint16_t count = 0;
  uint8_t *rx_ptr = rx_data;
  uint32_t status;
  uint32_t timeout_counter = 0;

  s_spi_proto_ssiof_start();
  ssiof0_clearFifo();

  /* Wait for master to assert CS LOW to read our response */
  /* Use loop counter for timeout (rough ~1ms per 10000 iterations at 48MHz) */
  while (true) {
    if (((read_reg32(PORT4->P4DI) >> 3) & 0x01) == 0x00) {
      break;  /* CS is LOW, master is reading */
    }
    timeout_counter++;
    if (timeout_counter > (timeout * 10000)) {  /* Rough timeout approximation */
      ssiof0_stop();
      return SPI_ERROR_TIMEOUT;  /* Master didn't read our response */
    }
    /* Do NOT clear watchdog here - let it catch real hangs */
  }

  status = ssiof0_getStatus();
  ssiof0_clearStatus(status);

  /* Now master is clocking, keep feeding TX FIFO and reading RX FIFO */
  while (((read_reg32(PORT4->P4DI) >> 3) & 0x01) == 0x00) {
    /* Read any incoming data from RX FIFO */
    while (ssiof0_getReadFifoSize() > 0) {
      *rx_ptr = read_reg8(SSIOF0->SF0DRR);
#ifdef EVENT_DEBUG
      /* Capture RX event */
      events[event_idx].timestamp = event_seq++;
      events[event_idx].srr = read_reg32(SSIOF0->SF0SRR);
      events[event_idx].fsr = read_reg32(SSIOF0->SF0FSR);
      events[event_idx].cs_pin = 0;
      events[event_idx].count = count;  /* RX events use count (RX progress) */
      events[event_idx].tx_data = 0;
      events[event_idx].rx_data = *rx_ptr;
      event_idx = (event_idx + 1) % EVENT_BUFFER_SIZE;
#endif // EVENT_DEBUG
      rx_ptr++;
      count++;
    }

    status = ssiof0_getStatus();
  }

  /* CS went HIGH, drain remaining RX FIFO */
  while (ssiof0_getReadFifoSize() > 0) {
    *rx_ptr = read_reg8(SSIOF0->SF0DRR);
#ifdef EVENT_DEBUG
    /* Capture RX event */
    events[event_idx].timestamp = event_seq++;
    events[event_idx].srr = read_reg32(SSIOF0->SF0SRR);
    events[event_idx].fsr = read_reg32(SSIOF0->SF0FSR);
    events[event_idx].cs_pin = 1;
    events[event_idx].count = count;  /* RX events use count (RX progress) */
    events[event_idx].tx_data = 0;
    events[event_idx].rx_data = *rx_ptr;
    event_idx = (event_idx + 1) % EVENT_BUFFER_SIZE;
#endif // EVENT_DEBUG
    rx_ptr++;
    count++;
  }

  ssiof0_stop();
  ssiof0_clearStatus(status);
  irq_siof0_clearIRQ();

  return SPI_OK;
}

spi_error_t spi_proto_hal_init(bool is_master) {
  s_spi_proto_hal_init_sync(is_master);
}

/**
 * @brief Initialize SPI hardware (Slave mode)
 */
static spi_error_t s_spi_proto_hal_init_sync(bool is_master) {
  uint32_t ctrl_config;
  uint32_t int_config;

  if (is_initialized) {
    return SPI_OK;
  }

  if (is_master) {
    /* This HAL is for slave mode only */
    return SPI_ERROR_HAL_ERROR;
  }

  /* Configure Port 4 for SSIOF0 (SPI slave mode) */
  write_reg32(PORT4->P4MOD0, SSIOF_PORT_CONFIG);

  /* Configure SSIOF as slave mode, 8-bit, SPI mode */
  ctrl_config = SSIOF_MST_SLAVE |         /* Slave mode */
    SSIOF_LG_8BIT |           /* 8-bit data */
    SSIOF_MDF_DIS |           /* Mode fault disable */
    SSIOF_DIR_MSB |           /* MSB first */
    SSIOF_CPHA_1SM_2SH |      /* Clock phase */
    // SSIOF_CPHA_1SH_2SM |
    SSIOF_CPOL_LOW |          /* Clock polarity low */
    // SSIOF_CPOL_HIGH |
    // SSIOF_SSZ_HIZ |           /* SS high-z (input) */
    SSIOF_SOZ_HIZ |           /* MISO high-z enable */
    SSIOF_MOZ_OUTPUT;         /* MOSI output (input) */

  /* Configure interrupt settings (initially disabled) */
  int_config = SSIOF_INT_TFIE_ENA |       /* Transmit FIFO interrupt enable */
    SSIOF_INT_RFIE_ENA |       /* Receive FIFO interrupt enable */
    SSIOF_INT_FIE_ENA |        /* transfer fnished interrupt enable */
    // SSIOF_INT_ORIE_ENA |       /* Overrun interrupt enable */
    // SSIOF_INT_MFIE_ENA |       /* Mode fault interrupt enable */
    SSIOF_INT_WR_THRESH_0 |    /* Write threshold */
    SSIOF_INT_RD_THRESH_1;     /* Read threshold */

  ssiof0_stop();

  /* Initialize SSIOF */
  ssiof0_init(ctrl_config, int_config);

  /* Set enable bit */
  ctrl_config = read_reg32(SSIOF0->SF0CTRL);
  ctrl_config = ctrl_config | SFnCTRL_SFnSPE;
  write_reg32(SSIOF0->SF0CTRL, ctrl_config);

  /* Initialize READY signal */
  spi_error_t ready_err = spi_proto_hal_ready_init(is_master);
  if (ready_err != SPI_OK) {
    return ready_err;
  }

  is_initialized = true;
  return SPI_OK;
}


/**
 * @brief Deinitialize SPI hardware
 */
void spi_proto_hal_deinit(void) {
  if (!is_initialized) {
    return;
  }

  ssiof0_stop();
  is_initialized = false;
}

/**
 * @brief Transmit data over SPI (Slave mode)
 */
spi_error_t spi_proto_hal_transmit(const uint8_t *data, size_t length, uint32_t timeout_ms) {
  spi_error_t rtn;
  if (!is_initialized) {
    return SPI_ERROR_HAL_ERROR;
  }

  if (data == NULL || length == 0 || length > SPI_BUFFER_SIZE) {
    return SPI_ERROR_HAL_ERROR;
  }

  memcpy(tx_buffer, data, length);
  rtn = s_spi_proto_transmit(length, tx_buffer, timeout_ms);

  return rtn;
}

/**
 * @brief Receive data over SPI (Slave mode)
 */
spi_error_t spi_proto_hal_receive(uint8_t *buffer, size_t length, uint32_t timeout_ms) {
  spi_error_t rtn;
  if (!is_initialized) {
    return SPI_ERROR_HAL_ERROR;
  }

  if (buffer == NULL || length == 0 || length > SPI_BUFFER_SIZE) {
    return SPI_ERROR_HAL_ERROR;
  }

  memset(tx_buffer, 0x00, length);
  rtn = s_spi_proto_receive(length, rx_buffer, timeout_ms);
  if (rtn != SPI_OK) {
    return rtn;
  }
  memcpy(buffer, rx_buffer, length);

  return SPI_OK;
}

/**
 * @brief Transmit and receive data simultaneously (Slave mode)
 */
spi_error_t spi_proto_hal_transfer(const uint8_t *tx_data, uint8_t *rx_buffer_out,
                                   size_t length, uint32_t timeout_ms) {
  return SPI_ERROR_HAL_ERROR;
}

/**
 * @brief Prepare response data for slave transmission (Slave only)
 *
 * For SPI slave with 4-byte FIFO and 8-byte packets:
 * - Loads initial 4 bytes into TX FIFO
 * - Waits for master to assert CS (initiate read)
 * - Continuously feeds remaining bytes while CS is low
 */
size_t spi_proto_hal_prepare_response(const uint8_t *data, size_t length) {
  if (!is_initialized) {
    return 0;  // Return 0 bytes loaded on error
  }

  if (data == NULL || length == 0 || length > SPI_BUFFER_SIZE) {
    return 0;  // Return 0 bytes loaded on error
  }

  memcpy(tx_buffer, data, length);
  uint16_t loaded = s_spi_proto_prepare_fifo(length, tx_buffer);

  return loaded;
}

/**
 * @brief Initialize READY signal GPIO
 */
spi_error_t spi_proto_hal_ready_init(bool is_master) {
  if (is_master) {
    return SPI_ERROR_HAL_ERROR;  /* This HAL is for slave only */
  }
  if (!is_master) {
    uint32_t bits = 0;
    bits |= 0x01 << 18; // P82 PULLUP
    bits |= 0x01 << 17; // P82 OUTPUT
    write_reg32(PORT8->P8MOD0, bits);

    bits = 0;
    bits |= 0x00 << 2; // default LOW(not ready)
    write_reg32(PORT8->P8DO, bits);
  }

  return SPI_OK;
}

/**
 * @brief Read READY signal state (Not used in slave mode)
 */
bool spi_proto_hal_ready_read(void) {
  uint32_t bits = read_reg32(PORT8->P8DI);
  return (bits & (0x01 << 2)) != 0x00;
}

/**
 * @brief Set READY signal state (Slave only)
 */
void spi_proto_hal_ready_write(bool ready) {
  uint32_t bits = read_reg32(PORT8->P8DO);  // Read current port state
  if (ready) {
    bits |= 0x01 << 2;   // Set bit 2 to HIGH
  } else {
    bits &= ~(0x01 << 2); // Clear bit 2 to LOW
  }
  write_reg32(PORT8->P8DO, bits);
}

/**
 * @brief Wait for READY signal (Not used in slave mode)
 */
bool spi_proto_hal_ready_wait(uint32_t timeout_us) {
  return false;
}

/**
 * @brief Get current tick count in milliseconds
 */
uint32_t spi_proto_hal_get_tick_ms(void) {
  uint32_t sysclk = device_get_sysclk();
  return sysclk / 1000;
}

/**
 * @brief Delay for specified microseconds
 */
void spi_proto_hal_delay_us(uint32_t us) {
  device_delay_us(us);
}

/**
 * @brief Delay for specified milliseconds
 */
void spi_proto_hal_delay_ms(uint32_t ms) {
  device_delay_ms(ms);
}

/* Debug helper to access event buffer */
const spi_event_t* spi_get_events(uint16_t *count) {
  *count = event_idx;
  return events;
}

/* Optional DMA callbacks */
void spi_proto_hal_tx_complete_callback(void) {
  /* TODO: Implement if using DMA */
}

void spi_proto_hal_rx_complete_callback(void) {
  /* TODO: Implement if using DMA */
}

void spi_proto_hal_error_callback(void) {
  /* TODO: Implement error handling */
}

#if SPI_DEBUG
#include <stdarg.h>
#include <stdio.h>

void spi_proto_log_debug(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  DEBUG_DEBUG("%s", buffer);
}

void spi_proto_log_info(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  DEBUG_INFO("%s", buffer);
}

void spi_proto_log_warn(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  DEBUG_WARN("%s", buffer);
}

void spi_proto_log_error(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  DEBUG_ERROR("%s", buffer);
}
#endif
