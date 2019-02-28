#include "main.h"

int delay_tim;
/*******************************1ms延时****************************************/
void TIM2Init(void)   
{
    TIM_TimeBaseInitTypeDef tim2;
    NVIC_InitTypeDef NVIC_TIM;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    tim2.TIM_ClockDivision = TIM_CKD_DIV1;   
    tim2.TIM_Prescaler = 7199;                   
    tim2.TIM_CounterMode = TIM_CounterMode_Up;
    tim2.TIM_RepetitionCounter = 0;
    tim2.TIM_Period = 9; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_TIM.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_TIM.NVIC_IRQChannelCmd = ENABLE;
	NVIC_TIM.NVIC_IRQChannelPreemptionPriority = 2;     //此处设定中断优先级比数据计算中断低，保证计时准确。通常设定延时函数中断优先级最高，可以用于任何位置
	NVIC_TIM.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_TIM);
    
    TIM_TimeBaseInit(TIM2, &tim2);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM2, DISABLE);

}

/*******************************1ms中断****************************************/
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        delay_tim--;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void delay_ms(int tim)
{
	delay_tim = tim;
    TIM_Cmd(TIM2, ENABLE);
    while(delay_tim != 0);
    TIM_Cmd(TIM2, DISABLE);
}
