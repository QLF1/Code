#include "main.h"
/*******************************PWM≈‰÷√****************************************/
void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
	GPIO_InitTypeDef         gpio;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//36M
	//PWM port
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	tim.TIM_Period=2000;
	tim.TIM_Prescaler=0;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &tim);

	oc.TIM_OCMode = TIM_OCMode_PWM1;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM3, &oc);
	TIM_OC2Init(TIM3, &oc);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

/*****************************ª•≤πPWM ‰≥ˆ****************************************/
void Motormove(int PWM)
{
	PWM=LIMIT_MAX_MIN(PWM, 2000, -2000);
	TIM_SetCompare1(TIM3, 1000 - PWM / 2);
	TIM_SetCompare2(TIM3, 1000 + PWM / 2);
}

