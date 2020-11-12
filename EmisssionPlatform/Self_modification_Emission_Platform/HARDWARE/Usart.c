#include "main.h"

uint8_t uart3_RxBuffer[18];
UART_HandleTypeDef huart3;

void USART_Config(void)
{
	huart3.UART_BASEx = USART3;
	
	{//tim
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	}
	{//GPIO

		UART_GPIO_RX_Config(&huart3,GPIO_Pin_11,GPIOB);
	}
	{//USART
		UART_Config(&huart3,100000,8,USART_StopBits_1,USART_Parity_Even,USART_Mode_Rx);

		USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
		
		USART_Cmd(USART3, ENABLE);
		
		USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	}
	{//NVIC
		NVIC_Config(USART3_IRQn, 5, 0);
	}
	{//DMA
		DMA_Config(DMA1_Channel3,(uint32_t)&huart3.UART_BASEx->DR,(uint32_t)uart3_RxBuffer,DMA_DIR_PeripheralSRC,30,
		           DMA_PeripheralInc_Disable,DMA_MemoryInc_Enable,DMA_PeripheralDataSize_Byte,DMA_MemoryDataSize_Byte,
                   DMA_Mode_Circular,DMA_Priority_VeryHigh,DMA_M2M_Disable);
		//U3 RX
		DMA_Cmd(DMA1_Channel3,ENABLE);    
	}
}
/**
  * @brief  Initializes the USARTx peripheral 
  * @param  USART_BaudRate: 
  *         @arg 9600
  *         @arg 115200
  *         @arg 100000
  * @param  USART_WordLength: 
  *         @arg USART_WordLength_8b                 
  *         @arg USART_WordLength_9b                
  * @param  USART_StopBits: 
  *         @arg USART_StopBits_1                     
  *         @arg USART_StopBits_0_5                 
  *         @arg USART_StopBits_2                   
  *         @arg USART_StopBits_1_5              
  * @param  USART_Parity: 
  *         @arg USART_Parity_No                     
  *         @arg USART_Parity_Even                   
  *         @arg USART_Parity_Odd                    
  * @param  USART_Mode: 
  *         @arg USART_Mode_Rx                      
  *         @arg USART_Mode_Tx    
  *         @arg USART_Mode_Tx|USART_Mode_Rx     
  * @retval None
  */
void UART_Config(UART_HandleTypeDef *uart,uint32_t USART_BaudRate, 
	           uint16_t USART_WordLength,uint16_t USART_StopBits,
               uint16_t USART_Parity,uint16_t USART_Mode)
{
	USART_DeInit(uart->UART_BASEx);
	uart->UARTx.USART_BaudRate = USART_BaudRate;
	uart->UARTx.USART_WordLength = USART_WordLength;
	uart->UARTx.USART_StopBits = USART_StopBits;
	uart->UARTx.USART_Parity = USART_Parity;
	uart->UARTx.USART_Mode = USART_Mode;
	uart->UARTx.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(uart->UART_BASEx,&uart->UARTx);
}
/**
  * @brief  Initializes the GPIOx for usart RX
  * @param  GPIO_PinRx: GPIO_Pin_x,where x can be (0..15) to select the GPIO pin.      
  * @param  GPIORx :GPIOx,where x can be (A..G) to select the GPIO peripheral.
  * @retval None
  */
void UART_GPIO_RX_Config(UART_HandleTypeDef *uart, 
	                uint16_t GPIO_PinRx,GPIO_TypeDef *GPIORx)
{
	uart->UARTx_Rx.GPIO_Pin = GPIO_PinRx;
	uart->UARTx_Rx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	uart->UARTx_Rx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIORx,&uart->UARTx_Rx); 
}
/**
  * @brief  Initializes the GPIOx for usart TX
  * @param  GPIO_PinTx: GPIO_Pin_x,where x can be (0..15) to select the GPIO pin.      
  * @param  GPIOTx :GPIOx,where x can be (A..G) to select the GPIO peripheral.
  * @retval None
  */
void UART_GPIO_TX_Config(UART_HandleTypeDef *uart, 
	                uint16_t GPIO_PinTx,GPIO_TypeDef *GPIOTx)
{
	uart->UARTx_Tx.GPIO_Pin = GPIO_PinTx;
	uart->UARTx_Tx.GPIO_Mode = GPIO_Mode_AF_PP;
	uart->UARTx_Tx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOTx,&uart->UARTx_Tx); 
}

void USART3_IRQHandler(void)
{
	static int DATA_LENGTH=0;
	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)    //ÏÐÊ±ÖÐ¶Ï
	{
		(void)USART3->SR;
		(void)USART3->DR;	
		DMA_Cmd(DMA1_Channel3,DISABLE);
		DATA_LENGTH = RX_USART3_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel3);
		if(DATA_LENGTH==18)
		{
			remote_receive(uart3_RxBuffer);
			Discnect_Flag.Rc_Cnt = 0;
			Discnect_Flag.Rc_Connect = 0;
		}
		DMA_SetCurrDataCounter(DMA1_Channel3,RX_USART3_BUFFER);	
		DMA_Cmd(DMA1_Channel3,ENABLE);
		Discnect_Flag.Usart3_Cnt=0;
		Discnect_Flag.Usart3_Connect=0;
   }	
}
