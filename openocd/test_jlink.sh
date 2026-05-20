#!/bin/bash
# Test J-Link connection to ML63Q2537

echo "======================================"
echo "J-Link SWD Connection Test"
echo "======================================"
echo ""

# Test 1: Check if J-Link is detected
echo "Test 1: Checking if J-Link is connected..."
JLinkExe -CommanderScript /dev/stdin << EOF
exit
EOF

if [ $? -ne 0 ]; then
    echo "ERROR: J-Link not found!"
    echo "Check USB connection."
    exit 1
fi

echo "✓ J-Link detected"
echo ""

# Test 2: Check target power
echo "Test 2: Checking target power..."
JLinkExe -CommanderScript /dev/stdin << EOF
VTref
exit
EOF

echo ""
echo "If VTref shows ~3.3V, power is OK."
echo "If VTref shows 0V, check your target power supply."
echo ""

# Test 3: Try to connect with different settings
echo "Test 3: Attempting SWD connection..."
echo ""
echo "Trying Cortex-M0 @ 100 kHz..."

JLinkExe -CommanderScript /dev/stdin << EOF
device Cortex-M0
si SWD
speed 100
connect
exit
EOF

echo ""
echo "======================================"
echo "Connection Test Complete"
echo "======================================"
echo ""
echo "If connection failed, check:"
echo "1. SWDIO and SWCLK pins (might be swapped)"
echo "2. Pull-up resistor on SWDIO (10kΩ recommended)"
echo "3. GND connection"
echo "4. See DEBUG_CONNECTION.md for detailed troubleshooting"
