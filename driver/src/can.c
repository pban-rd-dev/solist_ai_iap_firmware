/*****************************************************************************
 can.c

 Copyright (C) 2024 ROHM Co., Ltd.
 All rights reserved.

 This software is provided "as is" and any expressed or implied
 warranties, including, but not limited to, the implied warranties of
 merchantability and fitness for a particular purpose are disclaimed.
 ROHM shall not be liable for any direct, indirect, consequential or
 incidental damages arising from using or modifying this software.
 You (customer) can modify and use this software in whole or part on
 your own responsibility, only for the purpose of developing the software
 for use with microcontroller manufactured by ROHM.

 History
    2024.07.31 Ver 1.0.0

******************************************************************************/
/**
 * @file    can.c
 *
 * This module is 'CAN' drivers.
 *
 */
/**
 *  @addtogroup driver
 *  @{
 *
 *  @defgroup CAN
 *  @{
 *
 *  @brief CAN FD Controller register related
 *
 *  @file
 */

#include "can.h"
#include "can_reg.h"

static can_CallBackType can_add_proc_callback; /*!< Callback function storage variables */

/**
 * @brief Memory Copy
 *
 * @param[out] dst Copy to
 *
 * @param[in] src Copy from
 *
 * @param[in] n Copy Size (bytes)
 *
 * @note No error checking for speed
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - None
 *
 * @endif
 */
static void can_memcpy(void *dst, const void *src, uint32_t n)
{
    char       *cdst;
    const char *csrc;
    cdst = dst;
    csrc = src;
    while(n > 0U)
    {
        *cdst++ = *csrc++;
        n--;
    }
}

/**
 * @brief reset
 *
 * @retval E_OK normal
 *
 * @retval E_NOT_OK abnormal
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - set_reg32()
 *
 * @endif
 */
Std_ReturnType can_Reset(void)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint32_t         reset_counter = 0U;

    set_reg32(CAN0->CANCCCR, (uint32_t)CAN_CCCR_INIT);

    while(reset_counter < CAN_COUNT_LIMIT)
    {
        if((read_reg32(CAN0->CANCCCR) & CAN_CCCR_INIT) == CAN_CCCR_INIT)
        {
            ret = E_OK;
            break;
        }
        reset_counter++;
    }

    if(ret == E_OK)
    {
        set_reg32(CAN0->CANCCCR, (uint32_t)CAN_CCCR_CCE);
    }

    return ret;
}

/**
 * @brief CAN initialization
 *
 * @param[in] Param @copybrief can_InitParamType
 *
 * @retval E_OK normal
 *
 * @retval E_NOT_OK abnormal
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - [in] @ref can_add_proc_callback
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *  - set_reg32()
 *  - can_Reset()
 *  - can_SetBitTiming()
 *
 * @endif
 */
Std_ReturnType can_Init(can_InitParamType *Param)
{
    Std_ReturnType ret = E_OK;

    if(Param == NULL_PTR)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Initialization */
        /* Write permissions */
        (void)can_Reset();

        /* Operating mode setting */
        set_reg32(CAN0->CANCCCR, (uint32_t)(Param->cccr & CAN_CCCR_MASK));

        /* Bit Timing Settings */
        can_SetBitTiming(Param->dbtp, Param->nbtp, Param->tdcr);

        /* Filtering settings */
        write_reg32(CAN0->CANGFC, (uint32_t)(Param->gfc & CAN_GFC_MASK));
        write_reg32(CAN0->CANSIDFC, (uint32_t)(Param->sidfc & CAN_SIDFC_MASK));
        write_reg32(CAN0->CANXIDFC, (uint32_t)(Param->xidfc & CAN_XIDFC_MASK));
        write_reg32(CAN0->CANXIDAM, (uint32_t)(Param->xidam & CAN_XIDAM_MASK));

        /* Buffer, FIFO settings */
        write_reg32(CAN0->CANRXF0C, (uint32_t)(Param->rxf0c & CAN_RXF0C_MASK));
        write_reg32(CAN0->CANRXF1C, (uint32_t)(Param->rxf1c & CAN_RXF1C_MASK));
        write_reg32(CAN0->CANRXBC, (uint32_t)(Param->rxbc & CAN_RXBC_MASK));
        write_reg32(CAN0->CANRXESC, (uint32_t)(Param->rxesc & CAN_RXESC_MASK));
        write_reg32(CAN0->CANTXBC, (uint32_t)(Param->txbc & CAN_TXBC_MASK));
        write_reg32(CAN0->CANTXESC, (uint32_t)(Param->txesc & CAN_TXESC_MASK));
        write_reg32(CAN0->CANTXEFC, (uint32_t)(Param->txefc & CAN_TXEFC_MASK));

        /*  Interrupt settings */
        write_reg32(CAN0->CANTXBTIE, 0xFFFFFFFFU);
        write_reg32(CAN0->CANTXBCIE, 0xFFFFFFFFU);
        write_reg32(CAN0->CANIE, (uint32_t)(Param->ie & CAN_IE_MASK));
        write_reg32(CAN0->CANILS, (uint32_t)(Param->ils & CAN_ILS_MASK));
        write_reg32(CAN0->CANILE, (uint32_t)(Param->ile & CAN_ILE_MASK));

        can_add_proc_callback = *Param->cb;
    }
    return ret;
}

/**
 * @brief Initializing CAN RAM
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_InitCanRAM(void)
{
    uint32_t addr;
    for(addr = CAN_MESSAGE_RAM_ADDR; addr < (CAN_MESSAGE_RAM_ADDR + CAN_MESSAGE_RAM_SIZE); addr += 4U)
    {
        write_reg32(*((uint32_t *)addr), 0x00000000U);
    }
}

/**
 * @brief Setting Bit Timing
 *
 * @param[in] dbtp Setting value to DBTP register
 *
 * @param[in] nbtp Setting value to NBTP register
 *
 * @param[in] tdcr Setting value to TDCR register
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - write_reg32()
 *
 * @endif
 */
void can_SetBitTiming(uint32_t dbtp, uint32_t nbtp, uint32_t tdcr)
{
    write_reg32(CAN0->CANDBTP, dbtp & CAN_DBTP_MASK);
    write_reg32(CAN0->CANNBTP, nbtp & CAN_NBTP_MASK);
    write_reg32(CAN0->CANTDCR, tdcr & CAN_TDCR_MASK);
}

/**
 * @brief Standard filter settings
 *
 * @param[in] element @copybrief can_StandardFilterType
 *
 * @param[in] element_number Element Number
 *
 * @retval E_OK normal
 *
 * @retval E_NOT_OK abnormal
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
Std_ReturnType can_SetStandardFilter(can_StandardFilterType *element, uint8_t element_number)
{
    uint32_t         element_s0;
    uint32_t         adr_s0;
    Std_ReturnType Ret = E_NOT_OK;

    if(element != NULL_PTR)
    {
        element_s0 = (uint32_t)(element->sfid2 & CAN_SFILTER_SFID2_MASK);
        element_s0 |= (uint32_t)((element->ssync & CAN_SFILTER_SSYNC_MASK) << CAN_SFILTER_SSYNC);
        element_s0 |= (uint32_t)((element->sfid1 & CAN_SFILTER_SFID1_MASK) << CAN_SFILTER_SFID1);
        element_s0 |= (uint32_t)((element->sfec & CAN_SFILTER_SFEC_MASK) << CAN_SFILTER_SFEC);
        element_s0 |= (uint32_t)((element->sft & CAN_SFILTER_SFT_MASK) << CAN_SFILTER_SFT);

        adr_s0 = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_SIDFC_SADDR + (uint32_t)(element_number * 4U));
        write_reg32(*(uint32_t *)adr_s0, element_s0);

        Ret = E_OK;
    }

    return Ret;
}

/**
 * @brief Advanced filter settings
 *
 * @param[in] element @copybrief can_ExtFilterType
 *
 * @param[in] element_number Element Number
 *
 * @retval E_OK normal
 *
 * @retval E_NOT_OK abnormal
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
Std_ReturnType can_SetExtFilter(can_ExtFilterType *element, uint8_t element_number)
{
    uint32_t         element_f0;
    uint32_t         element_f1;
    uint32_t         adr_f0;
    uint32_t         adr_f1;
    Std_ReturnType Ret = E_NOT_OK;

    if(element != NULL_PTR)
    {

        element_f0 = (uint32_t)(element->element_f0.efid1 & CAN_EFILTER_EFID1_MASK);
        element_f0 |= (uint32_t)((element->element_f0.efec & CAN_EFILTER_EFEC_MASK) << CAN_EFILTER_EFEC);

        element_f1 = (uint32_t)(element->element_f1.efid2 & CAN_EFILTER_EFID2_MASK);
        element_f1 |= (uint32_t)((element->element_f1.esync & CAN_EFILTER_ESYNC_MASK) << CAN_EFILTER_ESYNC);
        element_f1 |= (uint32_t)((element->element_f1.eft & CAN_EFILTER_EFT_MASK) << CAN_EFILTER_EFT);

        adr_f0 = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_XIDFC_F0_SADDR + (uint32_t)(element_number * 8u));
        write_reg32(*(uint32_t *)adr_f0, element_f0);
        adr_f1 = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_XIDFC_F1_SADDR + (uint32_t)(element_number * 8u));
        write_reg32(*(uint32_t *)adr_f1, element_f1);
        Ret = E_OK;
    }
    return Ret;
}

/**
 * @brief Normal mode setting
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - clear_reg32()
 *
 * @endif
 */
Std_ReturnType can_SetNormalMode(void)
{
    Std_ReturnType ret            = E_NOT_OK;
    uint32_t         normal_counter = 0U;

    clear_reg32(CAN0->CANCCCR, CAN_CCCR_CCE | CAN_CCCR_INIT);
    normal_counter = 0U;
    while(normal_counter < CAN_COUNT_LIMIT)
    {
        if((read_reg32(CAN0->CANCCCR) & CAN_CCCR_INIT) == 0U)
        {
            ret = E_OK;
            break;
        }
        normal_counter++;
    }

    return ret;
}
/**
 * @brief Power-down settings
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - set_reg32()
 *
 * @endif
 */
Std_ReturnType can_SetSleepMode(void)
{
    Std_ReturnType ret           = E_NOT_OK;
    uint32_t         sleep_counter = 0U;

    set_reg32(CAN0->CANCCCR, (uint32_t)CAN_CCCR_CSR);
    sleep_counter = 0U;
    while(sleep_counter < CAN_COUNT_LIMIT)
    {
        if((read_reg32(CAN0->CANCCCR) & CAN_CCCR_CSA) == CAN_CCCR_CSA)
        {
            ret = E_OK;
            break;
        }
        sleep_counter++;
    }

    return ret;
}

/**
 * @brief CAN controller clocking request
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - clear_reg32()
 *
 * @endif
 */
void can_ClearClockStopRequest(void)
{
    clear_reg32(CAN0->CANCCCR, (uint32_t)CAN_CCCR_CSR);
}

/**
 * @brief Stop Transmission
 *
 * @param[in] element_number Element Number
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_CancelTransmit(uint8_t element_number)
{
    write_reg32(CAN0->CANTXBCR, (1U << element_number));
}

/**
 * @brief Checking the pending status of a send request in a transmitting buffer
 *
 * @return The pending element number. (bit alignment)
 *
 * @note For example, if element numbers 0 and 2 are pending, it will be 0b101 and it will be 0x05.
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_CheckPendingTransmit(void)
{
    return read_reg32(CAN0->CANTXBRP);
}
/**
 * @brief Prepare to send
 *
 * @param[in] element @copybrief can_TxBufferType
 *
 * @param[in] info @copybrief can_TxBuffer_InfoType
 *
 * @param[in] data Transmitted data
 *
 * ---
 * @par Sync/Async
 *  - Async
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *  - can_memcpy()
 *
 * @endif
 */
Std_ReturnType can_PrepareTransmit(can_TxBufferType      *element,
                                       can_TxBuffer_InfoType *info,
                                       uint8_t                      data[])
{
    Std_ReturnType ret = E_OK;

    uint32_t element_t0;
    uint32_t element_t1;
    uint32_t txbuff_size;
    uint32_t txbuff_one_element_size;
    uint32_t adr_t0;
    uint32_t adr_t1;
    uint32_t adr_wdata;

    /* Tx buffer size check +8 (number of bytes -> T0/T1) */
    if((element == NULL_PTR) || (info == NULL_PTR) || (data == NULL_PTR))
    {
        ret = E_NOT_OK;
    }
    else
    {
        txbuff_one_element_size = (info->data_size + 8u);
        txbuff_size             = (txbuff_one_element_size * info->element_num);
        if(txbuff_size > CAN_TXBUFF_SIZE)
        {
            ret = E_NOT_OK;
        }
        else
        {
            /* Writing data to Tx buffer */
            element_t0 = ((uint32_t)(element->element_t0.id & CAN_TXBUFF_ID_MASK) |
                          (uint32_t)((element->element_t0.rtr & CAN_TXBUFF_RTR_MASK) << CAN_TXBUFF_RTR) |
                          (uint32_t)((element->element_t0.xtd & CAN_TXBUFF_XTD_MASK) << CAN_TXBUFF_XTD));
            element_t1 = ((uint32_t)((element->element_t1.mm_h & CAN_TXBUFF_MM_MASK) << CAN_TXBUFF_MMH) |
                          (uint32_t)((element->element_t1.dlc & CAN_TXBUFF_DLC_MASK) << CAN_TXBUFF_DLC) |
                          (uint32_t)((element->element_t1.brs & CAN_TXBUFF_BRS_MASK) << CAN_TXBUFF_BRS) |
                          (uint32_t)((element->element_t1.fdf & CAN_TXBUFF_FDF_MASK) << CAN_TXBUFF_FDF) |
                          (uint32_t)((element->element_t1.tsce & CAN_TXBUFF_TSCE_MASK) << CAN_TXBUFF_TSCE) |
                          (uint32_t)((element->element_t1.efc & CAN_TXBUFF_EFC_MASK) << CAN_TXBUFF_EFC) |
                          (uint32_t)((element->element_t1.mm_l & CAN_TXBUFF_MM_MASK) << CAN_TXBUFF_MML));

            adr_t0 = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_TXBC_T0_SADDR + (uint32_t)(txbuff_one_element_size * info->write_element_number));
            write_reg32(*(uint32_t *)adr_t0, element_t0);
            adr_t1 = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_TXBC_T1_SADDR + (uint32_t)(txbuff_one_element_size * info->write_element_number));
            write_reg32(*(uint32_t *)adr_t1, element_t1);
            adr_wdata = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_TXBC_SDADDR + (uint32_t)(txbuff_one_element_size * info->write_element_number));
            can_memcpy((void *)adr_wdata, (void *)data, info->data_size);
        }
    }
    return ret;
}

/**
 * @brief Request to start sending
 *
 * @param[in] element_number Element Number
 *
 * ---
 * @par Sync/Async
 *  - Async
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_RequestTransmit(uint8_t element_number)
{
    write_reg32(CAN0->CANTXBAR, (1U << element_number));
}

/**
 * @brief Retrieving Received Data (Buffer)
 *
 * @param[in] element @copybrief can_RxMEMType
 *
 * @param[in] info @copybrief can_RxMEM_InfoType
 *
 * @param[out] data data
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *  - read_reg32()
 *
 * @endif
 */
void can_ReceiveBuffData(can_RxMEMType *element, can_RxMEM_InfoType *info, uint32_t data[])
{
    uint32_t rxbuff_r0data;
    uint32_t rxbuff_r1data;
    uint32_t rxbuff_one_element_size;
    uint32_t adr_r0;
    uint32_t adr_r1;
    uint32_t adr_rdata;

    if((element != NULL_PTR) && (info != NULL_PTR) && (data != NULL_PTR))
    {

        rxbuff_one_element_size = (info->data_size + 8u);

        /* Copy data from Rx buffer */

        adr_r0    = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXBC_R0_SADDR + (uint32_t)(rxbuff_one_element_size * info->read_element_number));
        adr_r1    = (CAN_MESSAGE_RAM_ADDR + CAN_OFFSET_RXBC_R1_SADDR + (rxbuff_one_element_size * info->read_element_number));
        adr_rdata = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXBC_SDADDR + (uint32_t)(rxbuff_one_element_size * info->read_element_number));

        can_memcpy((void *)data, (void *)adr_rdata, info->data_size);

        rxbuff_r0data = read_reg32(*(uint32_t *)adr_r0);
        rxbuff_r1data = read_reg32(*(uint32_t *)adr_r1);

        element->element_r0.id  = (rxbuff_r0data & CAN_RXBUFF_ID_MASK);
        element->element_r0.rtr = (uint8_t)((rxbuff_r0data & CAN_RXBUFF_RTR_MASK) >> CAN_RXBUFF_RTR);
        element->element_r0.xtd = (uint8_t)((rxbuff_r0data & CAN_RXBUFF_XTD_MASK) >> CAN_RXBUFF_XTD);
        element->element_r0.esi = (uint8_t)((rxbuff_r0data & CAN_RXBUFF_ESI_MASK) >> CAN_RXBUFF_ESI);

        element->element_r1a.rxts = (uint16_t)(rxbuff_r1data & CAN_RXBUFF_RXTS_MASK);
        element->element_r1a.dlc  = (uint8_t)((rxbuff_r1data & CAN_RXBUFF_DLC_MASK) >> CAN_RXBUFF_DLC);
        element->element_r1a.brs  = (uint8_t)((rxbuff_r1data & CAN_RXBUFF_BRS_MASK) >> CAN_RXBUFF_BRS);
        element->element_r1a.fdf  = (uint8_t)((rxbuff_r1data & CAN_RXBUFF_FDF_MASK) >> CAN_RXBUFF_FDF);
        element->element_r1a.fidx = (uint8_t)((rxbuff_r1data & CAN_RXBUFF_FIDX_MASK) >> CAN_RXBUFF_FIDX);
        element->element_r1a.anmf = (uint8_t)((rxbuff_r1data & CAN_RXBUFF_ANMF_MASK) >> CAN_RXBUFF_ANMF);

        /* Clearing Reception Status */
        if(info->read_element_number < 32U)
        {
            write_reg32(CAN0->CANNDAT1, (1U << info->read_element_number));
        }
        else
        {
            write_reg32(CAN0->CANNDAT2, (1U << (info->read_element_number - 32U)));
        }
    }
}

/**
 * @brief Retrieving Incoming Data (FIFO)
 *
 * @param[in] element @copybrief can_RxMEMType
 *
 * @param[in] info @copybrief can_RxMEM_InfoType
 *
 * @param[out] data data
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - can_memcpy()
 *
 * @endif
 */
void can_ReceiveFifoData(can_RxMEMType *element, can_RxMEM_InfoType *info, uint32_t data[])
{
    uint32_t rxfifo_data;
    uint32_t rxfifo_one_element_size;
    uint32_t adr_r0;
    uint32_t adr_r1;
    uint32_t adr_rdata;

    if((element != NULL_PTR) && (info != NULL_PTR) && (data != NULL_PTR))
    {

        rxfifo_one_element_size = (info->data_size + 8U);

        /* Copy data from Rx FIFO */
        if(info->fifo_sel == CAN_SEL_FIFO0)
        {
            adr_r0    = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF0C_R0_SADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
            adr_r1    = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF0C_R1_SADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
            adr_rdata = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF0C_SDADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
        }
        else
        {
            adr_r0    = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF1C_R0_SADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
            adr_r1    = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF1C_R1_SADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
            adr_rdata = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_RXF1C_SDADDR + (uint32_t)(rxfifo_one_element_size * info->read_element_number));
        }

        rxfifo_data = read_reg32(*(uint32_t *)adr_r0);

        element->element_r0.id  = (rxfifo_data & CAN_RXBUFF_ID_MASK);
        element->element_r0.rtr = (uint8_t)((rxfifo_data & CAN_RXBUFF_RTR_MASK) >> CAN_RXBUFF_RTR);
        element->element_r0.xtd = (uint8_t)((rxfifo_data & CAN_RXBUFF_XTD_MASK) >> CAN_RXBUFF_XTD);
        element->element_r0.esi = (uint8_t)((rxfifo_data & CAN_RXBUFF_ESI_MASK) >> CAN_RXBUFF_ESI);

        rxfifo_data = read_reg32(*(uint32_t *)adr_r1);

        element->element_r1a.rxts = (uint16_t)(rxfifo_data & CAN_RXBUFF_RXTS_MASK);
        element->element_r1a.dlc  = (uint8_t)((rxfifo_data & CAN_RXBUFF_DLC_MASK) >> CAN_RXBUFF_DLC);
        element->element_r1a.brs  = (uint8_t)((rxfifo_data & CAN_RXBUFF_BRS_MASK) >> CAN_RXBUFF_BRS);
        element->element_r1a.fdf  = (uint8_t)((rxfifo_data & CAN_RXBUFF_FDF_MASK) >> CAN_RXBUFF_FDF);
        element->element_r1a.fidx = (uint8_t)((rxfifo_data & CAN_RXBUFF_FIDX_MASK) >> CAN_RXBUFF_FIDX);
        element->element_r1a.anmf = (uint8_t)((rxfifo_data & CAN_RXBUFF_ANMF_MASK) >> CAN_RXBUFF_ANMF);

        can_memcpy((void *)data, (void *)adr_rdata, info->data_size);
    }
}

/**
 * @brief Get received element number (Buff)
 *
 * @param[in] buf_sel Buffer selection
 *
 * @return Element number received (0~63)
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_CheckReceiveBuffData(uint8_t buf_sel)
{
    uint32_t rcv_data_number = 0;
    uint32_t rcv_data_reg    = 0;
    uint8_t  bit_number;

    if(buf_sel == CAN_SEL_BUFF1)
    {
        rcv_data_reg = read_reg32(CAN0->CANNDAT1);
    }
    else
    {
        rcv_data_reg    = read_reg32(CAN0->CANNDAT2);
        rcv_data_number = 32;
    }

    if(rcv_data_reg != 0U)
    {
        for(bit_number = 0U; bit_number < 31U; bit_number++)
        {
            if((0x00000001U & (rcv_data_reg >> bit_number)) == 1U)
            {
                break;
            }
        }
        rcv_data_number += bit_number;
    }
    else
    {
        rcv_data_number = 0;
    }

    return rcv_data_number;
}

/**
 * @brief Get Received Element Number (FIFO)
 *
 * @param[in] fifo_sel FIFO selection
 *
 * @return 
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - write_reg32()
 *
 * @endif
 */
uint8_t can_CheckReceiveFifoData(uint8_t fifo_sel)
{
    uint8_t data_number;

    if(fifo_sel == CAN_SEL_FIFO0)
    {
        data_number = (uint8_t)((read_reg32(CAN0->CANRXF0S) & 0x00003F00u) >> CAN_RXF0S_F0GI);
        write_reg32(CAN0->CANRXF0A, data_number);
    }
    else
    {
        data_number = (uint8_t)((read_reg32(CAN0->CANRXF1S) & 0x00003F00u) >> CAN_RXF1S_F1GI);
        write_reg32(CAN0->CANRXF1A, data_number);
    }

    return data_number;
}

/**
 * @brief Get Transmission Event Information. 
 *
 * @param[in] element @copybrief can_TxEventType
 *
 * @param[in] element_number Element Number
 *
 * @retval E_OK Processing Complete
 *
 * @retval E_NOT_OK Incomplete
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
Std_ReturnType can_GetTxEvent(can_TxEventType *element, uint8_t element_number)
{
    Std_ReturnType ret = E_OK;

    uint32_t adr_e0;
    uint32_t adr_e1;
    uint32_t txevent_size;
    uint32_t txevent_data;
    uint32_t wmaker_en;

    /* Tx Event FiFo size check *8 (number of E0/E1->bytes) */
    txevent_size = element_number * 8U;

    if((txevent_size > CAN_TXEVENT_SIZE) || (element == NULL_PTR))
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Copying Data from Tx Event FIFO */
        adr_e0                  = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_TXEFC_SADDR + (uint32_t)(element_number * 8u));
        txevent_data            = read_reg32(*(uint32_t *)adr_e0);
        element->element_e0.id  = (txevent_data & CAN_TXEVENT_ID_MASK);
        element->element_e0.rtr = (uint8_t)((txevent_data & CAN_TXEVENT_RTR_MASK) >> CAN_TXEVENT_RTR);
        element->element_e0.xtd = (uint8_t)((txevent_data & CAN_TXEVENT_XTD_MASK) >> CAN_TXEVENT_XTD);
        element->element_e0.esi = (uint8_t)((txevent_data & CAN_TXEVENT_ESI_MASK) >> CAN_TXEVENT_ESI);

        adr_e1       = ((uint32_t)CAN_MESSAGE_RAM_ADDR + (uint32_t)CAN_OFFSET_TXEFC_E1_SADDR + (uint32_t)(element_number * 8u));
        txevent_data = read_reg32(*(uint32_t *)adr_e1);

        wmaker_en = read_reg32(CAN0->CANCCCR) & CAN_CCCR_WMM;

        if(wmaker_en == CAN_CCCR_WMM)
        {
            element->element_e1b.txtsp = (uint8_t)(txevent_data & CAN_TXEVENT_TXTSP_MASK);
            element->element_e1b.tsc   = (uint8_t)((txevent_data & CAN_TXEVENT_TSC_MASK) >> CAN_TXEVENT_TSC);
            element->element_e1b.mm_h  = (uint8_t)((txevent_data & CAN_TXEVENT_MMH_MASK) >> CAN_TXEVENT_MMH);
            element->element_e1b.dlc   = (uint8_t)((txevent_data & CAN_TXEVENT_DLC_MASK) >> CAN_TXEVENT_DLC);
            element->element_e1b.brs   = (uint8_t)((txevent_data & CAN_TXEVENT_BRS_MASK) >> CAN_TXEVENT_BRS);
            element->element_e1b.fdf   = (uint8_t)((txevent_data & CAN_TXEVENT_FDF_MASK) >> CAN_TXEVENT_FDF);
            element->element_e1b.et    = (uint8_t)((txevent_data & CAN_TXEVENT_ET_MASK) >> CAN_TXEVENT_ET);
            element->element_e1b.mm_l  = (uint8_t)((txevent_data & CAN_TXEVENT_MML_MASK) >> CAN_TXEVENT_MML);
        }
        else
        {
            element->element_e1a.txts = (uint16_t)(txevent_data & CAN_TXEVENT_TXTS_MASK);
            element->element_e1a.dlc  = (uint8_t)((txevent_data & CAN_TXEVENT_DLC_MASK) >> CAN_TXEVENT_DLC);
            element->element_e1a.brs  = (uint8_t)((txevent_data & CAN_TXEVENT_BRS_MASK) >> CAN_TXEVENT_BRS);
            element->element_e1a.fdf  = (uint8_t)((txevent_data & CAN_TXEVENT_FDF_MASK) >> CAN_TXEVENT_FDF);
            element->element_e1a.et   = (uint8_t)((txevent_data & CAN_TXEVENT_ET_MASK) >> CAN_TXEVENT_ET);
            element->element_e1a.mm_l = (uint8_t)((txevent_data & CAN_TXEVENT_MML_MASK) >> CAN_TXEVENT_MML);
        }
    }
    write_reg32(CAN0->CANTXEFA, element_number);

    return ret;
}

/**
 * @brief Interrupt enable/disable setting
 *
 * @param[in] ie Setting value to IE register
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_SetIrq(uint32_t ie)
{
    write_reg32(CAN0->CANIE, ie);
}

/**
 * @brief Test Mode Selection
 *
 * @param[in] lbck 
 *       - CAN_TEST_LBCK_ENA : Loopback mode enabled
 *       - CAN_TEST_LBCK_DIS : Loopback mode disabled
 *
 * @param[in] tx 
 *       - CAN_TEST_TX_CAN : Controlled by CAN0_TXD-terminal CAN core
 *       - CAN_TEST_TX_SAMPLE : CAN0_TXD terminals monitor sample points
 *       - CAN_TEST_TX_DOMINANT : Dominant level output at CAN0_TXD terminal
 *       - CAN_TEST_TX_RECESSIVE : Recessive level output at CAN0_TXD terminal
 *
 * @note Set it after executing can_Init(), can_Reset()
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_SetTestMode(uint32_t lbck, uint32_t tx)
{
    write_reg32(CAN0->CANTEST, (lbck << 4) | (tx << 5));
}

/**
 * @brief Get the value of the test mode register
 *
 * @param[out] *test @copybrief can_TestType
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
void can_GetTestMode(can_TestType *test)
{
    uint32_t Reg;

    if(test != NULL_PTR)
    {
        Reg         = read_reg32(CAN0->CANTEST);
        test->lbck  = (uint8_t)(((Reg & CAN_TEST_LBCK_MASK) >> 4U) & 0xFFU);
        test->tx    = (uint8_t)(((Reg & CAN_TEST_TX_MASK) >> 5U) & 0xFFU);
        test->rx    = (uint8_t)(((Reg & CAN_TEST_RX_MASK) >> 7U) & 0xFFU);
        test->txbnp = (uint8_t)(((Reg & CAN_TEST_TXBNP_MASK) >> 8U) & 0xFFU);
        test->pval  = (uint8_t)(((Reg & CAN_TEST_PVAL_MASK) >> 13U) & 0xFFU);
        test->txbns = (uint8_t)(((Reg & CAN_TEST_TXBNS_MASK) >> 16U) & 0xFFU);
        test->sval  = (uint8_t)(((Reg & CAN_TEST_SVAL_MASK) >> 21U) & 0xFFU);
    }
}

/**
 * @brief Obtaining PSR Register Values
 *
 * @return PSR register value
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_GetPSR(void)
{
    return read_reg32(CAN0->CANPSR);
}

/**
 * @brief Obtaining ECR Register Values
 *
 * @return ECR register value
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_GetECR(void)
{
    return read_reg32(CAN0->CANECR);
}

/**
 * @brief CAN interrupts
 *
 * @note Clear CANIR
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *  - write_reg32()
 *
 * @endif
 */
void Can_IRQHandler(void)
{
    uint32_t int_src;

    int_src = read_reg32(CAN0->CANIR);

    if((int_src & CAN_IR_TC) == CAN_IR_TC)
    {
        can_add_proc_callback.transmit();
        write_reg32(CAN0->CANIR, CAN_IR_TC);
    }
    else if((int_src & CAN_IR_DRX) == CAN_IR_DRX)
    {
        can_add_proc_callback.receive_buff();
        write_reg32(CAN0->CANIR, CAN_IR_DRX);
    }
    else if((int_src & CAN_IR_RF0N) == CAN_IR_RF0N)
    {
        can_add_proc_callback.receive_fifo(CAN_SEL_FIFO0);
        write_reg32(CAN0->CANIR, CAN_IR_RF0N);
    }
    else if((int_src & CAN_IR_RF1N) == CAN_IR_RF1N)
    {
        can_add_proc_callback.receive_fifo(CAN_SEL_FIFO1);
        write_reg32(CAN0->CANIR, CAN_IR_RF1N);
    }
    else if((int_src & CAN_IR_BO) == CAN_IR_BO)
    {
        can_add_proc_callback.busoff();
        write_reg32(CAN0->CANIR, CAN_IR_BO);
    }
    else
    {
        can_add_proc_callback.others();
        write_reg32(CAN0->CANIR, int_src);
    }
}

/**
 * @brief Get CAN interrupt status
 *
 * @return CANIR value
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_GetStatus(void)
{
    return read_reg32(CAN0->CANIR);
}

/**
 * @brief Clear CAN interrupt status
 *
 * @param[in] canir CANIR bit value to clear
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - write_reg32()
 *
 * @endif
 */
void can_ClearStatus(uint32_t canir)
{
    write_reg32(CAN0->CANIR, canir);
}

/**
 * @brief Get the status of outgoing event information
 *
 * @return @ref CAN_TXEFS_X
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_GetTxEventStatus(void)
{
    return read_reg32(CAN0->CANTXEFS);
}

/**
 * @brief Acquisition of status of transmitted FIFO and queue information
 *
 * @return @ref CAN_TXFQS_X
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - read_reg32()
 *
 * @endif
 */
uint32_t can_GetTxFifoQueueStatus(void)
{
    return read_reg32(CAN0->CANTXFQS);
}

/**
 * @brief CAN0INT1 interrupt
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Non Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - Can_IRQHandler()
 *
 * @endif
 */
void CAN0_INT1_IRQHandler(void)
{
    Can_IRQHandler();
}

/**
 * @brief CAN0INT0 interrupt
 *
 * ---
 * @par Sync/Async
 *  - Sync
 *
 * @par Re-Entrancy/Non Re-Entrancy
 *  - Non Re-Entrancy
 *
 * @par Static variable
 *  - None
 *
 * @ifnot RELEASE
 * @par Subroutine
 *  - Can_IRQHandler()
 *
 * @endif
 */
void CAN0_INT0_IRQHandler(void)
{
    Can_IRQHandler();
}

/** @} */
/** @} */
