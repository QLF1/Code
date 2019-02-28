#include "stm32f4xx.h"
#include "main.h"
#include "can1.h"
#include "led.h"
#include "delay.h"
#include "chassis.h"
#include "uart5.h"

int main()
{
    System_Configration();	

	while(1)
	{

	} 	
}

extern int carSpeedX, carSpeedY, carSpeedW;
extern RC_Ctl_t RC_Ctl; 
void SysTick_Handler(void)
{
	ChassisCal();
}

/**
  * @brief  系统初始化
  * @param  None
  * @retval None
  */

void System_Configration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    TIM6_Configration();
    TIM7_Configration();    
	UART5_Configuration();
    CAN1_Configration();

    delay_ms(2000);

    LED_Configration();	

    SysTick_Config(1680000);
}


