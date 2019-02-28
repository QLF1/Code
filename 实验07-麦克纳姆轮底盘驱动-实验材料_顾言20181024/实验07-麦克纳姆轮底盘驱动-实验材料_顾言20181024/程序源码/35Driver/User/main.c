#include "main.h"
RCC_ClocksTypeDef rcc;
/********************************主函数*********************************************/
int main(void)
{
	RCC_GetClocksFreq(&rcc);		//时钟配置
	system_Config();				//系统配置 

	while(1)
	{
		
	}
}

/*******************************系统配置********************************************/
void system_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration();
	LED_Configuration();
	PWM_Init();
	Encoder_Init();
	TIM2Init();
	CAN_Configuration();
	PID_Init();
	delay_ms(1000);
	SysTick_Config(720000);		//10ms定时中断
}

int speedCur;
int EncoderPulse,encode_last, encoder_dif;
extern int EncoderCircle;
extern PID WheelSpeed_Pid;
extern short Wheelspeed, ctrl_mode;
/*******************************10ms定时中断******************************************/
void SysTick_Handler(void)
{
/*******************************获取速度并迭代****************************************/
	EncoderPulse = (EncoderCircle<<16)+TIM_GetCounter(TIM4);
	encoder_dif = EncoderPulse - encode_last;
	encode_last = EncoderPulse;

/***************************闭环模式计算电流值****************************************/
	if(ctrl_mode == 0xff)
	{
		WheelSpeed_Pid.SetPoint = Wheelspeed;
		speedCur = (int)PIDCalc(&WheelSpeed_Pid, encoder_dif);
	}
/***************************开环模式计算电流值****************************************/
	else if(ctrl_mode == 0xee)
	{
		speedCur = Wheelspeed;
	}
/*********************************发送PWM*********************************************/
	Motormove(speedCur);
}

/*******************************时钟配置，与F4匹配************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);  
		RCC_PCLK1Config(RCC_HCLK_Div2);   
		FLASH_SetLatency(FLASH_Latency_2); 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);      
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     
		RCC_PLLCmd(ENABLE); 

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

		while(RCC_GetSYSCLKSource() != 0x08)     
		{ 
		}
	}	
}

