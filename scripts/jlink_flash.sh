#!/usr/bin/env bash
#
# jlink_flash.sh -- program the ML63Q2537 through SEGGER J-Link.
#
# The OpenOCD path (scripts/iap_flash.py + openocd/target/ml63q2537.cfg) drives
# the flash controller from the host: every 32-bit word costs an FLASHACP /
# FLASHA / FLASHD / FLASHSTA round trip over SWD, so a 4 KB image takes minutes.
#
# J-Link instead downloads the vendor CMSIS flash algorithm
# (jlink/ML63Q25x7.FLM, from ROHM.ML63Q25x7_DFP 0.4.0) into target RAM and runs
# it there. The erase/program loops execute on the Cortex-M0+ at 48 MHz; SWD
# only carries the image data. jlink/JLinkDevices.xml wires the algorithm to the
# ML63Q2537 flash bank at 0x10000000.
#
# Usage:
#   scripts/jlink_flash.sh [<build_dir>]        # program the three IAP bins
#   scripts/jlink_flash.sh --file <img> [<addr>]
#                                               # program one .bin/.hex/.elf
#                                               # (<addr> required for .bin)
#
# Environment:
#   JLINK_EXE    JLinkExe binary          (default: JLinkExe from PATH, else
#                                          /opt/SEGGER/JLink/JLinkExe)
#   JLINK_SPEED  SWD clock in kHz         (default: 4000)
#   JLINK_SN     probe serial number      (default: first probe found)
#   JLINK_VTREF  force VTref in mV, e.g. 3300, when the probe's VTref pin is
#                not wired to the target  (default: auto-detect)
#   JLINK_NO_RUN 1 = leave the core halted instead of reset-and-run

set -euo pipefail

REPO_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)

# Must match ml63q25x7/Source/GCC/ML63Q25x7_iap.ld and scripts/iap_flash.py.
IAP_DATA_ADDR=0x1003C000
IAP_CODE_ADDR=0x1003E000
IAP_CODEOPTION_ADDR=0x1003FFC0
IAP_REGION_START=0x1003C000
IAP_REGION_END=0x1003FFFF

JLINK_EXE=${JLINK_EXE:-}
if [ -z "$JLINK_EXE" ]; then
    if command -v JLinkExe >/dev/null 2>&1; then
        JLINK_EXE=JLinkExe
    elif [ -x /opt/SEGGER/JLink/JLinkExe ]; then
        JLINK_EXE=/opt/SEGGER/JLink/JLinkExe
    else
        JLINK_EXE=JLinkExe
    fi
fi
JLINK_SPEED=${JLINK_SPEED:-4000}
DEVICE=ML63Q2537

usage() {
    cat <<'USAGE'
Usage:
  scripts/jlink_flash.sh [<build_dir>]         program the three IAP bins
                                               (default build_dir: build)
  scripts/jlink_flash.sh --file <img> [<addr>] program one .bin/.hex/.elf
                                               (<addr> required for .bin)

Environment:
  JLINK_EXE    JLinkExe binary          (default: JLinkExe from PATH, else
                                         /opt/SEGGER/JLink/JLinkExe)
  JLINK_SPEED  SWD clock in kHz         (default: 4000)
  JLINK_SN     probe serial number      (default: first probe found)
  JLINK_VTREF  force VTref in mV, e.g. 3300, when the probe's VTref pin is
               not wired to the target  (default: auto-detect)
  JLINK_NO_RUN 1 = leave the core halted instead of reset-and-run
USAGE
    exit "${1:-1}"
}

mode=iap
build_dir=
image=
image_addr=

case "${1:-}" in
    -h|--help)  usage 0 ;;
    --file)
        mode=file
        image=${2:-}
        image_addr=${3:-}
        [ -n "$image" ] || { echo "error: --file needs an image path" >&2; exit 1; }
        ;;
    -*)         echo "error: unknown option $1" >&2; usage ;;
    *)          build_dir=${1:-build} ;;
esac

if ! command -v "$JLINK_EXE" >/dev/null 2>&1; then
    echo "error: '$JLINK_EXE' not found." >&2
    echo "       Install the SEGGER J-Link software or set JLINK_EXE=/path/to/JLinkExe." >&2
    exit 1
fi

[ -f "$REPO_ROOT/jlink/JLinkDevices.xml" ] || {
    echo "error: missing $REPO_ROOT/jlink/JLinkDevices.xml" >&2
    exit 1
}
[ -f "$REPO_ROOT/jlink/ML63Q25x7.FLM" ] || {
    echo "error: missing $REPO_ROOT/jlink/ML63Q25x7.FLM (flash algorithm)" >&2
    exit 1
}

cmdfile=$(mktemp)
trap 'rm -f "$cmdfile"' EXIT

{
    # The DLL concatenates this path with "JLinkDevices.xml" verbatim, so the
    # trailing slash is required.
    echo "exec JLinkDevicesXMLPath = $REPO_ROOT/jlink/"
    if [ -n "${JLINK_SN:-}" ]; then
        echo "usb ${JLINK_SN}"
    fi
    echo "si SWD"
    echo "speed $JLINK_SPEED"
    if [ -n "${JLINK_VTREF:-}" ]; then
        echo "vtref ${JLINK_VTREF}"
    fi
    echo "device $DEVICE"
    echo "connect"
    # Halt out of whatever the resident IAP was doing before touching flash.
    echo "r"
    echo "halt"

    if [ "$mode" = iap ]; then
        data_bin=$build_dir/iap_data.bin
        code_bin=$build_dir/iap_code.bin
        opt_bin=$build_dir/iap_codeoption.bin
        for p in "$data_bin" "$code_bin" "$opt_bin"; do
            [ -f "$p" ] || { echo "error: missing $p (build the IAP target first)" >&2; exit 1; }
        done

        # 8 x 2 KB sectors covering FLASH2 + FLASH + FLASH3. Unlike the OpenOCD
        # 32 KB block erase this leaves the user region below 0x1003C000 intact.
        echo "erase $IAP_REGION_START, $IAP_REGION_END"
        echo "loadfile $data_bin, $IAP_DATA_ADDR"
        echo "loadfile $code_bin, $IAP_CODE_ADDR"
        echo "loadfile $opt_bin, $IAP_CODEOPTION_ADDR"
        echo "verifybin $data_bin, $IAP_DATA_ADDR"
        echo "verifybin $code_bin, $IAP_CODE_ADDR"
        echo "verifybin $opt_bin, $IAP_CODEOPTION_ADDR"
    else
        [ -f "$image" ] || { echo "error: missing $image" >&2; exit 1; }
        case "$image" in
            *.bin)
                [ -n "$image_addr" ] || { echo "error: a .bin image needs an address" >&2; exit 1; }
                echo "loadfile $image, $image_addr"
                echo "verifybin $image, $image_addr"
                ;;
            *)
                echo "loadfile $image"
                ;;
        esac
    fi

    if [ "${JLINK_NO_RUN:-0}" = 1 ]; then
        echo "r"
        echo "halt"
    else
        echo "r"
        echo "g"
    fi
    echo "exit"
} > "$cmdfile"

echo "--- J-Link command script ---"
cat "$cmdfile"
echo "-----------------------------"

"$JLINK_EXE" -NoGui 1 -ExitOnError 1 -CommanderScript "$cmdfile" < /dev/null
