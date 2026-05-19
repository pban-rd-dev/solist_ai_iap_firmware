/*****************************************************************************
 * @file    xmodem.c
 * @brief   XMODEM-CRC state machine
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample). The only change
 * from the original is the renamed CRC header (crc.h -> xmodem_crc.h) so it
 * does not collide with driver/inc/crc.h.
 *****************************************************************************/

#include "mcu.h"
#include "irq.h"
#include "uartf0_i.h"
#include "xmodem_crc.h"
#include "xmodem.h"
#include "main.h"

static uint8_t  XmodemStatus;
static uint8_t  XmodemRetryCnt;
static uint8_t  XmodemBlockNum;
static uint8_t  XmodemSendData;
static uint16_t XmodemTimeoutCnt;
static uint16_t XmodemTimeoutRetryCnt;

static uint8_t *XmodemBuf;

#define TIMEOUT_CNT     (1280)    /* 128 Hz tick * 1280 = 10 s */
#define RETRY_CNT       (10)

static int32_t Xmodem_CheckBlockData( uint8_t *RecvData, uint8_t block_num );
static void    Xmodem_StartTimeOut( void );
static void    Xmodem_StopTimeOut( void );
static void    Xmodem_SendByteComplete( uint32_t size, uint16_t errStat );
static void    Xmodem_RecvBlockComplete( uint32_t size, uint16_t errStat );
static int8_t  Xmodem_RecvChar( uint8_t data, uint16_t errStat );
static void    Xmodem_RetryProc( void );

void Xmodem_Init( uint8_t *RecvBuf )
{
    XmodemBuf = (uint8_t *)RecvBuf;
    XmodemStatus = INIT_STATE;
    XmodemRetryCnt = 0U;
    XmodemBlockNum = 1U;
    XmodemSendData = 0U;
    XmodemTimeoutCnt = 0U;
    XmodemTimeoutRetryCnt = 0U;
}

static int32_t Xmodem_CheckBlockData( uint8_t *RecvData, uint8_t BlockNum )
{
    uint16_t crc;
    uint8_t  loopcnt;
    uint16_t data;
    uint16_t tmp;

    if ( RecvData[0] + RecvData[1] != 0xff ) {
        return -1;
    }
    crc = 0;
    for ( loopcnt = 0; loopcnt < XMODEM_DATA_SIZE; loopcnt++ ) {
        data = (uint16_t)(RecvData[loopcnt + 2]);
        crc = UpdCRC16( data, crc );
    }
    tmp = RecvData[loopcnt + 2];
    tmp = tmp << 8;
    tmp |= RecvData[loopcnt + 3];
    if ( crc != tmp ) {
        return -1;
    }
    if ( RecvData[0] != BlockNum ) {
        return -1;
    }
    return 0;
}

static void Xmodem_StartTimeOut( void )
{
    irq_tbc0_clearIRQ();
    irq_tbc0_ena();
    XmodemTimeoutCnt = 0;
}

static void Xmodem_StopTimeOut( void )
{
    irq_tbc0_dis();
}

extern uint8_t s_flag;

static void Xmodem_SendByteComplete( uint32_t Size, uint16_t ErrStat )
{
    (void)Size;
    (void)ErrStat;

    switch ( XmodemSendData ) {
    case 'C':
    case ACK:
    case NAK:
        if ( XmodemStatus != RECV_EOT ) {
            uartf0_read( XmodemBuf, XMODEM_BLOCK_SIZE, Xmodem_RecvChar, Xmodem_RecvBlockComplete );
            irq_uaf0_ena();
            Xmodem_StartTimeOut();
        } else {
            XmodemStatus = EOT_END;
        }
        break;
    case CAN:
        XmodemStatus = SEND_ERR;
        break;
    default:
        irq_uaf0_dis();
        XmodemStatus = SEND_ERR;
        Xmodem_SendByte( CAN );
        break;
    }
}

void Xmodem_SendByte( uint8_t send_data )
{
    XmodemSendData = send_data;
    if ( XmodemStatus != RECV_EOT ) {
        if ( (send_data == 'C') || (send_data == ACK) ) {
            XmodemStatus = RECV_STATE;
        }
    }
    uartf0_write( &XmodemSendData, 1, Xmodem_SendByteComplete );
    irq_uaf0_ena();
}

static void Xmodem_RecvBlockComplete( uint32_t Size, uint16_t ErrStat )
{
    (void)Size;

    Xmodem_StopTimeOut();

    if ( ErrStat != 0 ) {
        Xmodem_RetryProc();
    } else {
        if ( Xmodem_CheckBlockData( XmodemBuf + 1, XmodemBlockNum ) != 0 ) {
            XmodemRetryCnt++;
            if ( XmodemRetryCnt > RETRY_CNT ) {
                Xmodem_SendByte( CAN );
            } else {
                if ( XmodemBlockNum == 1 ) {
                    Xmodem_SendByte( 'C' );
                } else {
                    Xmodem_SendByte( NAK );
                }
            }
        } else {
            XmodemStatus = RECV_END;
            XmodemBlockNum++;
            XmodemRetryCnt = 0;
        }
    }
}

static int8_t Xmodem_RecvChar( uint8_t Data, uint16_t ErrStat )
{
    (void)Data;

    XmodemTimeoutCnt = 0;
    XmodemTimeoutRetryCnt = 0;
    if ( ErrStat != 0 ) {
        Xmodem_RetryProc();
    } else {
        switch ( XmodemBuf[0] ) {
        case EOT:
            XmodemStatus = RECV_EOT;
            Xmodem_SendByte( ACK );
            XmodemRetryCnt = 0;
            break;
        case STX:
            irq_uaf0_dis();
            Xmodem_SendByte( CAN );
            XmodemRetryCnt = 0;
            break;
        case SOH:
            XmodemRetryCnt = 0;
            break;
        default:
            Xmodem_SendByte( CAN );
            Xmodem_RetryProc();
            break;
        }
    }
    return 0;
}

uint8_t Xmodem_ReadStatus( void )
{
    return XmodemStatus;
}

void Xmodem_CountTimeOut( void )
{
    XmodemTimeoutCnt++;
    if ( XmodemTimeoutCnt > TIMEOUT_CNT ) {
        XmodemTimeoutRetryCnt++;
        irq_uaf0_dis();

        if ( XmodemTimeoutRetryCnt > RETRY_CNT ) {
            Xmodem_StopTimeOut();
            XmodemTimeoutCnt = TIMEOUT_CNT;
            XmodemStatus = TIMEOUT_ERR;
            Xmodem_SendByte( CAN );
        } else {
            Xmodem_SendByte( XmodemSendData );
        }
    }
}

static void Xmodem_RetryProc( void )
{
    irq_uaf0_dis();

    XmodemRetryCnt++;
    if ( XmodemRetryCnt > RETRY_CNT ) {
        XmodemStatus = RETRY_ERR;
        Xmodem_SendByte( CAN );
    } else {
        Xmodem_SendByte( NAK );
    }
}
