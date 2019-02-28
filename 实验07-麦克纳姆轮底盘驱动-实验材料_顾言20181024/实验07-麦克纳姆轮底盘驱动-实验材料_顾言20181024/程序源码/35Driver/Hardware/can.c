#include "main.h"

//HVD232---CAN_TX---PA12
//HVD232---CAN_RX---PA11
/*******************************CAN总线配置****************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	GPIO_InitTypeDef 	   	 gpio;
	NVIC_InitTypeDef   	   nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);	//CAN_RX

	gpio.GPIO_Pin = GPIO_Pin_12;	   
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);    //CAN_TX

	nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	CAN_DeInit(CAN1);

	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = DISABLE;
	can.CAN_AWUM = DISABLE;
	can.CAN_NART = DISABLE;	  
	can.CAN_RFLM = DISABLE;																
	can.CAN_TXFP = ENABLE;		
	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_5tq;
	can.CAN_BS2 = CAN_BS2_3tq;
	can.CAN_Prescaler = 4;     //CAN BaudRate 36/(1+5+3)/4=1Mbps
	CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber = 0; 
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh = 0;
	can_filter.CAN_FilterIdLow = 0;
	can_filter.CAN_FilterMaskIdHigh = 0;
	can_filter.CAN_FilterMaskIdLow = 0;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}

short Wheelspeed;
short ctrl_mode;
extern PID WheelSpeed_Pid;
CanRxMsg rx_message;
/*******************************CAN接收中断****************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
/*******************************接收控制模式***************************************/
		if(rx_message.StdId == 0X100)
		{
			ctrl_mode = rx_message.Data[0];
		}
		
		if(rx_message.StdId == 0X200)
		{
/***************************根据驱动号接收速度**************************************/
			switch(MOTO_ID)
			{
				case 0x201:
					Wheelspeed = (short)((rx_message.Data[0]<<8)|(rx_message.Data[1]));
				break;
				
				case 0x202:
					Wheelspeed = (short)((rx_message.Data[2]<<8)|(rx_message.Data[3]));
				break;
				
				case 0x203:
					Wheelspeed = (short)((rx_message.Data[4]<<8)|(rx_message.Data[5]));
				break;
				
				case 0x204:
					Wheelspeed = (short)((rx_message.Data[6]<<8)|(rx_message.Data[7]));
				break;
			}
		}
    }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}
