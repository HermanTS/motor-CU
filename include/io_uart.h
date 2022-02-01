/*
 * io_uart.h
 *
 *  Created on: 26 янв. 2022 г.
 *      Author: iRU
 */

#ifndef SRC_IO_UART_H_
#define SRC_IO_UART_H_

#define TXBUFFERSIZE 128
#define RXBUFFERSIZE 128

void io_uart_init();
uint32_t parce_command(char* cmd);
void print(char* str);
void uart_send_state();
void uart_get_command_str(char* buf, uint32_t size);
uint32_t get_target_id(char* buf, uint32_t size);

#endif /* SRC_IO_UART_H_ */
