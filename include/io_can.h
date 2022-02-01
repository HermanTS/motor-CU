/*
 * io_can.h
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */

#ifndef SRC_IO_CAN_H_
#define SRC_IO_CAN_H_

#include "stdint.h"
#include "board.h"

struct can_packet {
        uint16_t id;
        uint8_t dlc;
        uint8_t cmd;
        uint16_t idx;
        uint8_t subidx;
        uint8_t data[4];
};

void io_can_init();

void can_pckt_send(void);
void can_pckt_receive(void);

void can_send_request_state(uint16_t nodeId);
void can_send_command(uint16_t nodeId, uint32_t command);
uint8_t can_read_statusword(driver_t *drivers);
#endif /* SRC_IO_CAN_H_ */

