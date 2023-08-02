/*
 * TM1638.h
 *
 *  Created on: 2023/8/2
 *      Author: Syhen_XX
 */
 
#ifndef CODE_TM1638_H_
#define CODE_TM1638_H_

#include "zf_driver_gpio.h"




void TM1638_Write(unsigned char	DATA);						//д���ݺ���
unsigned char TM1638_Read(void);							//�����ݺ���
void Write_COM(unsigned char cmd);							//����������
unsigned char Read_key(void);								//��key
void Write_DATA(unsigned char add,unsigned char DATA);		//ָ����ַд������
void Write_allLED(unsigned char LED_flag);					//����ȫ��LED������LED_flag��ʾ����LED״̬
void init_TM1638(void);

#endif /* CODE_TM1638_H_ */