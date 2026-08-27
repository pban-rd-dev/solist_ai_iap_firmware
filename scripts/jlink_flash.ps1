<#
.SYNOPSIS
    Program the ML63Q2537 through SEGGER J-Link (Windows counterpart of
    scripts/jlink_flash.sh).

.DESCRIPTION
    The OpenOCD path (scripts/iap_flash.py + openocd/target/ml63q2537.cfg) drives
    the flash controller from the host: every 32-bit word costs an FLASHACP /
    FLASHA / FLASHD / FLASHSTA round trip over SWD.

    J-Link instead downloads the vendor CMSIS flash algorithm
    (jlink/ML63Q25x7.FLM, from ROHM.ML63Q25x7_DFP 0.4.0) into target RAM and runs
    it there. The erase/program loops execute on the Cortex-M0+ at 48 MHz; SWD
    only carries the image data. jlink/JLinkDevices.xml wires the algorithm to the
    ML63Q2537 flash bank at 0x10000000.

    On Windows the J-Link Commander executable is JLink.exe (JLinkExe on
    Linux/macOS); everything else is identical to the shell script, including the
    JLINK_* environment overrides.

.PARAMETER BuildDir
    Directory holding iap_data.bin / iap_code.bin / iap_codeoption.bin.
    Defaults to "build".

.PARAMETER Image
    Program a single .bin/.hex/.elf instead of the three IAP bins.

.PARAMETER Address
    Load address for -Image. Required for a raw .bin, ignored otherwise.

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File scripts\jlink_flash.ps1 -Image build\solist_ai_iap_firmware_test.hex

.NOTES
    Environment overrides (same names as scripts/jlink_flash.sh):
      JLINK_EXE    JLink.exe path       (default: PATH, then the standard
                                         SEGGER install directories)
      JLINK_SPEED  SWD clock in kHz     (default: 4000)
      JLINK_SN     probe serial number  (default: first probe found)
      JLINK_VTREF  force VTref in mV, e.g. 3300, when the probe's VTref pin is
                   not wired to the target (default: auto-detect)
      JLINK_NO_RUN 1 = leave the core halted instead of reset-and-run
#>

#Requires -Version 5.1

[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [string]$BuildDir = 'build',

    [string]$Image,

    [string]$Address
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

trap {
    [Console]::Error.WriteLine("error: $($_.Exception.Message)")
    exit 1
}

$RepoRoot = Split-Path -Parent $PSScriptRoot

# Must match ml63q25x7/Source/GCC/ML63Q25x7_iap.ld and scripts/iap_flash.py.
$IapDataAddr       = '0x1003C000'
$IapCodeAddr       = '0x1003E000'
$IapCodeOptionAddr = '0x1003FFC0'
$IapRegionStart    = '0x1003C000'
$IapRegionEnd      = '0x1003FFFF'

$Device = 'ML63Q2537'

function Resolve-JLinkExe {
    if ($env:JLINK_EXE) {
        if (Test-Path -LiteralPath $env:JLINK_EXE) {
            return (Resolve-Path -LiteralPath $env:JLINK_EXE).Path
        }
        throw "JLINK_EXE is set to '$($env:JLINK_EXE)' but that file does not exist."
    }

    $onPath = Get-Command 'JLink.exe' -ErrorAction SilentlyContinue
    if ($onPath) { return $onPath.Source }

    # Standard installs, then the versioned directories the installer leaves
    # behind (JLink_V812, ...). Newest name wins.
    $roots = @($env:ProgramFiles, ${env:ProgramFiles(x86)}) | Where-Object { $_ }
    foreach ($root in $roots) {
        $fixed = Join-Path (Join-Path $root 'SEGGER') (Join-Path 'JLink' 'JLink.exe')
        if (Test-Path -LiteralPath $fixed) { return $fixed }
    }
    foreach ($root in $roots) {
        $candidate = Get-ChildItem -Path (Join-Path $root 'SEGGER') -Filter 'JLink_V*' -Directory -ErrorAction SilentlyContinue |
            Sort-Object Name -Descending |
            ForEach-Object { Join-Path $_.FullName 'JLink.exe' } |
            Where-Object { Test-Path -LiteralPath $_ } |
            Select-Object -First 1
        if ($candidate) { return $candidate }
    }

    throw "JLink.exe not found. Install the SEGGER J-Link software or set JLINK_EXE to its full path."
}

function Get-ImagePath {
    param([string]$Path, [string]$What)
    if (-not (Test-Path -LiteralPath $Path)) {
        throw "missing $Path$What"
    }
    return (Resolve-Path -LiteralPath $Path).Path
}

$jlinkExe   = Resolve-JLinkExe
$jlinkDir   = Join-Path $RepoRoot 'jlink'
$devicesXml = Join-Path $jlinkDir 'JLinkDevices.xml'
$algorithm  = Join-Path $jlinkDir 'ML63Q25x7.FLM'
foreach ($required in @($devicesXml, $algorithm)) {
    if (-not (Test-Path -LiteralPath $required)) {
        throw "missing $required"
    }
}

$speed = if ($env:JLINK_SPEED) { $env:JLINK_SPEED } else { '4000' }
$noRun = ($env:JLINK_NO_RUN -eq '1')

$lines = New-Object System.Collections.Generic.List[string]

# The DLL concatenates this path with "JLinkDevices.xml" verbatim, so the
# trailing separator is required.
$lines.Add("exec JLinkDevicesXMLPath = " + $jlinkDir + [IO.Path]::DirectorySeparatorChar)
if ($env:JLINK_SN)    { $lines.Add("usb $($env:JLINK_SN)") }
$lines.Add('si SWD')
$lines.Add("speed $speed")
if ($env:JLINK_VTREF) { $lines.Add("vtref $($env:JLINK_VTREF)") }
$lines.Add("device $Device")
$lines.Add('connect')
# Halt out of whatever the resident IAP was doing before touching flash.
$lines.Add('r')
$lines.Add('halt')

if ($Image) {
    $imagePath = Get-ImagePath -Path $Image -What ''
    if ([IO.Path]::GetExtension($imagePath) -ieq '.bin') {
        if (-not $Address) { throw "a .bin image needs an address (-Address)" }
        $lines.Add("loadfile $imagePath, $Address")
        $lines.Add("verifybin $imagePath, $Address")
    }
    else {
        $lines.Add("loadfile $imagePath")
    }
}
else {
    $hint = ' (build the IAP target first)'
    $dataBin = Get-ImagePath -Path (Join-Path $BuildDir 'iap_data.bin')       -What $hint
    $codeBin = Get-ImagePath -Path (Join-Path $BuildDir 'iap_code.bin')       -What $hint
    $optBin  = Get-ImagePath -Path (Join-Path $BuildDir 'iap_codeoption.bin') -What $hint

    # 8 x 2 KB sectors covering FLASH2 + FLASH + FLASH3. Unlike the OpenOCD
    # 32 KB block erase this leaves the user region below 0x1003C000 intact.
    $lines.Add("erase $IapRegionStart, $IapRegionEnd")
    $lines.Add("loadfile $dataBin, $IapDataAddr")
    $lines.Add("loadfile $codeBin, $IapCodeAddr")
    $lines.Add("loadfile $optBin, $IapCodeOptionAddr")
    $lines.Add("verifybin $dataBin, $IapDataAddr")
    $lines.Add("verifybin $codeBin, $IapCodeAddr")
    $lines.Add("verifybin $optBin, $IapCodeOptionAddr")
}

$lines.Add('r')
if ($noRun) { $lines.Add('halt') } else { $lines.Add('g') }
$lines.Add('exit')

$cmdFile = [IO.Path]::GetTempFileName()
try {
    # No BOM: J-Link Commander reads the script as plain text.
    [IO.File]::WriteAllText($cmdFile, ($lines -join "`r`n") + "`r`n",
                            (New-Object System.Text.UTF8Encoding $false))

    Write-Host '--- J-Link command script ---'
    $lines | ForEach-Object { Write-Host $_ }
    Write-Host '-----------------------------'

    & $jlinkExe -NoGui 1 -ExitOnError 1 -CommanderScript $cmdFile
    exit $LASTEXITCODE
}
finally {
    Remove-Item -LiteralPath $cmdFile -Force -ErrorAction SilentlyContinue
}
