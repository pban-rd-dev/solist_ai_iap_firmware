/*****************************************************************************
 * @file    xmodem.h
 * @brief   XMODEM-CRC / XMODEM-1K state machine (dual-size)
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample). Accepts both
 * 1024-byte (STX, XMODEM-1K) and 128-byte (SOH, XMODEM-CRC) blocks from the
 * sender, deciding per block based on the header byte. This is necessary
 * because `sx --1k` ends a transfer with one or more 128-byte SOH blocks
 * when the file size is not a multiple of 1024 — rejecting SOH causes the
 * final block to fail with "incomplete".
 *
 * The receive buffer is always sized for the worst case (1029 bytes). The
 * per-byte callback latches the actual block size from the header and tells
 * the UART driver to stop early when an SOH block ends at byte 133.
 * CRC-16 (poly 0x1021), 10 retries, 10 s timeout.
 *****************************************************************************/

#ifndef XMODEM_H__
#define XMODEM_H__

#define INIT_STATE          (0)
#define RECV_STATE          (1)
#define RECV_END            (2)
#define RECV_EOT            (3)
#define EOT_END             (4)
#define TIMEOUT_ERR         (5)
#define RETRY_ERR           (6)
#define SEND_ERR            (7)

#define SOH                 (0x01)
#define STX                 (0x02)
#define EOT                 (0x04)
#define ACK                 (0x06)
#define NAK                 (0x15)
#define CAN                 (0x18)

#define XMODEM_DATA_SIZE    (1024)
#define XMODEM_BLOCK_SIZE   (XMODEM_DATA_SIZE + 5)   /* hdr + blk# + ~blk# + data + CRC16 */

void     Xmodem_Init( uint8_t *RecvBuf );
void     Xmodem_SendByte( uint8_t send_data );
uint8_t  Xmodem_ReadStatus( void );
void     Xmodem_CountTimeOut( void );

/* Data-bytes (128 or 1024) of the last accepted block. Read by main after
 * RECV_END so write_verify knows how many bytes to flush to flash. */
uint16_t Xmodem_GetLastDataSize( void );

#endif /* XMODEM_H__ */
