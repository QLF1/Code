/************************************************************************************
CAN1控制底盘，ID号如下：
			|
	0x201	|   0x204
			|
————————————————————————————
			|
	0x202	|   0x203
			|

***************************************************************************************/

#include "stm32f4xx.h"
#include "main.h"
#include "can1.h"

/**
  * @brief  配置CAN1
  * @param  None
  * @retval None
  */
void CAN1_Configration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);

    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN1);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = ENABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
                          // can.CAN_Mode =CAN_Mode_LoopBack
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

    can_filter.CAN_FilterNumber = 0;
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = 0x0000;
    can_filter.CAN_FilterIdLow = 0x0000;
    can_filter.CAN_FilterMaskIdHigh = 0x0000;
    can_filter.CAN_FilterMaskIdLow = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0;
    can_filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&can_filter);

    CAN_ITConfig(CAN1, CAN_IT_TME,ENABLE); 
}


/**
  * @brief  CAN1发送中断
  * @param  None
  * @retval None
  */
void CAN1_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET) 
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}
}

/**
  * @brief  CAN1发送数据
  * @param  a：0x201电流给定
            b：0x202电流给定
            c：0x203电流给定
            d：0x204电流给定
  * @retval None
  */
void ChassisSend(int a, int b, int c, int d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
	
    a = LIMIT_MAX_MIN(a, 300, -300);
    b = LIMIT_MAX_MIN(b, 300, -300);
    c = LIMIT_MAX_MIN(c, 300, -300);
    d = LIMIT_MAX_MIN(d, 300, -300);
	  
	
    tx_message.Data[0] = (unsigned char)((a >> 8) & 0xff);
    tx_message.Data[1] = (unsigned char)(a & 0xff);  
    tx_message.Data[2] = (unsigned char)((b >> 8) & 0xff);
    tx_message.Data[3] = (unsigned char)(b & 0xff);
    tx_message.Data[4] = (unsigned char)((c >> 8) & 0xff);
    tx_message.Data[5] = (unsigned char)(c & 0xff);
    tx_message.Data[6] = (unsigned char)((d >> 8) & 0xff); 
    tx_message.Data[7] = (unsigned char)(d & 0xff);

    CAN_Transmit(CAN1, &tx_message);
}

void ChassisModeSend(int mode)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x100;
	
	if(mode == 0)
		tx_message.Data[0] = (unsigned char)(0xee);
	else if(mode == 1)
		tx_message.Data[0] = (unsigned char)(0xff);
		
    tx_message.Data[1] = (unsigned char)(0);  
    tx_message.Data[2] = (unsigned char)(0);
    tx_message.Data[3] = (unsigned char)(0);
    tx_message.Data[4] = (unsigned char)(0);
    tx_message.Data[5] = (unsigned char)(0);
    tx_message.Data[6] = (unsigned char)(0); 
    tx_message.Data[7] = (unsigned char)(0);

    CAN_Transmit(CAN1, &tx_message);
}
