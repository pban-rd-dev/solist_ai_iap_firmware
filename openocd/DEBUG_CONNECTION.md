# J-Link SWD Connection Debugging

## Problem
Getting "SWD ack not OK: 7 JUNK" - J-Link sees power but no valid SWD communication.

## Required Connections (Minimum)

J-Link Pin → ML63Q2537 Pin:
```
VTref (Pin 1)  → 3.3V (target power)
SWDIO (Pin 7)  → SWDIO (check datasheet - usually P3.0 or similar)
SWCLK (Pin 9)  → SWCLK (check datasheet - usually P3.1 or similar)
GND   (Pin 4/6/etc) → GND
```

**Optional but helpful:**
```
RESET (Pin 15) → NRST/RESET
```

## Diagnostic Steps

### 1. Verify J-Link can see the target
```bash
JLinkExe
# In the console:
ShowEmuList
VTref
# Should show ~3.3V (confirmed working - you have 3.301V)
```

### 2. Check if SWDIO/SWCLK are swapped
The most common cause of JUNK responses is **swapped SWDIO and SWCLK pins**.

Try swapping these two wires and test again.

### 3. Try J-Link Commander directly
```bash
JLinkExe
```

At the J-Link> prompt:
```
device Cortex-M0
si SWD
speed 100
connect
```

If this works, the problem is OpenOCD config.
If this also fails with errors, it's a hardware issue.

### 4. Check ML63Q2537 Datasheet

Verify the SWD pins:
- Find SWDIO pin number
- Find SWCLK pin number
- Check if SWD needs to be enabled (some chips disable debug by default)

### 5. Check for SWD Pin Conflicts

The ML63Q2537 might have:
- **SWD disabled by fuse/option byte**
- **SWD pins configured as GPIO** by running firmware
- **Debug protection enabled**

### 6. Try Mass Erase / Unlock

If previous firmware disabled SWD, you may need an unlock sequence.

For ROHM chips, check if there's a "Connect Under Reset" requirement.

## Next Steps if Still Failing

1. **Measure with multimeter/oscilloscope:**
   - Check continuity between J-Link SWDIO pin and MCU SWDIO pin
   - Check continuity between J-Link SWCLK pin and MCU SWCLK pin
   - Verify 3.3V on VTref and MCU VDD

2. **Check pull-ups:**
   - SWDIO should have a pull-up resistor (10kΩ to VDD) on the target board
   - If missing, add 10kΩ resistor between SWDIO and 3.3V

3. **Try different J-Link speeds:**
   Edit `openocd/interface/jlink.cfg` and try:
   - `adapter speed 50` (very slow)
   - `adapter speed 10` (extremely slow)

4. **Contact ROHM support:**
   ML63Q2537 may require special unlock procedure
