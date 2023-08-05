/*
 * JY60.c
 *
 *  Created on: 2023/8/3
 *      Author: Syhen_XX
 */
 
#include "JY901S.h"
#include "zf_driver_uart.h"
 
uint8 jy_head[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
uint8 jy_end[5] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
 
void jy901s_init(void)
{
	uart_init(JY901S_UART_INDEX, JY901S_UART_BAUDRATE, JY901S_UART_TX_PIN, JY901S_UART_RX_PIN);
}
