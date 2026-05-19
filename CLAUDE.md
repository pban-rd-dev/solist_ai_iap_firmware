# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Minimal embedded-firmware **template** for the ML63Q2537 (ROHM/Lapis "Solist-AI") MCU using CMake + arm-none-eabi-gcc. Provides toolchain setup, peripheral drivers, board utilities, startup/linker scripts, an on-target test framework, and an ML63Q2537-aware OpenOCD flash workflow. Intended as the starting point for a Solist-AI application.

**Hardware:**
- MCU: ML63Q2537 (ARM Cortex-M0+ @ 48 MHz, PLL-driven)
- Flash: 256 KB at `0x10000000`
- RAM: 16 KB at `0x20000000` (linker reserves 1 KB stack, 3 KB heap)

## Repository Layout

```
src/                application sources — built into solist_ai_template ELF
  main.c                entry point
  device.[ch]           clock + tick + WDT init, delay_us/ms
  uart_print.[ch]       UART_PRINT_DEBUG/WARN/... macros + printf over UARTF0 @ 115200 (P32/P33)
  app_irq.c             app-level IRQ handlers
  syscalls.c            newlib stubs
driver/             ML63Q2537 peripheral drivers, built as static lib `driver`
utility/board/      board helpers (LEDs, periph init), static lib `utility`
external/CMSIS/     ARM CMSIS_6 submodule (REQUIRED — see Setup)
ml63q25x7/Source/   startup (startup_ML63Q25x7.c), system init, GCC linker script
RTE/                Runtime Environment (system init)
cmake/              arm-none-eabi-toolchain.cmake (auto-loaded by top-level CMakeLists)
openocd/openocd.cfg J-Link/SWD config + full TCL flash-programming routines for ML63Q2537
scripts/hex_to_flash.py   converts Intel HEX → TCL script of `flash_write_word` calls
tests/              on-target test binary (test framework + suites)
```

## Setup (one-time)

`external/CMSIS` is a git submodule:

```bash
git submodule update --init --recursive
```

## Build

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..       # Debug: -O0 -g3 -DDEBUG
# or
cmake -DCMAKE_BUILD_TYPE=Release ..     # Release: -Os -DNDEBUG
make -j4
```

Add `-DBUILD_TESTS=ON` to also build the test binary.

The toolchain file `cmake/arm-none-eabi-toolchain.cmake` is auto-selected by the top-level `CMakeLists.txt` if `CMAKE_TOOLCHAIN_FILE` is unset — do not pass `-DCMAKE_TOOLCHAIN_FILE=...` unless overriding intentionally.

**Outputs in `build/`:**
- `solist_ai_template` (ELF), `.hex` (Intel HEX), `.bin` (raw)
- With `BUILD_TESTS=ON`: `solist_ai_template_test` + `.hex`/`.bin`

CPU/compile flags are set centrally in the top-level `CMakeLists.txt` (`-mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -Wall -Wextra`, C99, `-specs=nano.specs`, `--gc-sections`). The MCU variant macros `ML63Q2537` and `ML63Q25x7` are defined for the target.

## Flashing (ML63Q2537-specific workflow)

**Do not** use a stock `openocd program ... verify reset exit` invocation — the ML63Q2537 has a custom flash controller (FLASHA/FLASHD/FLASHCON/FLASHACP/FLASHSLF) that requires:
1. System clock raised to 48 MHz PLL first.
2. A specific accept-flag write sequence to unlock self-programming.
3. WDT clears interleaved with long erase/write operations (WDT is always on).

All of this is implemented as TCL procs in `openocd/openocd.cfg`. The supported workflow is:

```bash
# 1. Generate a TCL script of flash_write_word calls from the hex file
python3 scripts/hex_to_flash.py build/solist_ai_template.hex flash_from_hex.tcl

# 2. Start OpenOCD in one terminal
openocd -f openocd/openocd.cfg

# 3. From another terminal, drive it via telnet
telnet localhost 4444
> prepare_flash           ;# erase + clock setup; defaults to 0x10000000, 256 KB
> source flash_from_hex.tcl
```

Other useful TCL commands defined in `openocd.cfg`: `test_clock`, `test_flash`, `flash_program <addr> <data>`, `flash_erase_block <addr> <size>`, `program_firmware_with_erase <file>`.

## Testing

Tests run **on-target** (Cortex-M0+) as a separate binary built with `-DBUILD_TESTS=ON`. There is no host-side unit test runner. `tests/test_framework.[ch]` provides minimal `TEST_ASSERT` / suite-runner macros; `tests/test_main.c` is the entry point that calls each suite's run function.

**Adding a suite:** create `tests/test_<area>.c`, add it to `TEST_SOURCES` in `tests/CMakeLists.txt`, and call its run function from `test_main.c`.

## Architecture Notes

**Watchdog** — always active with a 2 s timeout (set in `device_initialize()`). Any loop that takes >2 s (long peripheral transfers, flash erase, big computations) must call `wdt_clear()` periodically, or the chip resets. The OpenOCD TCL flash routines interleave WDT clears for this reason.

**Memory budget** — Stack 1 KB, heap 3 KB (in `ml63q25x7/Source/GCC/ML63Q25x7_gcc.ld`). Total RAM is 16 KB; large static buffers eat meaningfully into it.

## Common Modification Workflows

**Adding application source files:** drop the `.c` into `src/`, append to `APP_SOURCES` in `src/CMakeLists.txt` (note `PARENT_SCOPE`), rebuild.

**Driver headers** live in `driver/inc/`: `ssiof0.h`, `uartf0.h`, `wdt.h`, `irq.h`, `timer0_1.h`, `dmac0.h`, etc.

## Toolchain & Standard

- C99 (`-std=c99`, no GNU extensions: `CMAKE_C_EXTENSIONS OFF`).
- `arm-none-eabi-gcc` (10.3+ tested).
- CMake ≥ 3.16.
