#!/usr/bin/env python3
"""
Convert Intel HEX file directly to OpenOCD flash commands
This avoids the gap-filling issue with binary conversion
"""

import sys
import os

def parse_hex_line(line):
    """Parse a single Intel HEX line"""
    if not line.startswith(':'):
        return None
    
    line = line.strip()
    if len(line) < 11:
        return None
    
    # Intel HEX format: :LLAAAATT[DD...]CC
    # LL = byte count, AAAA = address, TT = type, DD = data, CC = checksum
    byte_count = int(line[1:3], 16)
    address = int(line[3:7], 16)
    record_type = int(line[7:9], 16)
    
    if record_type == 0x00:  # Data record
        data = line[9:9 + byte_count * 2]
        return {'address': address, 'data': data, 'type': 'data'}
    elif record_type == 0x01:  # End of file
        return {'type': 'eof'}
    elif record_type == 0x04:  # Extended linear address
        high_addr = int(line[9:13], 16)
        return {'type': 'extended_addr', 'high_addr': high_addr}
    elif record_type == 0x05:  # Start linear address
        return {'type': 'start_addr'}
    
    return None

def hex_to_flash_commands(hex_file, base_addr=0x10000000):
    """Convert HEX file to flash write commands"""
    commands = []
    high_addr = 0
    
    with open(hex_file, 'r') as f:
        for line in f:
            record = parse_hex_line(line)
            if not record:
                continue
            
            if record['type'] == 'extended_addr':
                high_addr = record['high_addr'] << 16
            elif record['type'] == 'data':
                addr = high_addr + record['address']
                data = record['data']
                
                # Process data in 4-byte words
                for i in range(0, len(data), 8):  # 8 hex chars = 4 bytes
                    if i + 8 <= len(data):
                        word_data = data[i:i+8]
                        # Keep bytes in original order (HEX file already has correct byte order)
                        # HEX stores as: byte0 byte1 byte2 byte3
                        # We need to write as 32-bit word: 0xbyte3byte2byte1byte0
                        byte0 = word_data[0:2]
                        byte1 = word_data[2:4]
                        byte2 = word_data[4:6]
                        byte3 = word_data[6:8]
                        word = f"0x{byte3}{byte2}{byte1}{byte0}"
                        
                        word_addr = addr + i // 2
                        commands.append((word_addr, word))
            elif record['type'] == 'eof':
                break
    
    return commands

def generate_tcl_script(commands, output_file):
    """Generate TCL script with flash commands"""
    with open(output_file, 'w') as f:
        f.write("# Auto-generated flash programming commands from HEX file\n\n")
        f.write("proc program_hex_data {} {\n")
        f.write("    echo \"Programming HEX data to flash...\"\n\n")
        
        # Add initialization
        f.write("    # Initialize system\n")
        f.write("    set_clock_48mhz\n")
        f.write("    wdt_init\n")
        f.write("    wdt_clear\n\n")
        
        f.write("    # Initialize flash\n")
        f.write("    flash_init\n")
        f.write("    flash_open\n")
        f.write("    flash_self_program 0xA5A5A5A5\n\n")
        
        f.write("    # Erase flash\n")
        f.write("    flash_erase_block 0x10000000 0x40000\n")
        f.write("    wdt_clear\n\n")
        
        f.write("    # Re-open after erase\n")
        f.write("    flash_close\n")
        f.write("    flash_open\n")
        f.write("    flash_self_program 0xA5A5A5A5\n\n")
        
        f.write("    # Write data\n")
        count = 0
        for addr, data in commands:
            f.write(f"    flash_write_word 0x{addr:08X} {data}\n")
            count += 1
            
            # Add WDT clear every 256 words
            if count % 256 == 0:
                f.write("    wdt_clear\n")
                f.write(f"    echo \"Progress: {count} words written\"\n")
        
        f.write("\n    # Close flash\n")
        f.write("    flash_self_program 0x5A5A5A5A\n")
        f.write("    flash_close\n")
        f.write("    wdt_clear\n\n")
        
        f.write("    echo \"Programming complete!\"\n")
        f.write("}\n\n")
        f.write("# Execute\n")
        f.write("program_hex_data\n")
    
    return count

def main():
    if len(sys.argv) < 2:
        print("Usage: hex_to_flash.py <hex_file> [output.tcl]")
        print("")
        print("Converts Intel HEX file directly to OpenOCD flash commands")
        print("This avoids gap-filling issues from binary conversion")
        sys.exit(1)
    
    hex_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "flash_from_hex.tcl"
    
    if not os.path.exists(hex_file):
        print(f"Error: {hex_file} not found")
        sys.exit(1)
    
    print(f"Converting {hex_file} to flash commands...")
    
    # Parse HEX file
    commands = hex_to_flash_commands(hex_file)
    
    # Calculate statistics
    addresses = [cmd[0] for cmd in commands]
    if addresses:
        min_addr = min(addresses)
        max_addr = max(addresses)
        
        print(f"  Address range: 0x{min_addr:08X} - 0x{max_addr:08X}")
        print(f"  Total words: {len(commands)}")
        print(f"  Total bytes: {len(commands) * 4}")
    
    # Generate TCL script
    count = generate_tcl_script(commands, output_file)
    
    print(f"\nGenerated {output_file} with {count} flash_write_word commands")
    print("\nNote: This only writes addresses that have data in the HEX file.")
    print("Gaps in the HEX file will remain as erased flash (0xFFFFFFFF).")

if __name__ == "__main__":
    main()