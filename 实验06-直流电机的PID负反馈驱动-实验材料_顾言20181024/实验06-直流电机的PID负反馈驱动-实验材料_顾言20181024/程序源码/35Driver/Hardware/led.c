#include "main.h"

//LED_GREEN----PB0, '0' is ON,'1' is OFF
//LED_BLUE------PB1, '0' is ON,'1' is OFF

void LED_Configuration(void)
{
	GPIO_InitTypeDef gpio;   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	LED_GREEN_OFF();
	LED_BLUE_OFF();
}
