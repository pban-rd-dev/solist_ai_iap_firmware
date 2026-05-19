/*****************************************************************************
 * @file    xmodem_crc.h
 * @brief   Software CRC-16 (poly 0x1021, MSB-first) for XMODEM-CRC
 *
 * Renamed from ROHM ML63Q2500 reference software's crc.h to avoid colliding
 * with driver/inc/crc.h (which is the HW CRC peripheral interface).
 *****************************************************************************/

#ifndef XMODEM_CRC_H__
#define XMODEM_CRC_H__

unsigned short UpdCRC16(unsigned short cp, unsigned short crc);

#endif /* XMODEM_CRC_H__ */
