/*****************************************************************************
 * @file    xmodem.h
 * @brief   XMODEM-CRC state machine
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample).
 * 128-byte data blocks, CRC-16 (poly 0x1021), 10 retries, 10 s timeout.
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

#define XMODEM_DATA_SIZE    (128)
#define XMODEM_BLOCK_SIZE   (133)

void    Xmodem_Init( uint8_t *RecvBuf );
void    Xmodem_SendByte( uint8_t send_data );
uint8_t Xmodem_ReadStatus( void );
void    Xmodem_CountTimeOut( void );

#endif /* XMODEM_H__ */
