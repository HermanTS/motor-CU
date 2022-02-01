/*
 * board.h
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include "stdint.h"

#ifndef DRIVERS_NUMBER
#define DRIVERS_NUMBER      4
#endif

//controlworld
#define CONTROLWORD_ID              0x6040
#define COMMAND_SHUTDOWN            0x06
#define COMMAND_SWITCH_ON           0x07
#define COMMAND_DISABLE_VOLTAGE     0x00
#define COMMAND_QUICK_STOP          0x02
#define COMMAND_DISABLE_OPERATION   0x07
#define COMMAND_ENABLE_OPERATION    0x0f
#define COMMAND_FAUILT_RESET        0x80

//statusworld
#define STATUSWORD_ID               0x6041
#define STATE_NOT_READY             0x00
#define STATE_SWITCH_ON_DISABLED    0x40
#define STATE_READY_TO_SWICHT_ON    0x21
#define STATE_SWITCHED_ON           0x23
#define STATE_OPERATION_ENABLED     0x27
#define STATE_QUICK_STOP_ACTIVE     0x07
#define STATE_FAULT_REACTION_ACTIVE 0x0f
#define STATE_FAULT                 0x08
#define STATE_UNNOWN                0xff

#define TRUE                        1
#define FALSE                       !TRUE

#define BUSY                        TRUE
#define IDDLE                       !BUSY

enum {
    GET_STATE_CMD,
    ENABLE_CMD,
    STOP_CMD,
    NOT_CMD
};


typedef struct {
    uint8_t  state;
    uint8_t  command;
    uint16_t can_id;
    uint32_t statusword;
    uint32_t controlword;

} driver_t;

extern driver_t drivers[DRIVERS_NUMBER];

static inline void Error_Handler()
{
    while(1);
}

/* Definition for USARTx clock resources */
#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()
/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6
/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "io_can.h"
#include "io_uart.h"

#endif /* INCLUDE_BOARD_H_ */
