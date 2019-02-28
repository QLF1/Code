#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

#include "delay.h"
#include "LED.h"
#include "pwm.h"
#include "encoder.h"
#include "PID.h"
#include "can.h"

#define MOTO_ID			0X203

#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
void RCC_Configuration(void);
void system_Config(void);
void system_Init(void);
#endif
