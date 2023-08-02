/*
 * TM1638.h
 *
 *  Created on: 2023/8/2
 *      Author: Syhen_XX
 */
 
#ifndef CODE_TM1638_H_
#define CODE_TM1638_H_

#include "zf_driver_gpio.h"




void TM1638_Write(unsigned char	DATA);						//写数据函数
unsigned char TM1638_Read(void);							//读数据函数
void Write_COM(unsigned char cmd);							//发送命令字
unsigned char Read_key(void);								//读key
void Write_DATA(unsigned char add,unsigned char DATA);		//指定地址写入数据
void Write_allLED(unsigned char LED_flag);					//控制全部LED函数，LED_flag表示各个LED状态
void init_TM1638(void);

#endif /* CODE_TM1638_H_ */