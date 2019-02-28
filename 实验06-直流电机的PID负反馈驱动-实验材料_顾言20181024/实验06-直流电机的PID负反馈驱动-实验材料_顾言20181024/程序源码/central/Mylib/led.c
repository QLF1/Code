#include "stm32f4xx.h"
#include "main.h"
#include "led.h"

/**
  * @brief  LEDµ∆≈‰÷√
  * @param  None
  * @retval None
  */
void LED_Configration(void)
{
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);

    LED_G_ON;
    LED_R_OFF;
}

