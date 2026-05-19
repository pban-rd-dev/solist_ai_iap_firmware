/*****************************************************************************
 * @file    uartf_common_i.h
 * @brief   Common definitions for the interrupt-driven UARTF driver
 *
 * Ported from ROHM ML63Q2500 reference software (IAPSample).
 * Used by uartf0_i.c.
 *****************************************************************************/

#ifndef UARTF_COMMON_H__
#define UARTF_COMMON_H__

#include "mcu.h"
#include "rdwr_reg.h"

/* Enable per-byte callback in uartf0_read (XMODEM needs it). */
#define UART_BYTE_CALLBACK

/*=== UAFnMOD bit fields ===*/
#define UAFnMOD_UFnLG0      ( 0x0001 )
#define UAFnMOD_UFnLG1      ( 0x0002 )
#define UAFnMOD_UFnSTP      ( 0x0004 )
#define UAFnMOD_UFnPT0      ( 0x0008 )
#define UAFnMOD_UFnPT1      ( 0x0010 )
#define UAFnMOD_UFnPT2      ( 0x0020 )
#define UAFnMOD_UFnBC       ( 0x0040 )
#define UAFnMOD_UFnDLAB     ( 0x0080 )
#define UAFnMOD_UFnFEN      ( 0x0100 )
#define UAFnMOD_UFnRFR      ( 0x0200 )
#define UAFnMOD_UFnTFR      ( 0x0400 )
#define UAFnMOD_UFnFTL0     ( 0x1000 )
#define UAFnMOD_UFnFTL1     ( 0x2000 )

/*=== UAFnIER bit fields ===*/
#define UAFnIIR_UFnERBFI    ( 0x0001 )
#define UAFnIIR_UFnETBEI    ( 0x0002 )
#define UAFnIIR_UFnELSI     ( 0x0004 )
#define UAFnIIR_UFnTEMTI    ( 0x0040 )

/*=== UAFnIIR bit fields ===*/
#define UAFnIIR_UFnIRP      ( 0x0001 )
#define UAFnIIR_UFnIRID0    ( 0x0002 )
#define UAFnIIR_UFnIRID1    ( 0x0004 )
#define UAFnIIR_UFnIRID2    ( 0x0008 )
#define UAFnIIR_UFnFMD0     ( 0x0040 )
#define UAFnIIR_UFnFMD1     ( 0x0080 )

/*=== UAFnLSR bit fields ===*/
#define UAFnLSR_UFnDR       ( 0x0001 )
#define UAFnLSR_UFnOER      ( 0x0002 )
#define UAFnLSR_UFnPER      ( 0x0004 )
#define UAFnLSR_UFnFER      ( 0x0008 )
#define UAFnLSR_UFnBI       ( 0x0010 )
#define UAFnLSR_UFnTHRE     ( 0x0020 )
#define UAFnLSR_UFnTEMT     ( 0x0040 )
#define UAFnLSR_UFnRFE      ( 0x0080 )
#define UAFnLSR_UFnTIDL     ( 0x0100 )

/*=== UAFnCAJ bit fields ===*/
#define UAFnCAJ_UFnRMV      ( 0x0040 )

/*=== Init parameter macros ===*/
#define UARTF_LG_5BIT       (              0 )
#define UARTF_LG_6BIT       (              0 |  UAFnMOD_UFnLG0 )
#define UARTF_LG_7BIT       ( UAFnMOD_UFnLG1 |               0 )
#define UARTF_LG_8BIT       ( UAFnMOD_UFnLG1 |  UAFnMOD_UFnLG0 )
#define UARTF_STP_1BIT      (              0 )
#define UARTF_STP_2BIT      ( UAFnMOD_UFnSTP )
#define UARTF_PT_NON        (              0 )
#define UARTF_PT_ODD        (              0 |  UAFnMOD_UFnPT0 )
#define UARTF_PT_EVEN       ( UAFnMOD_UFnPT1 |  UAFnMOD_UFnPT0 )
#define UARTF_PT_FIXED_1    ( UAFnMOD_UFnPT2 |  UAFnMOD_UFnPT0 )
#define UARTF_PT_FIXED_0    ( UAFnMOD_UFnPT2 |  UAFnMOD_UFnPT1 |  UAFnMOD_UFnPT0 )
#define UARTF_BC_DIS        (              0 )
#define UARTF_BC_ENA        ( UAFnMOD_UFnBC  )
#define UARTF_DLAB_RBR_THR  (               0 )
#define UARTF_DLAB_DLR      ( UAFnMOD_UFnDLAB )
#define UARTF_FEN_DIS       (              0 )
#define UARTF_FEN_ENA       ( UAFnMOD_UFnFEN )
#define UARTF_RFR_KEEP      (              0 )
#define UARTF_RFR_CLR       ( UAFnMOD_UFnRFR )
#define UARTF_TFR_KEEP      (              0 )
#define UARTF_TFR_CLR       ( UAFnMOD_UFnTFR )
#define UARTF_FTL_1BYTE     (               0 |               0 |               0 |               0 )
#define UARTF_FTL_2BYTE     (               0 |               0 |               0 | UAFnMOD_UFnFTL0 )
#define UARTF_FTL_3BYTE     (               0 |               0 | UAFnMOD_UFnFTL1 |               0 )
#define UARTF_FTL_4BYTE     (               0 |               0 | UAFnMOD_UFnFTL1 | UAFnMOD_UFnFTL0 )

/*=== Interrupt enable bits ===*/
#define UARTF_ERBFI_DIS     (                0 )
#define UARTF_ERBFI_ENA     ( UAFnIIR_UFnERBFI )
#define UARTF_ETBEI_DIS     (                0 )
#define UARTF_ETBEI_ENA     ( UAFnIIR_UFnETBEI )
#define UARTF_ELSI_DIS      (               0 )
#define UARTF_ELSI_ENA      ( UAFnIIR_UFnELSI )
#define UARTF_TEMTI_DIS     (               0 )
#define UARTF_TEMTI_ENA     ( UAFnIIR_UFnTEMTI )

/*=== Interrupt ID values ===*/
#define UARTF_IRID_NONE         (                0 )
#define UARTF_IRID_WRITE_REQ    ( UAFnIIR_UFnIRID0 )
#define UARTF_IRID_READ_REQ     ( UAFnIIR_UFnIRID1 )
#define UARTF_IRID_CHAR_TIMEOUT ( UAFnIIR_UFnIRID2 | UAFnIIR_UFnIRID1 )
#define UARTF_IRID_DATA_ERR     ( UAFnIIR_UFnIRID1 | UAFnIIR_UFnIRID0 )
#define UARTF_IRID_TRANS_COMP   ( UAFnIIR_UFnIRID2 | UAFnIIR_UFnIRID0 )
#define UARTF_IRID_MASK         ( UAFnIIR_UFnIRID2 | UAFnIIR_UFnIRID1 | UAFnIIR_UFnIRID0 )

/*=== Clock adjustment ===*/
#define UARTF_RMV_DIS       ( UAFnCAJ_UFnRMV )
#define UARTF_RMV_ENA       (              0 )

/*=== API return values ===*/
#define UARTF_R_OK              (  0 )
#define UARTF_R_TRANS_FIN       (  1 )
#define UARTF_R_TRANS_CONT_OK   (  0 )
#ifdef UART_BYTE_CALLBACK
#define UART_R_STOP             (  1 )
#endif

typedef void (*cbfUartF_t)( uint32_t size, uint16_t errStatus );
#ifdef UART_BYTE_CALLBACK
typedef int8_t (*cbfUartFByte_t)( uint8_t recData, uint16_t errStatByte );
#endif

typedef struct {
    uint8_t         *data;
    uint32_t        size;
    uint32_t        cnt;
    cbfUartF_t      callBack;
#ifdef UART_BYTE_CALLBACK
    cbfUartFByte_t   callBackByte;
#endif
    uint16_t        errStat;
    uint8_t         blockSize;
    uint8_t         reserved;
} uartfCtrlParam_t;

#endif /* UARTF_COMMON_H__ */
