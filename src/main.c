/*
 * main.c
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */
#include "board.h"
#include "io_uart.h"
#include "io_can.h"

int main(int argc, char **argv) {

    HAL_Init();
    io_can_init();
    io_uart_init();

}
