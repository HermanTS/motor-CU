/*
 * main.c
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */
#include "board.h"
#include "io_uart.h"
#include "io_can.h"
#include "motor_drivers.h"

extern driver_t drivers[DRIVERS_NUMBER];

void main() {

    HAL_Init();
    SystemInit();
    SystemCoreClockUpdate();
    io_can_init();
    io_uart_init();
    //Первично настроить состояние драйверов

    drivers_init(drivers);

    //Отправить по uart сообщение о готовности
    print("All motor drivers ready to work");

    //переодическая проверка буффера
    char command_str[32];

    while(1){

        uart_get_command_str(command_str, 32);

        uint8_t command_name = parce_command(command_str);
        uint8_t target_id;
        if (command_name == ENABLE_CMD || command_name == STOP_CMD)
        {
            target_id = get_target_id(command_str, 32);
            if (drivers[target_id].state == BUSY)
                print("Requested driver is bysy\n");
            else
                drivers[target_id].command = command_name;
        } else if (command_name == GET_STATE_CMD)
            uart_send_state();


        for (uint32_t i = 0; i < DRIVERS_NUMBER; i++)
            driver_change_state(&drivers[i]);

        for (uint32_t i = 0; i < DRIVERS_NUMBER; i++)
            can_send_request_state(drivers[i].can_id);

        can_read_statusword(drivers);
    }
}
