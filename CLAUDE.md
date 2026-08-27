# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Pre-installed **IAP (In-Application Programming) firmware** for the ML63Q2537 (ROHM/Lapis "Solist-AI") MCU, built with CMake + arm-none-eabi-gcc. Factory-flashed into the top 32 KB of internal flash on customer devices. At boot, the IAP brings up UARTF0, accepts an XMODEM-CRC transfer of a user-firmware image, programs it into the user-firmware region, then remaps + resets to boot the new image.

This is the firmware that ships on the device. End-user applications are linked separately and delivered through the IAP update flow.

**Hardware:**
- MCU: ML63Q2537 (ARM Cortex-M0+ @ 48 MHz, PLL-driven)
- Flash: 256 KB at `0x10000000`
- RAM: 16 KB at `0x20000000` (linker reserves 1 KB stack, 3 KB heap)
- Update transport: UARTF0 on P32 (RX) / P33 (TX), 115200 8N1 (hard MCU cap)

**Flash memory map:**
| Region | Range                       | Contents                                                      |
| ------ | --------------------------- | ------------------------------------------------------------- |
| User   | `0x10000000` – `0x1003BFFF` | End-user application (programmed via XMODEM by the IAP)       |
| FLASH2 | `0x1003C000` – `0x1003DFFF` | IAP `.iap2` + `.remap_end` LMA + `.data` LMA (`iap_data.bin`) |
| FLASH  | `0x1003E000` – `0x1003FFBF` | IAP `.text` + ARM.exidx + tables (`iap_code.bin`)             |
| FLASH3 | `0x1003FFC0` – `0x1003FFFF` | Code option (`iap_codeoption.bin`)                            |

## Repository Layout

```
src/                IAP application — built into solist_ai_iap_firmware ELF
  main.c              IAP entry — receive XMODEM, program user flash, remap, reset
  xmodem.[ch]         XMODEM-CRC receiver (dual SOH/STX block size)
  xmodem_crc.[ch]     CRC-16-CCITT
  uartf0_i.[ch]       UARTF0 driver instance used by the IAP
  remap_end.c         Flash-remap + reset sequence (executed from RAM)
  codeoption.c/.h     Code-option section contents
  debug_log.[ch]      In-RAM event log for slowness analysis
  device.[ch]         Clock + tick + WDT init, delay_us/ms
  app_irq.c           App-level IRQ handlers
  syscalls.c          Newlib stubs
driver/             ML63Q2537 peripheral drivers (static lib `driver`)
utility/board/      Board helpers (LEDs, periph init) — static lib `utility`
external/CMSIS/     ARM CMSIS_6 submodule (REQUIRED — see Setup)
ml63q25x7/Source/   Startup, system init, GCC linker scripts
  GCC/ML63Q25x7_iap.ld   IAP linker script (FLASH/FLASH2/FLASH3/REMAP layout)
RTE/                Runtime Environment (system init)
cmake/              arm-none-eabi-toolchain.cmake (auto-loaded by top-level CMakeLists)
jlink/              SEGGER J-Link device definition + vendor CMSIS flash algorithm
  JLinkDevices.xml       ML63Q2537 flash bank wired to ML63Q25x7.FLM
  ML63Q25x7.FLM          Flash algorithm, verbatim from ROHM.ML63Q25x7_DFP 0.4.0
openocd/            Modular OpenOCD config: interface/ + target/ + top-level cfg
scripts/
  jlink_flash.sh      Program the IAP (or any image) via JLinkExe — the fast path
  jlink_flash.ps1     Same flow for Windows / JLink.exe (PowerShell 5.1+)
  iap_flash.py        Generate TCL to flash the three IAP bins at their addresses
  hex_to_flash.py     Generate TCL from a full .hex (used for non-IAP test builds)
tests_iap/          On-target IAP test binary (XMODEM CRC, I/O)
```

## Setup (one-time)

`external/CMSIS` is a git submodule:

```bash
git submodule update --init --recursive
```

## Build

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..    # Release: -Os -DNDEBUG
# or
cmake -DCMAKE_BUILD_TYPE=Debug ..      # Debug:   -O0 -g3 -DDEBUG
make -j4
```

Add `-DBUILD_IAP_TESTS=ON` to also build the on-target test binary under `tests_iap/`.

The toolchain file `cmake/arm-none-eabi-toolchain.cmake` is auto-selected by the top-level `CMakeLists.txt` if `CMAKE_TOOLCHAIN_FILE` is unset — do not pass `-DCMAKE_TOOLCHAIN_FILE=...` unless overriding intentionally.

**Outputs in `build/`:**
- `solist_ai_iap_firmware` (ELF), `solist_ai_iap_firmware.hex` (full Intel HEX, inspection only)
- `iap_code.bin` → programmed at `0x1003E000`
- `iap_data.bin` → programmed at `0x1003C000`
- `iap_codeoption.bin` → programmed at `0x1003FFC0`
- With `BUILD_IAP_TESTS=ON`: `solist_ai_iap_firmware_test` + `.hex`/`.bin`

Run `make bin` to additionally produce `solist_ai_iap_firmware.bin` — whole-image raw binary covering `0x1003C000`–`0x1003FFFF` (16 KB), padded with 0xFF. On-demand only; the three split bins above are what `scripts/iap_flash.py` programs.

The whole-image `.hex` is not what gets programmed in production — the IAP lives in three non-contiguous flash regions and is flashed via the three `.bin` images at their distinct base addresses.

CPU/compile flags are set centrally in the top-level `CMakeLists.txt` (`-mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -Wall -Wextra`, C99, `-specs=nano.specs`, `--gc-sections`). MCU variant macros `ML63Q2537` and `ML63Q25x7` are defined for the target.

## Factory Installation (programming the IAP onto a blank device)

The ML63Q2537 has a custom flash controller (FLASHA/FLASHD/FLASHCON/FLASHACP/FLASHSLF). A stock `openocd program ... verify reset exit` invocation does **not** work — the chip requires:
1. System clock raised to 48 MHz PLL first.
2. A specific accept-flag write sequence to unlock self-programming.
3. WDT clears interleaved with long erase/write operations (WDT is always on).

Two paths implement that sequence. **Use J-Link.**

### J-Link (fast path)

Linux / macOS:

```bash
scripts/jlink_flash.sh            # programs build/{iap_data,iap_code,iap_codeoption}.bin
scripts/jlink_flash.sh <build_dir>
scripts/jlink_flash.sh --file build/solist_ai_iap_firmware_test.hex
```

Windows (PowerShell):

```powershell
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 <build_dir>
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 -Image build\solist_ai_iap_firmware_test.hex
```

Both scripts generate the same J-Link Commander script and honour the same
`JLINK_*` environment variables. They differ only in how they find the J-Link
Commander binary — `JLinkExe` on Linux/macOS, `JLink.exe` on Windows (PATH, then
`%ProgramFiles%\SEGGER\JLink\`, then the versioned `JLink_V*` install dirs).
`-Address` is the PowerShell spelling of the shell script's positional address
argument after `--file`.

The heavy lifting is done by the vendor CMSIS flash algorithm `jlink/ML63Q25x7.FLM`
(from ROHM.ML63Q25x7_DFP 0.4.0), which J-Link downloads into target RAM and runs
there. Its `Init()` raises the clock to the 48 MHz PLL, and its erase/program
loops drive the flash controller and clear the WDT while polling FLASHSTA — the
same sequence as the OpenOCD TCL, but executing on the Cortex-M0+ instead of one
SWD round trip per 32-bit word.

`jlink/JLinkDevices.xml` declares the device (the MCU is not in the J-Link
built-in database): Cortex-M0, work RAM `0x20000000`+16 KB, one flash bank at
`0x10000000` of 256 KB, `LoaderType="FLASH_ALGO_TYPE_OPEN"`. Flash geometry comes
from the algorithm's own FlashDevice descriptor: **2 KB erase sectors**, 1 KB
program pages. `scripts/jlink_flash.sh` points the DLL at that XML with
`exec JLinkDevicesXMLPath = <repo>/jlink/` — the trailing slash is required, the
DLL concatenates the path with `JLinkDevices.xml` verbatim.

Because the erase is per 2 KB sector, only `0x1003C000`–`0x1003FFFF` (the IAP
region proper) is erased. The user region below `0x1003C000` is left intact —
unlike the OpenOCD path, which erases a whole 32 KB block.

Environment overrides: `JLINK_EXE`, `JLINK_SPEED` (kHz, default 4000),
`JLINK_SN`, `JLINK_VTREF` (mV, for probes whose VTref pin is not wired),
`JLINK_NO_RUN=1` (leave the core halted instead of reset-and-run).

Measured on this repo's 4360-byte IAP image (J-Link Lite-Cortex-M V9, SWD
4000 kHz): the full script — connect, erase, program all three bins, read-back
verify, reset and run — takes **2.8 s**, against **10.7 s** for the OpenOCD path.
Most of the J-Link time is fixed connect/prepare overhead; its measured
program+verify rate is 23–32 KB/s, whereas the OpenOCD path costs one host round
trip per programmed 32-bit word, so the gap widens with image size.

### OpenOCD (legacy path, kept as fallback)

Implemented as TCL procs in `openocd/target/ml63q2537.cfg`, driving the flash
controller one word at a time from the host:

```bash
# 1. Generate a TCL script that erases the top 32 KB block and writes the three IAP bins
python3 scripts/iap_flash.py build/ flash_iap.tcl

# 2. Start OpenOCD in one terminal
openocd -f openocd/openocd.cfg

# 3. From another terminal, drive it via telnet
telnet localhost 4444
> source flash_iap.tcl
```

The block erase covers `0x10038000`–`0x1003FFFF` (one 32 KB block); the bottom half is the tail of the user region and is wiped as a side effect. This only matters on first-time installation on a blank chip. After install, normal user-firmware updates via the IAP do not touch the IAP region.

Useful TCL helpers from `openocd/target/ml63q2537.cfg`: `prepare_flash`, `flash_write_word`, `flash_erase_block`, `flash_program`, `test_clock`, `test_flash`.

## Field Update (sending user firmware to a running IAP device)

Once installed, user firmware is delivered over UARTF0 with any XMODEM-CRC sender:

1. Power the device. The IAP brings up UARTF0 and waits.
2. From the host, send the user-firmware `.bin` via XMODEM-CRC (e.g. `sx -X`, `lrzsz`, Tera Term).
3. The IAP programs each block as it arrives, then remaps the user region and resets.
4. Any error → IAP emits `Error` on UARTF0, aborts. Power-cycle to retry.

User firmware must be raw `.bin`, linked to base `0x10000000`, and ≤ 240 KB (`0x3C000` bytes).

## Testing

`tests_iap/` is a separate on-target binary (Cortex-M0+) built with `-DBUILD_IAP_TESTS=ON`. It exercises IAP helpers (currently XMODEM CRC and I/O). There is no host-side runner; flash `solist_ai_iap_firmware_test.hex` to the device and watch UART for pass/fail.

Note: `tests_iap/CMakeLists.txt` deliberately links against the **flat** linker layout (`ml63q25x7/Source/GCC/ML63Q25x7_gcc.ld`), not the IAP layout — the test binary is a normal ML63Q2537 ELF, not the split-region IAP image.

**Adding a suite:** create `tests_iap/test_<area>.c`, add it to `TEST_SOURCES` in `tests_iap/CMakeLists.txt`, and call its run function from `test_main.c`.

## Architecture Notes

**Watchdog** — always active with a 2 s timeout (set in `device_initialize()`). The IAP must call `wdt_clear()` at phase boundaries during long flash erase/write loops, or the chip resets. The TCL flash routines in `openocd/target/ml63q2537.cfg` interleave WDT clears for the same reason during factory programming.

**Memory budget** — Stack 1 KB, heap 3 KB (in `ml63q25x7/Source/GCC/ML63Q25x7_iap.ld`). Total RAM is 16 KB. The XMODEM receive buffer alone takes `XMODEM_BLOCK_SIZE` bytes; large static buffers eat meaningfully into the budget.

**Remap sequence** — `Remap_End()` in `src/remap_end.c` runs from RAM (its `.remap_end` section has LMA in FLASH2 and VMA in RAM). The newlib crt0 only copies `.data`, so the IAP explicitly copies `.remap_end` itself (`s_loadRemapEnd()` in `main.c`) before invoking `Remap_End()`.

## Common Modification Workflows

**Adding application source files:** drop the `.c` into `src/`, append to `APP_SOURCES` in `src/CMakeLists.txt` (note `PARENT_SCOPE`), rebuild.

**Driver headers** live in `driver/inc/`: `ssiof0.h`, `uartf0.h`, `wdt.h`, `irq.h`, `timer0_1.h`, `dmac0.h`, etc.

## Toolchain & Standard

- C99 (`-std=c99`, no GNU extensions: `CMAKE_C_EXTENSIONS OFF`).
- `arm-none-eabi-gcc` (10.3+ tested; 14.3 also works).
- CMake ≥ 3.16.
