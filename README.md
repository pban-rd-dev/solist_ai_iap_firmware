# Solist-AI ML63Q2537 Project Template

Minimal CMake project template for Solist-AI (ML63Q2537) development.

## Project Structure

```
.
├── cmake/                      # CMake toolchain configuration
├── driver/                     # Hardware driver library
│   ├── inc/                    # Driver headers
│   └── src/                    # Driver implementations
├── utility/                    # Board support utilities
│   └── board/                  # Board-specific functions
├── src/                        # Application source code
│   └── main.c                  # Application entry point
├── ml63q25x7/                  # MCU-specific files
│   ├── Source/                 # Startup code and linker script
│   └── include/                # CMSIS device headers
└── RTE/                        # Runtime Environment
    └── Device/ML63Q25x7/       # System initialization

```

## Prerequisites

- **ARM GCC Toolchain**: `arm-none-eabi-gcc` (tested with version 10.3 or later)
- **CMake**: 3.16 or higher
- **CMSIS**: ARM Cortex Microcontroller Software Interface Standard

## Setup

### 1. Install CMSIS

```bash
# Clone CMSIS repository
git clone --depth 1 --branch v6.2.0 https://github.com/ARM-software/CMSIS_6.git external/CMSIS
```

## Build

### Basic Build

```bash
# Create build directory
mkdir build && cd build

# Configure project
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build
make -j4
```

### Output Files

After successful build, you will find:
- `solist_ai_template` - ELF executable
- `solist_ai_template.hex` - Intel HEX format (for flashing)
- `solist_ai_template.bin` - Raw binary format

### Build Types

```bash
# Debug build (with symbols, -O0)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized, -Os)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Flash Programming

### Using OpenOCD

```bash
# Convert hex to tcl code
python scripts/convert_hex_to_tcl.py build/solist_ai_gcc_template.hex flash_from_hex.tcl

# Start openocd
openocd -f openocd/openocd.cfg 
telnet localhost 4444
```

```bash
# Connect to openocd
telnet localhost 4444

# Run the script
$ > load flash_from_hex.tcl
```

### Using J-Link

TODO

## Hardware Information

- **MCU**: ML63Q2537 (ROHM/Lapis)
- **Core**: ARM Cortex-M0+ @ 48MHz
- **Flash**: 256KB (starts at 0x10000000)
- **RAM**: 16KB (starts at 0x20000000)

## Application Entry Point

The template includes a minimal `main.c` with:
- Watchdog timer initialization (2-second timeout)
- Main loop with WDT clearing
- Simple delay function

### Minimal Example

```c
#include <stdint.h>
#include "ML63Q25x7.h"
#include "wdt.h"

int main(void)
{
    /* Initialize watchdog timer */
    wdt_init(WDT_2S);
    wdt_clear();

    /* Main loop */
    while (1) {
        wdt_clear();

        /* Your application code here */
    }

    return 0;
}
```

## Development

### Adding Source Files

1. Add your .c files to `src/` directory
2. Update `src/CMakeLists.txt`:

```cmake
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/your_file.c
    PARENT_SCOPE
)
```

### Using Drivers

Available drivers in `driver/` directory:
- WDT (Watchdog Timer)
- SSIOF0 (SPI)
- UARTF0 (UART)
- IRQ (Interrupt controller)

Include headers:
```c
#include "wdt.h"
#include "ssiof0.h"
#include "uartf0.h"
```

### Using Utilities

Board utilities in `utility/board/`:
- LED control
- Peripheral initialization helpers

## Notes

- **Watchdog Timer**: Always active, must be cleared regularly with `wdt_clear()`
- **Flash Programming**: Requires 48MHz system clock
- **Stack Size**: 1KB (configured in linker script)
- **Heap Size**: 3KB (configured in linker script)

## Troubleshooting

### Build Errors

1. **Toolchain not found**:
   ```bash
   # Check ARM GCC installation
   which arm-none-eabi-gcc
   ```

2. **CMSIS headers missing**:
   ```bash
   # Verify CMSIS installation
   ls external/CMSIS/CMSIS/Core/Include/
   ```

### Flashing Issues

- Ensure SWD connection is properly wired
- Check debug probe (J-Link, CMSIS-DAP, etc.) is recognized
- Verify OpenOCD configuration matches your debug probe

## License

Check individual source files for license information.
