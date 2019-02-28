#include "stm32f4xx.h"
#include "main.h"
#include "uart5.h"

volatile unsigned char sbus_rx_buffer[18];
RC_Ctl_t RC_Ctl; 

/**
  * @brief  UART5配置
  * @param  None
  * @retval None
  */
void UART5_Configuration(void)
{
    USART_InitTypeDef usart5;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); 

    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD,&gpio);

    usart5.USART_BaudRate = 100000;
    usart5.USART_WordLength = USART_WordLength_8b;
    usart5.USART_StopBits = USART_StopBits_1;
    usart5.USART_Parity = USART_Parity_Even;
    usart5.USART_Mode = USART_Mode_Rx;
    usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5,&usart5);

    USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
    USART_Cmd(UART5,ENABLE);
    USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);

    nvic.NVIC_IRQChannel = UART5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Stream0);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = RX_UART5_BUFFER;
        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dma.DMA_Mode = DMA_Mode_Circular;
        dma.DMA_Priority = DMA_Priority_VeryHigh;
        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dma.DMA_MemoryBurst = DMA_Mode_Normal;
        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMA1_Stream0, &dma);
        DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream0, ENABLE);
    }
}

/**
  * @brief  UART5中断服务函数
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
	static int DATA_LENGTH=0;
	if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
        (void)UART5->SR;
        (void)UART5->DR;	
        DMA_Cmd(DMA1_Stream0,DISABLE);
        DATA_LENGTH=RX_UART5_BUFFER-DMA_GetCurrDataCounter(DMA1_Stream0);
        if(DATA_LENGTH==18)
        {
            RemoteReceive(sbus_rx_buffer);
        }
        DMA_SetCurrDataCounter(DMA1_Stream0,RX_UART5_BUFFER);	
        DMA_Cmd(DMA1_Stream0,ENABLE);
    }
}

/**
  * @brief  遥控器数据清零
  * @param  None
  * @retval None
  */
void RC_Rst(void)
{
    RC_Ctl.rc.ch0 = 1024;
    RC_Ctl.rc.ch1 = 1024;
    RC_Ctl.rc.ch2 = 1024;
    RC_Ctl.rc.ch3 = 1024;
    RC_Ctl.mouse.x = 0;
    RC_Ctl.mouse.y = 0;
    RC_Ctl.mouse.z = 0;
    RC_Ctl.mouse.press_l = 0;                                                
    RC_Ctl.mouse.press_r = 0;

    RC_Ctl.key.w = 0;
    RC_Ctl.key.s = 0;                            
    RC_Ctl.key.a = 0;
    RC_Ctl.key.d = 0;
    RC_Ctl.key.q = 0;
    RC_Ctl.key.e = 0;
    RC_Ctl.key.r = 0;
    RC_Ctl.key.f = 0;
    RC_Ctl.key.shift = 0;
    RC_Ctl.key.ctrl = 0;

    RC_Ctl.rc.s1 = 2;
    RC_Ctl.rc.s2 = 2;
}   

/**
  * @brief  遥控器数据解码
  * @param  None
  * @retval None
  */
void RemoteReceive(volatile unsigned char rx_buffer[])
{
	RC_Ctl.rc.ch0 = (rx_buffer[0]| (rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
	RC_Ctl.rc.ch1 = ((rx_buffer[1] >> 3) | (rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
	RC_Ctl.rc.ch2 = ((rx_buffer[2] >> 6) | (rx_buffer[3] << 2) | (rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
	RC_Ctl.rc.ch3 = ((rx_buffer[4] >> 1) | (rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
	RC_Ctl.rc.s1 = ((rx_buffer[5] >> 4)& 0x0003); //!< Switch left
	RC_Ctl.rc.s2 = ((rx_buffer[5] >> 6)& 0x0003);

	RC_Ctl.mouse.x = rx_buffer[6] | (rx_buffer[7] << 8); //!< Mouse X axis
	RC_Ctl.mouse.y = rx_buffer[8] | (rx_buffer[9] << 8); //!< Mouse Y axis
	RC_Ctl.mouse.z = rx_buffer[10] | (rx_buffer[11] << 8); //!< Mouse Z axis
	RC_Ctl.mouse.press_l = rx_buffer[12]; //!< Mouse Left Is Press ?
	RC_Ctl.mouse.press_r = rx_buffer[13]; //!< Mouse Right Is Press ?

	RC_Ctl.key.w = rx_buffer[14]&0x01; // KeyBoard value
	RC_Ctl.key.s = (rx_buffer[14]>>1)&0x01;
	RC_Ctl.key.a = (rx_buffer[14]>>2)&0x01;
	RC_Ctl.key.d = (rx_buffer[14]>>3)&0x01;
	RC_Ctl.key.shift = (rx_buffer[14]>>4)&0x01;
	RC_Ctl.key.ctrl = (rx_buffer[14]>>5)&0x01;
	RC_Ctl.key.q = (rx_buffer[14]>>6)&0x01;
	RC_Ctl.key.e = (rx_buffer[14]>>7)&0x01;	
	RC_Ctl.key.r = (rx_buffer[15])&0x01;
	RC_Ctl.key.f = (rx_buffer[15]>>1)&0x01;
	RC_Ctl.key.g = (rx_buffer[15]>>2)&0x01; 
	RC_Ctl.key.z = (rx_buffer[15]>>3)&0x01;
	RC_Ctl.key.x = (rx_buffer[15]>>4)&0x01;
	RC_Ctl.key.c = (rx_buffer[15]>>5)&0x01;
	RC_Ctl.key.v = (rx_buffer[15]>>6)&0x01;
	RC_Ctl.key.b = (rx_buffer[15]>>7)&0x01;
}
