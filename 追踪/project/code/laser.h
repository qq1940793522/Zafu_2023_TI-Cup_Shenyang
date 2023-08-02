/*
 * laser.h
 *
 *  Created on: 2023年8月2日
 *      Author: Syhen_XX
 */
 
#ifndef CODE_LASER_H_
#define CODE_LASER_H_

#include "zf_driver_gpio.h"

void laser_init(void);
void laser_on(void);
void laser_off(void);

#endif /* CODE_LASER_H_ */