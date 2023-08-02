/*
 * TM1638.c
 *
 *  Created on: 2023/8/2
 *      Author: Syhen_XX
 */
 
 #include "TM1638.h"
 
unsigned char tab[16] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
						  0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71 };

unsigned char num[8];
 
void TM1638_Write(unsigned char	DATA)			//д���ݺ���
{
	unsigned char i;
	gpio_init(B1, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(F12, GPO, GPIO_LOW, GPO_PUSH_PULL);
	for(i=0;i<8;i++)
	{
		gpio_set_level(B1,0);
		if(DATA&0X01)
			gpio_set_level(F12,1);
		else
			gpio_set_level(F12,0);
		DATA>>=1;
		gpio_set_level(B1,1);
	}
}

unsigned char TM1638_Read(void)					//�����ݺ���
{
	unsigned char i;
	unsigned char temp=0;
	gpio_init(B1, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(F12, GPI, GPIO_HIGH, GPI_PULL_UP);
	for(i=0;i<8;i++)
	{
		temp>>=1;
		gpio_set_level(B1,0);
		if(gpio_get_level(F12))
			temp|=0x80;
		gpio_set_level(B1,1);
	}
	return temp;
}

void Write_COM(unsigned char cmd)		//����������
{
	gpio_init(A4, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_set_level(A4,0);
	TM1638_Write(cmd);
	gpio_set_level(A4,1);
}

unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	gpio_init(A4, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_set_level(A4,0);
	TM1638_Write(0x42);					//����ɨ���� ����
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	gpio_set_level(A4,1);				//4���ֽ����ݺϳ�һ���ֽ�
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}

void Write_DATA(unsigned char add,unsigned char DATA)		//ָ����ַд������
{
	gpio_init(A4, GPO, GPIO_LOW, GPO_PUSH_PULL);
	Write_COM(0x44);
	gpio_set_level(A4,0);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	gpio_set_level(A4,1);
}

void Write_allLED(unsigned char LED_flag)					//����ȫ��LED������LED_flag��ʾ����LED״̬
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				//Write_DATA(2*i+1,3);
				Write_DATA(2*i+1,1);
			else
				Write_DATA(2*i+1,0);
		}
}

void init_TM1638(void)
{
	gpio_init(A4, GPO, GPIO_LOW, GPO_PUSH_PULL);
	unsigned char i;
	Write_COM(0x8b);		//���� (0x88-0x8f)8�����ȿɵ�
	Write_COM(0x40);		//���õ�ַ�Զ���1
	gpio_set_level(A4,0);	//STB = 0
	TM1638_Write(0xc0);		//������ʼ��ַ

	for(i=0;i<16;i++)		//����16���ֽڵ�����
		TM1638_Write(0x00);
	gpio_set_level(A4,1);	//STB = 0
	for(i=0;i<8;i++)
		Write_DATA(i<<1,tab[0]);
}