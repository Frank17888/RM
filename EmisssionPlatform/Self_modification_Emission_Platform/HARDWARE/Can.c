#include "main.h"

u8	Gyro_Initial_Flag;
float yaw_angle;
float gz;
rmc620_t Gimbal_Motor[MotorNum];//[0]换弹[1]yaw[2]pitch

extern PID	Pid_Gimbal_Pos_Yaw;

/**
  * @brief  配置CAN1
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


	/* CAN1 RX PB8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN1 TX PB9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);  // CAN1 remap

	/* CAN1 Enabling interrupt */	
	NVIC_Config(CAN1_RX0_IRQn, 0, 0);									
	NVIC_Config(CAN1_TX_IRQn, 1, 0);
	
	/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);   

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=ENABLE;
	CAN_InitStructure.CAN_NART=ENABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
	CAN_InitStructure.CAN_Prescaler=4;

	CAN_Init(CAN1,&CAN_InitStructure);	// CAN1											

	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
//	CAN1->IER = 0x03;

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}


void CAN1_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_TME)!=0)
	{
		
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
	
}
/**
  * @brief  CAN1接收中断
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		switch(rx_message.StdId)
    {
            case 0x201:
                Gimbal_Motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                Gimbal_Motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
            break;
           
            case 0x203:
                Gimbal_Motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                Gimbal_Motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
            break;    
						
						default:
						break;
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		Discnect_Flag.Can1_Cnt=0;
		Discnect_Flag.Can1_Connect=0;
	}
}	
/**
  * @brief  CAN1发送数据,内带电流限幅 -16384 ~ 0 ~ 16384
  * @param  a：0x201电流给定
            b：0x202电流给定
            c：0x203电流给定
            d：0x204电流给定
  * @retval None
  */
void Gimbal_Current_Send(int a, int b, int c, int d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
	
    a = LIMIT_MAX_MIN(a, Current_Max, -Current_Max);
    b = LIMIT_MAX_MIN(b, Current_Max, -Current_Max);
    c = LIMIT_MAX_MIN(c, Current_Max, -Current_Max);
    d = LIMIT_MAX_MIN(d, Current_Max, -Current_Max);
	
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
/**
  * @brief  符号整型转浮点
	* @param  Inte:整型
  * @retval 单精度浮点型
  */
float Int_To_Float(int Inte)
{
	return Inte;
}
/**
  * @brief  浮点型转符号整型
	* @param  Inte:整型
  * @retval 整型
  */
int Float_To_Int(float Floa)
{
	return Floa;
}
/**
  * @brief  配置CAN2
  * @param  None
  * @retval None
  */
void CAN2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	/* CAN2 RX PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN2 TX PB13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);								

	/* CAN2 Enabling interrupt */								 	  
	NVIC_Config(CAN2_RX1_IRQn, 2, 1);
	NVIC_Config(CAN2_TX_IRQn, 2, 2);
	
	/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_DeInit(CAN2);
	CAN_StructInit(&CAN_InitStructure);   

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=ENABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
	//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
	CAN_InitStructure.CAN_Prescaler=4;

	CAN_Init(CAN2,&CAN_InitStructure);   // CAN2													
	
	CAN_FilterInitStructure.CAN_FilterNumber=15;	// 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x100 << 5;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0 | CAN_ID_STD;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x101 << 5;		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE); 
	CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}
	
/**
  * @brief  CAN2接收中断，FIFO1
  * @param  None
  * @retval None
  */
CanRxMsg rx_message_1;	

void CAN2_RX1_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET) 
	{
    CAN_Receive(CAN2, CAN_FIFO1, &rx_message_1);	
		if (rx_message_1.StdId == 0x101)
		{
			memcpy(&yaw_angle,&rx_message_1.Data,4);
			memcpy(&gz,&(rx_message_1.Data[4]),4);
			
		}
		
		Discnect_Flag.Gyro_Cnt = 0;
		Discnect_Flag.Gyro_Connect = 0;
		if(Gyro_Initial_Flag == 0)
		{
			Gyro_Initial_Flag = 1;
			Pid_Gimbal_Pos_Yaw.SetPoint = yaw_angle;
		}
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
		Discnect_Flag.Can2_Cnt=0;
		Discnect_Flag.Can2_Connect=0;
	}
}

/**
  * @brief  CAN2发送中断
  * @param  None
  * @retval None
  */
void CAN2_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2, CAN_IT_TME) != RESET) 
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_TME);
	}
}

