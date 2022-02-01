/*
 * io_uart.c
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */
#include "stdint.h"
#include <string.h>
#include <stdio.h>
#include "board.h"
#include "io_uart.h"

UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
char aTxBuffer[TXBUFFERSIZE];

/* Buffer used for reception */
char aRxBuffer[RXBUFFERSIZE];

void io_uart_init(){
    UartHandle.Instance        = USART1;

    UartHandle.Init.BaudRate   = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_ODD;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;

    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
}

uint32_t parce_command(char* cmd){

    if (!strncmp("get state", cmd, sizeof("get state")))
        return GET_STATE_CMD;
    if (!strncmp("enable", cmd, sizeof("enable")))
        return ENABLE_CMD;
    if (!strncmp("stop", cmd, sizeof("stop")))
        return STOP_CMD;
    return NOT_CMD;
}

void print(char* str)
{
    sprintf(aTxBuffer, str, TXBUFFERSIZE);
    HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE);
}

void uart_send_state(){

    for (int i = 0; i < DRIVERS_NUMBER; i++)
    {
        char message_str[14];
        sprintf(message_str, "%03d unit in state ", 14);
        print("unit in state ");

        switch (drivers[i].state){
        case STATE_NOT_READY:
            print("not ready to switch on \n");
            break;
        case STATE_SWITCH_ON_DISABLED:
            print("switch on disabled \n");
            break;
        case STATE_READY_TO_SWICHT_ON:
            print("ready to switch on \n");
            break;
        case STATE_SWITCHED_ON:
            print("switched on \n");
            break;
        case STATE_OPERATION_ENABLED:
            print("operation enabled \n");
            break;
        case STATE_QUICK_STOP_ACTIVE:
            print("quick stop active \n");
            break;
        case STATE_FAULT_REACTION_ACTIVE:
            print("fault reaction active \n");
            break;
        case STATE_FAULT:
            print("fault\n");
            break;
        default:
            print("\r internal error on processing %d unit\n");
        }
    }
}

void uart_get_command_str(char* buf, uint32_t size)
{
    sprintf(buf, aRxBuffer, RXBUFFERSIZE);
}

uint32_t get_target_id(char* buf, uint32_t size)
{
    int id = 0xff;
    sscanf(buf, "%d", &id);
    return (uint32_t)id;
}
