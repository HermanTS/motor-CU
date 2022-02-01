/*
 * motor_drivers.h
 *
 *  Created on: 30 янв. 2022 г.
 *      Author: iRU
 */

#ifndef INCLUDE_MOTOR_DRIVERS_H_
#define INCLUDE_MOTOR_DRIVERS_H_

#include "stdint.h"
#include "board.h"

void drivers_init();
void driver_change_state(driver_t *driver);

#endif /* INCLUDE_MOTOR_DRIVERS_H_ */
