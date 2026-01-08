# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Embedded firmware for ML63Q2537 (ROHM Solist-AI) MCU, implementing SPI slave communication with a custom protocol for high-performance data transfer (1.2-1.4 MB/s). This project is designed for on-device AI applications requiring large dataset transfers.

**Hardware:**
- MCU: ML63Q2537 (ARM Cortex-M0+ @ 48MHz)
- Flash: 256KB at 0x10000000
- RAM: 16KB at 0x20000000
- Communication: SPI slave (SSIOF0 peripheral)

## Build Commands

### Standard Build Workflow

```bash
# Initial setup (one-time)
git clone --depth 1 --branch v6.2.0 https://github.com/ARM-software/CMSIS_6.git external/CMSIS

# Build
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4
```

### Build Variants

```bash
# Debug build (with symbols, -O0)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized, -Os for size)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build with tests
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
make -j4
```

### Output Files

After build, the following files are generated in `build/`:
- `solist_ai_template` - ELF executable (main application)
- `solist_ai_template.hex` - Intel HEX for flashing
- `solist_ai_template.bin` - Raw binary

When built with `-DBUILD_TESTS=ON`:
- `solist_ai_template_test` - Test binary ELF
- `solist_ai_template_test.hex` - Test binary HEX
- `solist_ai_template_test.bin` - Test binary

## Flashing

### Using OpenOCD (CMSIS-DAP)

```bash
# Flash from build directory
cd build
openocd -f ../openocd/openocd.cfg -c "program solist_ai_template.hex verify reset exit"
```

Alternative configurations available in `openocd/`:
- `openocd-jlink.cfg` - For J-Link debuggers
- `openocd-jlink-ram.cfg` - RAM-only debugging

### Using J-Link

```bash
JLinkExe -device ML63Q2537 -if SWD -speed 1000
# In J-Link console: connect, loadfile build/solist_ai_template.hex, r, g
```

## Architecture

### Core Components

**1. Device HAL (`src/device.h`, `src/device.c`)**
- System initialization and clock management
- Timing utilities: `device_delay_us()`, `device_delay_ms()`, `device_get_tick_ms()`
- System clock query: `device_get_sysclk()`

**2. SPI Protocol HAL (`src/spi_hal_solist_ai.c`)**
- Implements HAL interface for `external/spi_protocol` library
- SSIOF0 peripheral driver wrapper
- Slave mode only (master mode returns error)
- FIFO-based transmit/receive with timeout handling
- Optional READY signal on Port 8.2 for flow control

**3. Debug UART (`src/debug_uart.h`, `src/debug_uart.c`)**
- Logging macros: `DEBUG_DEBUG()`, `DEBUG_WARN()`, etc.
- Uses UARTF for debug output

**4. Driver Layer (`driver/`)**
- Low-level peripheral drivers for ML63Q2537
- Key drivers: SSIOF (SPI), UARTF (UART), WDT (watchdog), timers, DMA, etc.
- Built as static library linked to main application

**5. SPI Protocol Library (`external/spi_protocol/`)**
- Platform-agnostic SPI communication protocol
- CRC-16 error detection, automatic retry, flow control
- Supports large dataset transfers with auto-packetization
- HAL abstraction layer (implemented in `src/spi_hal_solist_ai.c`)

### SPI Configuration

The SPI slave is configured in `src/spi_hal_solist_ai.c:130`:
- Mode: Slave
- Data size: 8-bit
- Clock phase: CPHA_1SM_2SH (sample on 1st edge, shift on 2nd)
- Clock polarity: CPOL_LOW (idle low)
- Port: Port 4 (SSIOF0)
- READY signal: Port 8.2 (when `SPI_USE_READY_SIGNAL` enabled)

### Interrupt Handling

Application-level IRQ handlers in `src/app_irq.c`.

### Critical Considerations

**Watchdog Timer:**
- Always active, must call `wdt_clear()` regularly in main loop
- Configured in `device_initialize()` with 2-second timeout

**SPI Transmission:**
- `s_spi_proto_transmit()` in `src/spi_hal_solist_ai.c:65` handles FIFO-based communication
- Clears watchdog during long transfers
- Uses polling (not interrupt/DMA currently)
- Error handling for overrun and mode fault

**Memory Constraints:**
- Stack: 1KB (linker script)
- Heap: 3KB (linker script)
- SPI buffers: `SPI_BUFFER_SIZE = SPI_MAX_PAYLOAD_SIZE + 16`

## Testing

### Hardware Test Binary

A dedicated test binary (`tests/test_main.c`) runs on the ARM Cortex-M0+ hardware to verify basic functionality.

**Build test binary:**
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
make -j4
```

This generates `solist_ai_template_test.hex` and `solist_ai_template_test.bin` in the build directory.

**Test coverage:**
- Device initialization and system clock
- Watchdog timer functionality
- Timing functions (delays and tick counter)
- Debug UART output

**Adding new tests:**
Add test functions in `tests/test_main.c` and call them from `main()`.

## Development Workflow

### Adding Source Files

1. Add `.c` file to `src/`
2. Update `src/CMakeLists.txt` to include it in `APP_SOURCES`
3. Rebuild

### Using Peripheral Drivers

Include driver headers from `driver/inc/`:
```c
#include "ssiof0.h"    // SPI
#include "uartf0.h"    // UART
#include "timer0_1.h"  // Timers
#include "dmac0.h"     // DMA
```

### Modifying SPI Protocol

The SPI protocol library is in `external/spi_protocol/`. To modify:
1. Update protocol code in `external/spi_protocol/src/`
2. Update HAL implementation in `src/spi_hal_solist_ai.c`
3. Configuration is in `external/spi_protocol/include/spi_config.h`

Key HAL functions to implement:
- `spi_proto_hal_init()` - Initialize SPI hardware
- `spi_proto_hal_transmit()` - Send data
- `spi_proto_hal_receive()` - Receive data
- `spi_proto_hal_transfer()` - Full-duplex transfer

### SPI Protocol Configuration

Edit `external/spi_protocol/include/spi_config.h`:
- `SPI_MAX_PAYLOAD_SIZE` - Packet size (256-4096 bytes)
- `SPI_MAX_RETRIES` - Retry attempts for failed transfers
- `SPI_TIMEOUT_MS` - Operation timeout
- `SPI_USE_READY_SIGNAL` - Enable/disable flow control

## Important Notes

- All code must be compatible with C99 standard
- The project uses ARM GCC toolchain (`arm-none-eabi-gcc`)
- Startup code and linker script are in `ml63q25x7/Source/`
- CMSIS headers required (must be cloned into `external/CMSIS`)
- Current implementation in `spi_hal_solist_ai.c` has placeholder returns (`SPI_ERROR_HAL_ERROR`) after transmit/receive operations - these need to be updated when finalizing implementation
- OpenOCD configuration may need adjustment for flash programming (currently uses `stm32f1x` driver as placeholder)
