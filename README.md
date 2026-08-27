# Solist-AI IAP Firmware (ML63Q2537)

In-Application Programming (IAP) firmware for ML63Q2537-based devices. It occupies the top 32 KB of internal flash, accepts a user-firmware image over UART (XMODEM-CRC), programs it into the user-firmware region, then remaps and resets to boot the new image.

End-user applications are linked separately and delivered through the IAP update flow described below.

## What this firmware does

On reset the IAP:

1. Configures the system clock to 48 MHz PLL and starts the watchdog (2 s timeout).
2. Brings up UARTF0 on **P32 (RX) / P33 (TX) @ 115200 8N1**.
3. Waits for an XMODEM-CRC transfer of a user-firmware image.
4. Programs the received image into the user-firmware area (`0x10000000`–`0x1003BFFF`) word-by-word, clearing the watchdog at phase boundaries.
5. Executes the flash-remap sequence from RAM and triggers a reset so the new user firmware boots on the next cycle.

Errors during reception or programming emit an `Error` string on UARTF0 and abort the transfer; the device remains in the IAP and can be retried.

## Hardware

- **MCU:** ML63Q2537 (ROHM/Lapis "Solist-AI"), ARM Cortex-M0+ @ 48 MHz (PLL)
- **Flash:** 256 KB at `0x10000000`
- **RAM:** 16 KB at `0x20000000` (linker reserves 1 KB stack, 3 KB heap)
- **Update transport:** UARTF0 on P32/P33, 115200 8N1 (caps at 115200 — hard MCU limit)

## Flash memory map

| Region   | Range                       | Contents                                                       |
| -------- | --------------------------- | -------------------------------------------------------------- |
| User     | `0x10000000` – `0x1003BFFF` | End-user application (programmed via XMODEM by the IAP)        |
| FLASH2   | `0x1003C000` – `0x1003DFFF` | IAP `.iap2` + `.remap_end` LMA + `.data` LMA (`iap_data.bin`)  |
| FLASH    | `0x1003E000` – `0x1003FFBF` | IAP `.text` + ARM.exidx + tables (`iap_code.bin`)              |
| FLASH3   | `0x1003FFC0` – `0x1003FFFF` | Code option (`iap_codeoption.bin`)                             |

User-firmware images delivered through the IAP must therefore be linked to start at `0x10000000` and fit within the user region.

## Prerequisites

- `arm-none-eabi-gcc` 10.3+ (tested with 14.3)
- CMake ≥ 3.16
- Ninja — required on Windows, optional elsewhere (see Build)
- SEGGER J-Link software and a J-Link probe — the factory flash step
  (`JLinkExe` on Linux/macOS, `JLink.exe` on Windows)
- Python 3 + OpenOCD — only for the legacy OpenOCD flash path

## Setup (one-time)

CMSIS is tracked as a git submodule:

```bash
git submodule update --init --recursive
```

## Build

### Linux / macOS

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..   # Release: -Os -DNDEBUG  (use Debug for -O0 -g3)
make -j4
```

### Windows

Use **Ninja**. CMake defaults to the Visual Studio generator on Windows, and that
generator cannot drive `arm-none-eabi-gcc` — it emits `.vcxproj` files that build
for MSVC/x86, which is not this project. Select the generator explicitly:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
cmake --build build --target bin       # optional whole-image .bin
```

A `build/` directory that was already configured with another generator has to be
deleted first — CMake cannot switch generators in place. `-G "MinGW Makefiles"`
with `mingw32-make` is an untested alternative; Ninja is the verified path.

Both must be on PATH in the shell you configure from:

- Arm GNU Toolchain — `arm-none-eabi-gcc --version` must succeed
- Ninja — ships with Visual Studio 2019+, or `winget install Ninja-build.Ninja`

The generated binaries are byte-identical to a Unix Makefiles build.

Add `-DBUILD_IAP_TESTS=ON` to also build the on-target test binary; it lands in
`build/tests_iap/`.

Keep the checkout in a path without spaces if you can. The build itself handles
them, but the OpenOCD TCL and J-Link Commander scripts both take unquoted file
paths.

Outputs in `build/`:

- `solist_ai_iap_firmware` — ELF
- `solist_ai_iap_firmware.hex` — full Intel HEX (for inspection)
- `iap_code.bin` — section image for `0x1003E000`
- `iap_data.bin` — section image for `0x1003C000`
- `iap_codeoption.bin` — section image for `0x1003FFC0`

Run `make bin` (or `cmake --build build --target bin`, which works with any
generator) to additionally produce `solist_ai_iap_firmware.bin` — a whole-image raw binary covering the IAP load region (`0x1003C000`–`0x1003FFFF`, 16 KB) with 0xFF padding. Useful for inspection or single-file flash tools; not used by `scripts/iap_flash.py`.

The whole-image `.hex` is **not** what gets programmed in production — the IAP is split across three non-contiguous flash regions, so the three `.bin` images are flashed at their distinct base addresses via the script below.

## Factory installation (programming the IAP itself)

Use this on a blank or to-be-reprovisioned device, before the unit ships.

### With J-Link (recommended)

Linux / macOS:

```bash
scripts/jlink_flash.sh                 # programs build/{iap_data,iap_code,iap_codeoption}.bin
scripts/jlink_flash.sh <build_dir>     # explicit build directory
```

Windows (PowerShell 5.1 or later):

```powershell
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 <build_dir>
```

Both emit the same J-Link Commander script and honour the same `JLINK_*`
environment variables; only the Commander binary differs (`JLinkExe` on
Linux/macOS, `JLink.exe` on Windows).

The script drives `JLinkExe` with the device definition in `jlink/JLinkDevices.xml`,
which binds the ML63Q2537 flash bank at `0x10000000` to the vendor CMSIS flash
algorithm `jlink/ML63Q25x7.FLM` (taken verbatim from ROHM.ML63Q25x7_DFP 0.4.0).
J-Link downloads that algorithm into target RAM and runs it on the Cortex-M0+:
it raises the clock to the 48 MHz PLL, drives FLASHA/FLASHD/FLASHCON/FLASHACP,
and clears the watchdog while polling FLASHSTA. SWD only carries the image data
instead of one round trip per programmed 32-bit word.

Erase granularity is the algorithm's 2 KB sector, so only `0x1003C000`–`0x1003FFFF`
is erased and the user-firmware region below it stays intact.

Environment overrides: `JLINK_EXE` (path to `JLinkExe`), `JLINK_SPEED` (SWD kHz,
default 4000), `JLINK_SN` (probe serial), `JLINK_VTREF` (force VTref in mV when
the probe's VTref pin is not wired), `JLINK_NO_RUN=1` (leave the core halted).

Measured on this repo's 4360-byte IAP image (J-Link Lite-Cortex-M V9, SWD
4000 kHz): the full script — connect, erase, program all three bins, read-back
verify, reset and run — takes **2.8 s**, against **10.7 s** for the OpenOCD path.
Most of the J-Link time is fixed connect/prepare overhead; its measured
program+verify rate is 23–32 KB/s, whereas the OpenOCD path costs one host round
trip per programmed 32-bit word, so the gap widens with image size.

Any other image can go through the same path:

```bash
scripts/jlink_flash.sh --file build/tests_iap/solist_ai_iap_firmware_test.hex
scripts/jlink_flash.sh --file some_image.bin 0x10000000
```

```powershell
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 -Image build\tests_iap\solist_ai_iap_firmware_test.hex
powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 -Image some_image.bin -Address 0x10000000
```

### With OpenOCD (legacy fallback)

```bash
# 1. Generate a TCL script that erases the top 32 KB block and writes the three IAP bins
python3 scripts/iap_flash.py build/ flash_iap.tcl

# 2. Start OpenOCD in one terminal
openocd -f openocd/openocd.cfg

# 3. From another terminal, drive it via telnet
telnet localhost 4444
> source flash_iap.tcl
```

This path programs one 32-bit word per SWD transaction sequence and is
correspondingly slow. Its erase covers `0x10038000`–`0x1003FFFF` (one 32 KB block).
The bottom of that block (`0x10038000`–`0x1003BFFF`) is the tail of the
user-firmware region and is wiped as a side effect — intentional, and only
relevant on first-time IAP installation when the chip is expected to be blank.
After install, normal user-firmware updates via the IAP do not touch the IAP region.

## Field update (sending a user firmware image to a running device)

Once the IAP is installed, end-user firmware is delivered over UARTF0 with any XMODEM-CRC sender:

1. Power the device. The IAP starts and waits on UARTF0 (P32 RX / P33 TX, 115200 8N1).
2. From the host, send the user-firmware `.bin` using an XMODEM-CRC client (e.g. `sx -X`, `lrzsz`, Tera Term's XMODEM/CRC, or any tool that speaks XMODEM-CRC with SOH/STX block sizes).
3. The IAP programs each block as it arrives. On success the device remaps the user region into the boot view and resets; the user firmware then runs.
4. On any error the IAP emits `Error` on UARTF0 and aborts; power-cycle and retry.

User firmware images must be raw `.bin`, linked to base `0x10000000`, and no larger than the user region (`0x3C000` bytes = 240 KB).

## Repo layout

```
src/                IAP application (built into solist_ai_iap_firmware ELF)
  main.c              IAP entry — receive XMODEM, program user flash, remap, reset
  xmodem.[ch]         XMODEM-CRC receiver (dual SOH/STX block size)
  xmodem_crc.[ch]     CRC-16-CCITT for XMODEM
  uartf0_i.[ch]       UARTF0 driver instance used by the IAP
  remap_end.c         Flash-remap + reset sequence (executed from RAM)
  codeoption.c        Code-option section contents
  debug_log.[ch]      In-RAM event log, read over SWD
  device.[ch]         Clock + tick + WDT init
driver/             ML63Q2537 peripheral drivers (static lib `driver`)
utility/board/      Board helpers (static lib `utility`)
external/CMSIS/     ARM CMSIS_6 submodule (REQUIRED)
ml63q25x7/Source/   Startup, system init, GCC linker scripts
  GCC/ML63Q25x7_iap.ld   IAP linker script (FLASH/FLASH2/FLASH3/REMAP layout)
jlink/              J-Link device definition + vendor CMSIS flash algorithm
  JLinkDevices.xml       ML63Q2537 flash bank bound to ML63Q25x7.FLM
  ML63Q25x7.FLM          Flash algorithm, verbatim from ROHM.ML63Q25x7_DFP 0.4.0
docs/
  iap_reference.md    End-user guide: entering IAP mode and sending firmware (Japanese)
openocd/            OpenOCD config split into interface/ + target/ + top-level cfg
scripts/
  jlink_flash.sh      Program the IAP (or any image) via JLinkExe — recommended
  jlink_flash.ps1     Same flow for Windows / JLink.exe (PowerShell 5.1+)
  iap_flash.py        Generate TCL to flash the three IAP bins at their addresses
  hex_to_flash.py     Generate TCL from a full .hex (used for non-IAP builds)
tests_iap/          On-target test binary (build with -DBUILD_IAP_TESTS=ON)
```

## Tests

`tests_iap/` builds a separate on-target binary that exercises IAP helpers (XMODEM CRC, I/O). It runs on the device itself — there is no host-side runner. Build with `-DBUILD_IAP_TESTS=ON`.

## License

Copyright (c) 2026 株式会社ピーバンドットコム. All rights reserved. Published for
reference; see [LICENSE](LICENSE) for the terms and the exact list of paths they
cover.

This repository is not all first-party. `driver/`, `utility/` and `ml63q25x7/`
are ROHM Co., Ltd. code, redistributed here with ROHM's permission, and
`jlink/ML63Q25x7.FLM` comes verbatim from ROHM's device family pack. Four files
under `ml63q25x7/` derive from Arm CMSIS and are Apache-2.0. `external/CMSIS` is
a submodule rather than vendored source. Each component keeps its own terms —
see [NOTICE](NOTICE).

