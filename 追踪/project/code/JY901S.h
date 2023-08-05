/*
 * laser.h
 *
 *  Created on: 2023/8/3
 *      Author: Syhen_XX
 */
 
#ifndef CODE_JY60_H_
#define CODE_JY60_H_

#include "zf_common_typedef.h"

#define JY901S_UART_INDEX            ( UART_3 )                        
#define JY901S_UART_BAUDRATE         ( 115200 )                               
#define JY901S_UART_TX_PIN           ( UART3_TX_D8 )                         
#define JY901S_UART_RX_PIN           ( UART3_RX_D9 )
#define JY901S_UART_PRIORITY         ( UART3_IRQn )

void jy901s_init(void);

#endif /* CODE_JY60_H_ */