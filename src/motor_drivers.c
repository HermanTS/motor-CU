/*
 * motor_drivers.c
 *
 *  Created on: 30 янв. 2022 г.
 *      Author: iRU
 */
#include "motor_drivers.h"
#include "board.h"
#include "io_can.h"

driver_t drivers[DRIVERS_NUMBER];

void drivers_init()
{
    //заполнить адреса драйверов
    for (uint32_t i = 0; i < DRIVERS_NUMBER; i++)
    {
        drivers[i].can_id = 0x600+i;
        drivers[i].statusword = STATE_UNNOWN;
    }

    uint8_t drivers_is_ready = FALSE;
    while (drivers_is_ready != TRUE)
    {
        //Опросить драйвера о их текущем состоянии
        for (uint32_t i = 0; i < DRIVERS_NUMBER; i++)
        {
            can_send_request_state(drivers[i].can_id);
            drivers_is_ready = (drivers[i].statusword != STATE_UNNOWN) ? TRUE : FALSE;
        }
    }
}

void driver_change_state(driver_t *driver)
{

    switch (driver->command)
    {
    case STOP_CMD:
        switch (driver->statusword)
        {
        case STATE_SWITCH_ON_DISABLED:
            driver->controlword = COMMAND_SHUTDOWN;
            break;
        case STATE_SWITCHED_ON:
            driver->controlword = COMMAND_DISABLE_VOLTAGE;
            break;
        case STATE_OPERATION_ENABLED:
            driver->controlword = COMMAND_SHUTDOWN;
            break;
        case STATE_FAULT:
            driver->controlword = COMMAND_FAUILT_RESET;
            break;
        case STATE_READY_TO_SWICHT_ON:
            driver->state = IDDLE;
            driver->command = NOT_CMD;
            return;
        default:
            return;
        }
        break;
    case ENABLE_CMD:
        switch (driver->statusword)
        {
        case STATE_SWITCH_ON_DISABLED:
            driver->controlword = COMMAND_SHUTDOWN;
            break;
        case STATE_READY_TO_SWICHT_ON:
            driver->controlword = COMMAND_SWITCH_ON;
            break;
        case STATE_SWITCHED_ON:
            driver->controlword = COMMAND_ENABLE_OPERATION;
            break;
        case STATE_OPERATION_ENABLED:
            driver->state = IDDLE;
            driver->command = NOT_CMD;
            return;
        case STATE_FAULT:
            driver->controlword = COMMAND_FAUILT_RESET;
            break;
        default:
            return;
        }
        break;
    default:
        return;
    }

    driver->state = BUSY;
    can_send_command(driver->can_id, driver->controlword);
    return;
}
