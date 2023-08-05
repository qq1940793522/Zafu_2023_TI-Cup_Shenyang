/*********************************************************************************************************************
* MM32F527X-E9P Opensourec Library 即（MM32F527X-E9P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F527X-E9P 开源库的一部分
* 
* MM32F527X-E9P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MM32F527X_E9P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "math.h"
#include "zf_common_headfile.h"
#include "laser.h"
#include "TM1638.h"
#include "JY901S.h"

uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

uint8 get_data = 0;                                                             // 接收数据变量
uint32 fifo_data_count = 0;                                                     // fifo 数据个数

fifo_struct uart_data_fifo;														// fifo

uint32 servo_x = 755;															//舵机X PWM
uint32 servo_y = 755;															//舵机Y PWM

unsigned int i,j;																//

float step_x;
float step_y;

uint8 point_num = 0;															//点位

float point_pwm_x[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
float point_pwm_y[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

float point_roll[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
float point_yaw[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

float point_angle[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,								//屏幕八点角度
						  0, 0, 0, 0, 0, 0, 0, 0, 0,	
						  0, 0, 0, 0, 0, 0, 0, 0, 0,
						  0, 0, 0, 0, 0, 0, 0, 0, 0 };

float red_x[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
float red_y[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

float black_x[4] = { 0, 0, 0, 0 };
float black_y[4] = { 0, 0, 0, 0 };

float square[2] = {0, 0};														//屏幕中心

float angle[3];
unsigned int angle_num;

int main (void)
{
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                                                               // 初始化默认 Debug UART
	//外设初始化
	laser_init();																//初始化红色激光
	init_TM1638();																//初始化TM1638模块
	jy901s_init();																//初始化JY901S
	uart_init(UART_5, 115200, UART5_TX_C12, UART5_RX_D2);
	//暂停按键初始化
	gpio_init(A10, GPI, GPIO_HIGH, GPI_PULL_UP);
	exti_init(A10, EXTI_TRIGGER_FALLING);
	//屏幕初始化
	ips200_init(IPS200_TYPE_SPI);
	ips200_set_dir(IPS200_PORTAIT);
	ips200_set_font(IPS200_8X16_FONT);
	ips200_set_color(RGB565_BLACK, RGB565_WHITE);
	
	ips200_show_string(60, 16*0, "Servo");
	ips200_show_string(55, 16*1, "X");
	ips200_show_string(95, 16*1, "Y");
	ips200_show_string(155, 16*0, "Angle");
	ips200_show_string(130, 16*1, "Roll");
	ips200_show_string(195, 16*1, "Yaw");
	ips200_show_string(8, 16*2, "1:");
	ips200_show_string(8, 16*3, "2:");
	ips200_show_string(8, 16*4, "3:");
	ips200_show_string(8, 16*5, "4:");
	ips200_show_string(8, 16*6, "5:");
	ips200_show_string(8, 16*7, "6:");
	ips200_show_string(8, 16*8, "7:");
	ips200_show_string(8, 16*9, "8:");
	ips200_show_string(8, 16*10, "9:");
	ips200_show_string(8, 16*11, "10:");
	ips200_show_string(8, 16*12, "11:");
	ips200_show_string(8, 16*13, "12:");
	ips200_show_string(8, 16*14, "MID:");
	ips200_show_string(8, 16*15, "NOW:");
	ips200_show_string(8, 16*16, "POINT:");
	ips200_show_string(8*2, 16*17, "Pitch");
	ips200_show_string(8*12, 16*17, "Roll");
	ips200_show_string(8*22, 16*17, "Yaw");
	
	pwm_init(TIM1_PWM_CH1_A8, 50, servo_x);
	pwm_init(TIM1_PWM_CH2_A9, 50, servo_y);		                       
	
	fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区
	uart_rx_interrupt(JY901S_UART_INDEX, 1);                            		// 开启 UART_INDEX 的接收中断
	interrupt_set_priority(JY901S_UART_PRIORITY, 1);                          	// 设置对应 UART_INDEX 的中断优先级为 1
	uart_rx_interrupt(UART_5, 1);                            					// 开启 UART_INDEX 的接收中断
	interrupt_set_priority(UART5_IRQn, 0);                          			// 设置对应 UART_INDEX 的中断优先级为 0
	
	laser_on();																
	exti_enable(A10);
	
    while(1)
    {
		i=Read_key();                          
		if(i<8)
		{
			while(Read_key()==i);		       
			printf("%d\r\n",i);
			system_delay_ms(10);
			switch(i){
				//上
				case 0 :
					servo_y -= 2;
				break;
				//下
				case 1 :
					servo_y += 2;
				break;
				//左
				case 2 :
					servo_x += 2;
				break;
				//右
				case 3 :
					servo_x -= 2;
				break;
				//记录点位
				case 4 :
					point_pwm_x[point_num] = servo_x;
					point_pwm_y[point_num] = servo_y;
					point_roll[point_num] = angle[1];
					point_yaw[point_num] = angle[2];
					red_x[point_num] = tan(point_yaw[point_num]*PI/180);
					red_y[point_num] = tan(point_roll[point_num]*PI/180);
					ips200_show_uint(45, 16 * (2 + point_num), point_pwm_x[point_num], 4);
					ips200_show_uint(85, 16 * (2 + point_num), point_pwm_y[point_num], 4);
					ips200_show_float(120, 16 * (2 + point_num), point_roll[point_num], 3, 2);
					ips200_show_float(180, 16 * (2 + point_num), point_yaw[point_num], 3, 2);
					point_num++;
					if(point_num>11)
						point_num = 0;
				break;
				//基础(1)
				case 5 :
					square[0] =  (point_pwm_x[0] + point_pwm_x[1] + point_pwm_x[2] + point_pwm_x[3]
								+ point_pwm_x[4] + point_pwm_x[5] + point_pwm_x[6] + point_pwm_x[7])/8;
					square[1] =  (point_pwm_y[0] + point_pwm_y[1] + point_pwm_y[2] + point_pwm_y[3]
								+ point_pwm_y[4] + point_pwm_y[5] + point_pwm_y[6] + point_pwm_y[7])/8;
					servo_x = square[0];
					servo_y = square[1];
					ips200_show_uint(55, 16*14, square[0], 4);
					ips200_show_uint(95, 16*14, square[1], 4);
					pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
					pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
				break;
				//基础(2)
				case 6 :
					for(i = 0;i < 7;i++)
					{	
						step_x = (point_pwm_x[i+1] - point_pwm_x[i])/10;
						step_y = (point_pwm_y[i+1] - point_pwm_y[i])/10;
						servo_x = point_pwm_x[i];
						servo_y = point_pwm_y[i];
						pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
						pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
						system_delay_ms(100);
						for(j = 0;j < 10;j++)
						{
							servo_x += step_x;
							servo_y += step_y;
							pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
							pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
							ips200_show_uint(55, 16*15, servo_x, 4);
							ips200_show_uint(95, 16*15, servo_y, 4);
							system_delay_ms(100);
						}
					}
					step_x = (point_pwm_x[0] - point_pwm_x[7])/10;
					step_y = (point_pwm_y[0] - point_pwm_y[7])/10;
					servo_x = point_pwm_x[7];
					servo_y = point_pwm_y[7];
					pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
					pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
					system_delay_ms(100);
					for(j = 0;j < 10;j++)
					{
						servo_x += step_x;
						servo_y += step_y;
						pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
						pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
						ips200_show_uint(55, 16*15, servo_x, 4);
						ips200_show_uint(95, 16*15, servo_y, 4);
						system_delay_ms(100);
					}
				break;
				//基础(3)(4)
				case 7 :
					for(i = 8;i < 11;i++)
					{	
						step_x = (point_pwm_x[i+1] - point_pwm_x[i])/10;
						step_y = (point_pwm_y[i+1] - point_pwm_y[i])/10;
						servo_x = point_pwm_x[i];
						servo_y = point_pwm_y[i];
						pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
						pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
						system_delay_ms(100);
						for(j = 0;j < 10;j++)
						{
							servo_x += step_x;
							servo_y += step_y;
							pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
							pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
							ips200_show_uint(55, 16*15, servo_x, 4);
							ips200_show_uint(95, 16*15, servo_y, 4);
							system_delay_ms(100);
						}
					}
					step_x = (point_pwm_x[8] - point_pwm_x[11])/10;
					step_y = (point_pwm_y[8] - point_pwm_y[11])/10;
					servo_x = point_pwm_x[11];
					servo_y = point_pwm_y[11];
					pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
					pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
					system_delay_ms(100);
					for(j = 0;j < 10;j++)
					{
						servo_x += step_x;
						servo_y += step_y;
						pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
						pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
						ips200_show_uint(55, 16*15, servo_x, 4);
						ips200_show_uint(95, 16*15, servo_y, 4);
						system_delay_ms(100);
					}
				break;
			}
			pwm_set_duty(TIM1_PWM_CH1_A8, servo_x);
			pwm_set_duty(TIM1_PWM_CH2_A9, servo_y);
		}
		ips200_show_uint(55, 16*15, servo_x, 4);
		ips200_show_uint(95, 16*15, servo_y, 4);
		
		ips200_show_uint(65, 16*16, point_num + 1, 2);
		
		ips200_show_float(8*2, 16*18, angle[0], 3, 3);
		ips200_show_float(8*10, 16*18, angle[1], 3, 3);
		ips200_show_float(8*20, 16*18, angle[2], 3, 3);
		
		system_delay_ms(10);
    }
}

void uart_rx_interrupt_handler (void)
{ 
    uart_query_byte(JY901S_UART_INDEX, &get_data);                                					// 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
	if(get_data == 0x55 && angle_num == 0)
		angle_num = 1;
	if(angle_num)
	{
		fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           					// 将数据写入 fifo 中
		fifo_data_count = fifo_used(&uart_data_fifo);                          							// 查看 fifo 是否有数据
		if(fifo_data_count == 11)                                                						// 读取到数据了
		{
			fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
			angle[0] = (float)((short)((short)fifo_get_data[3]<<8|fifo_get_data[2]))/32786*180;
			angle[1] = (float)((short)((short)fifo_get_data[5]<<8|fifo_get_data[4]))/32786*180;
			angle[2] = (float)((short)((short)fifo_get_data[7]<<8|fifo_get_data[6]))/32786*180;
			angle_num = 0;
//			printf("%f,%f,%f\r\n", angle[0], angle[1], angle[2]);
//			uart_write_buffer(DEBUG_UART_INDEX, fifo_get_data, fifo_data_count);      					// 将读取到的数据发送出去
		}
	}
}

void uart5_rx_interrupt_handler (void)
{ 
    uart_query_byte(UART_5, &get_data);                                					// 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE

	fifo_write_buffer(&uart_data_fifo, &get_data, 1);                           					// 将数据写入 fifo 中
	fifo_data_count = fifo_used(&uart_data_fifo);                          							// 查看 fifo 是否有数据
	
		if(j == 4)                                                						// 读取到数据了
		{	
			if(fifo_data_count == 5) 
			{
				fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
				red_x[point_num] = (fifo_get_data[0] - 48) * 10 + fifo_get_data[1] - 48;
				red_y[point_num] = (fifo_get_data[3] - 48) * 10 + fifo_get_data[4] - 48;
	//			printf("%f,%f\r\n",red_x[point_num],red_y[point_num]);
				uart_write_buffer(DEBUG_UART_INDEX, fifo_get_data, fifo_data_count);      					// 将读取到的数据发送出去
			}
		}
		else if(j == 7)
		{
			{
				if(fifo_data_count == 10)
				{
					fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
//					printf("%f,%f\r\n",red_x[point_num],red_y[point_num]);
					uart_write_buffer(DEBUG_UART_INDEX, fifo_get_data, fifo_data_count);      					// 将读取到的数据发送出去
				}
			}
		}
	
	
}