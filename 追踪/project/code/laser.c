/*
 * laser.c
 *
 *  Created on: 2023年8月2日
 *      Author: Syhen_XX
 */
 
#include "laser.h"

void laser_init(void)
{
	gpio_init(A3, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

void laser_on(void)
{
	gpio_set_level(A3,1);
}

void laser_off(void)
{
	gpio_set_level(A3,0);
}