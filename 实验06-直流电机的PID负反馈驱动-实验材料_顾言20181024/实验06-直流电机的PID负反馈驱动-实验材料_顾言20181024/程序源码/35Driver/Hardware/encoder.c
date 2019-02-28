#include "main.h"
int EncoderCircle = 0;
/*******************************±àÂëÆ÷ÅäÖÃ****************************************/
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_Initure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Initure.GPIO_Mode = GPIO_Mode_IN_FLOATING;;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initure);
	
	nvic.NVIC_IRQChannel = TIM4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI2, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	
	TIM_Cmd(TIM4, ENABLE);
}
/**************************±àÂëÆ÷ÖÐ¶Ï£¬»ñÈ¡È¦Êý*********************************/
void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == ENABLE)
	{
		if(!((TIM4->CR1 &(TIM_CR1_DIR)) == (TIM_CR1_DIR)))
			EncoderCircle++;
		else
			EncoderCircle--;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
