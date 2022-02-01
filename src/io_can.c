/*
 * io_can.c
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */
#include "io_can.h"
#include "board.h"

CAN_HandleTypeDef hcan1;
struct can_packet canTxP, canRxP;
uint8_t can_packet_recived = FALSE;

void io_can_init(){

    // Enable clock on CAN
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 16;
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;
    if (HAL_CAN_Init(&hcan1) != HAL_OK)
    {
    Error_Handler();
    }

}

void can_pckt_send(void)
{
        // STM has 3 mailboxes on transmission
        uint8_t mailbox = 0;

        // Find free(empty) mailbox
        if ((CAN1->TSR & CAN_TSR_TME0) == CAN_TSR_TME0)
                mailbox = 0;
        else if ((CAN1->TSR & CAN_TSR_TME1) == CAN_TSR_TME1)
                mailbox = 1;
        else if ((CAN1->TSR & CAN_TSR_TME2) == CAN_TSR_TME2)
                mailbox = 2;
        else
                return;

        // Standard Id
        CAN1->sTxMailBox[mailbox].TIR = (uint32_t)canTxP.id << CAN_TI0R_STID_Pos;
        // Data length
        CAN1->sTxMailBox[mailbox].TDTR = canTxP.dlc;
        // Data
        CAN1->sTxMailBox[mailbox].TDLR = ((uint32_t)canTxP.cmd |
                        ((uint32_t)canTxP.idx << 8) |
                        ((uint32_t)canTxP.subidx << 24));
        CAN1->sTxMailBox[mailbox].TDHR = ((uint32_t)canTxP.data[0] |
                        ((uint32_t)canTxP.data[1] << 8) |
                        ((uint32_t)canTxP.data[2] << 16) |
                        ((uint32_t)canTxP.data[3] << 24));
        // Request a transmission
        CAN1->sTxMailBox[mailbox].TIR |= CAN_TI0R_TXRQ;
}


void can_pckt_receive(void)
{
    // STM has 2 FIFOs on receive
    uint8_t fifo = 0;

    // Check if a message is filtered and received by FIFO 0 or FIFO 1
    if ((CAN1->RF0R & CAN_RF0R_FMP0) != 0)
            fifo = 0;
    else if ((CAN1->RF1R & CAN_RF1R_FMP1) != 0)
            fifo = 1;
    else
            return;

    // NOTE: 2 FIFOs are used with the same parameters, so
    // "CAN_RI0R_STID_Pos" and other bit-definitions are the same for 2 FIFOs
    // Generally "CAN_RI0R_STID_Pos", "CAN_RI1R_STID_Pos" should be used
    canRxP.id = (uint8_t)(CAN1->sFIFOMailBox[fifo].RIR >> CAN_RI0R_STID_Pos);
    canRxP.dlc = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_DLC_Msk);
    canRxP.cmd = (uint8_t)CAN1->sFIFOMailBox[fifo].RDLR;
    canRxP.idx = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDLR >> 8)
            | (uint8_t)(CAN1->sFIFOMailBox[fifo].RDLR >> 16);
    canRxP.subidx = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDLR >> 24);
    canRxP.data[0] = (uint8_t)CAN1->sFIFOMailBox[fifo].RDHR;
    canRxP.data[1] = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDHR >> 8);
    canRxP.data[2] = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDHR >> 16);
    canRxP.data[3] = (uint8_t)(CAN1->sFIFOMailBox[fifo].RDHR >> 24);

    if (fifo == 0)
            CAN1->RF0R |= CAN_RF0R_RFOM0; // Release FIFO 0
    else
            CAN1->RF1R |= CAN_RF1R_RFOM1; // Release FIFO 1

    can_packet_recived = TRUE;
}

void can_send_request_state(uint16_t nodeId)
{
    // node id
    canTxP.id = nodeId;
    canTxP.dlc = 8;
    //cmd
    canTxP.cmd = 0x40;
    //  idx = 0x6041
    canTxP.idx = STATUSWORD_ID;
    // sub idx = 0
    canTxP.subidx = 0x00;
    // data
    canTxP.data[1] = 0x00;
    canTxP.data[2] = 0x00;
    canTxP.data[3] = 0x00;
    canTxP.data[4] = 0x00;

    can_pckt_send();
}

void can_send_command(uint16_t nodeId, uint32_t command)
{
    // node id
    canTxP.id = nodeId;
    canTxP.dlc = 8;
    //cmd
    canTxP.cmd = 0x23;
    //  idx = 0x6040
    canTxP.idx = CONTROLWORD_ID;
    // sub idx = 0
    canTxP.subidx = 0x00;
    // data
    canTxP.data[1] = (command >> 24);
    canTxP.data[2] = (command >> 16);
    canTxP.data[3] = (command >> 8);
    canTxP.data[4] = command;

    can_pckt_send();
}

uint8_t can_read_statusword(driver_t *drivers)
{
    if (can_packet_recived == TRUE)
    {
        uint32_t nodeId = canRxP.id-600;
        if (nodeId >=  DRIVERS_NUMBER)
            return FALSE;

        if ((canRxP.data[0] == 0x80) && *((uint32_t*)(canRxP.data)))
            return FALSE;

        drivers[nodeId].statusword = canRxP.data[7] |
                                    (canRxP.data[6] << 8) |
                                    (canRxP.data[5] << 16) |
                                    (canRxP.data[4] << 24);
        return TRUE;
    }
    return FALSE;
}


