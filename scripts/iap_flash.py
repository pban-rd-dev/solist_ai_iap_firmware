#!/usr/bin/env python3
"""
iap_flash.py -- generate OpenOCD TCL to flash the three IAP bins to their
target addresses on ML63Q2537.

The IAP build produces three raw binaries that live at non-contiguous flash
addresses:

    iap_code.bin        -> 0x1003E000  (FLASH:  .text + ARM.exidx + tables)
    iap_data.bin        -> 0x1003C000  (FLASH2: .iap2 + .remap_end LMA + .data LMA)
    iap_codeoption.bin  -> 0x1003FFC0  (FLASH3: .codeoption)

The IAP region sits inside the top 32 KB flash block (0x10038000–0x1003FFFF),
so the block erase wipes a small slice of user-firmware area below 0x1003C000
as a side effect. That is intentional: first-time IAP installation assumes a
blank chip, and after install the IAP region is not re-erased on normal use.

This script does NOT modify master's scripts/hex_to_flash.py. It is specific
to feature/IAP_impl.

Usage:
    python3 scripts/iap_flash.py <build_dir> [output.tcl]

Default output: flash_iap.tcl
"""

import os
import struct
import sys

# Flash addresses (must match ML63Q25x7_iap.ld and openocd/openocd.cfg).
IAP_CODE_ADDR        = 0x1003E000
IAP_DATA_ADDR        = 0x1003C000
IAP_CODEOPTION_ADDR  = 0x1003FFC0

# Erase: 32 KB block that contains the IAP region.
ERASE_BLOCK_ADDR     = 0x10038000
ERASE_BLOCK_SIZE     = 0x00008000


def read_bin_as_words(path):
    """Return a list of (addr, word) entries for a raw binary at base address.

    The caller supplies the base separately. We just return the words.
    Pads the final word with 0xFF (erased-flash byte) if the bin is not
    aligned to 4 bytes.
    """
    with open(path, "rb") as f:
        data = f.read()

    if len(data) % 4 != 0:
        data += b"\xFF" * (4 - (len(data) % 4))

    return [struct.unpack_from("<I", data, i)[0] for i in range(0, len(data), 4)]


def emit_writes(out, base_addr, words, label):
    out.write(f"    # ----- {label}: {len(words)} words @ 0x{base_addr:08X} -----\n")
    addr = base_addr
    for i, word in enumerate(words):
        out.write(f"    flash_write_word 0x{addr:08X} 0x{word:08X}\n")
        addr += 4
        # WDT clear + progress every 256 words (1 KB) to mirror prepare_flash.
        if (i + 1) % 256 == 0:
            out.write("    wdt_clear\n")
            out.write(f'    echo "  {label}: {i + 1} / {len(words)} words"\n')
    # Always wdt_clear at the end of a phase. Without this, the last
    # (len(words) % 256) words of one phase plus the first ~256 words of the
    # next phase run with no WDT clear in between — long enough to risk a
    # 2 s timeout reset during the iap_data -> iap_code transition.
    out.write("    wdt_clear\n")
    out.write("\n")


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    build_dir = sys.argv[1]
    out_path = sys.argv[2] if len(sys.argv) > 2 else "flash_iap.tcl"

    code_bin       = os.path.join(build_dir, "iap_code.bin")
    data_bin       = os.path.join(build_dir, "iap_data.bin")
    codeoption_bin = os.path.join(build_dir, "iap_codeoption.bin")

    for p in (code_bin, data_bin, codeoption_bin):
        if not os.path.exists(p):
            print(f"error: missing {p}", file=sys.stderr)
            print("       (build the IAP target first: cmake .. && make)", file=sys.stderr)
            sys.exit(1)

    code_words       = read_bin_as_words(code_bin)
    data_words       = read_bin_as_words(data_bin)
    codeoption_words = read_bin_as_words(codeoption_bin)

    with open(out_path, "w") as out:
        out.write("# Auto-generated flash programming commands for the IAP image\n\n")

        out.write("proc program_iap {} {\n")
        out.write('    echo "Programming IAP image to flash..."\n\n')

        # Order matches master's scripts/hex_to_flash.py (proven workflow):
        # clock first, then WDT, then clear.
        out.write("    # Initialize system\n")
        out.write("    set_clock_48mhz\n")
        out.write("    wdt_init\n")
        out.write("    wdt_clear\n\n")

        out.write("    # Initialize flash\n")
        out.write("    flash_init\n")
        out.write("    flash_open\n")
        out.write("    flash_self_program 0xA5A5A5A5\n\n")

        out.write(f"    # Erase the 32 KB block containing the IAP region (0x{ERASE_BLOCK_ADDR:08X}-0x{ERASE_BLOCK_ADDR + ERASE_BLOCK_SIZE - 1:08X}).\n")
        out.write(f"    flash_erase_block 0x{ERASE_BLOCK_ADDR:08X} 0x{ERASE_BLOCK_SIZE:08X}\n")
        out.write("    wdt_clear\n\n")

        out.write("    # Re-open after erase\n")
        out.write("    flash_close\n")
        out.write("    flash_open\n")
        out.write("    flash_self_program 0xA5A5A5A5\n\n")

        emit_writes(out, IAP_DATA_ADDR,       data_words,       "iap_data       FLASH2")
        emit_writes(out, IAP_CODE_ADDR,       code_words,       "iap_code       FLASH")
        emit_writes(out, IAP_CODEOPTION_ADDR, codeoption_words, "iap_codeoption FLASH3")

        out.write("    # Close flash\n")
        out.write("    flash_self_program 0x5A5A5A5A\n")
        out.write("    flash_close\n")
        out.write("    wdt_clear\n\n")

        out.write('    echo "IAP programming complete. Reset the chip to boot it."\n')
        out.write("}\n\n")
        out.write("# Execute\n")
        out.write("program_iap\n")

    total_words = len(code_words) + len(data_words) + len(codeoption_words)
    print(f"wrote {out_path}")
    print(f"  iap_data       : {len(data_words):4d} words @ 0x{IAP_DATA_ADDR:08X}")
    print(f"  iap_code       : {len(code_words):4d} words @ 0x{IAP_CODE_ADDR:08X}")
    print(f"  iap_codeoption : {len(codeoption_words):4d} words @ 0x{IAP_CODEOPTION_ADDR:08X}")
    print(f"  total          : {total_words} words ({total_words * 4} bytes)")


if __name__ == "__main__":
    main()
