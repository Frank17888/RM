#include "FreeRTOS.h"
#include "task.h"

#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
//Ӳ����ʼ������
static void HardWare_Init(void);
//������
static void AppTaskCreate(void);
static void Commute_Task(void* pvParameters);
//������
static TaskHandle_t AppTaskCreate_Handle=NULL;
static TaskHandle_t Commute_Task_Handle=NULL;


uint32_t SRC_Buffer[16]={20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40};
union
	{
		uint8_t sendBuffer[2*32];
		uint32_t DMA_sendBuffer[32/2];	
}dshotBuffer;


int main(void)
{
	BaseType_t xReturn=pdPASS;//���崴����Ϣ����ֵ��Ĭ��ΪpdPass
	HardWare_Init();//Ӳ����ʼ��
	xReturn=xTaskCreate((TaskFunction_t )AppTaskCreate,//������ں���
											(const char*    )"AppTaskCreate",//��������
											(uint16_t				)256,//����ջ��С
											(void*					)NULL,//������ں�������
											(UBaseType_t    )1,//�������ȼ�
											(TaskHandle_t*  )&AppTaskCreate_Handle);//������ƿ�ָ��
	if(pdPASS==xReturn)
	{
		vTaskStartScheduler();//�������񣬿�ʼ����
	}
	else
		return -1;
	
	while(1);
	
}
//Ϊ�˷��������Ϊ���񴴽��������������񶼷���������
static void AppTaskCreate(void)
{
	BaseType_t xReturn=pdPASS;
	taskENTER_CRITICAL();//�����ٽ���
	
	//�˴���������1
	xReturn=xTaskCreate((TaskFunction_t)Commute_Task,//���조ͨ��
											(const char *   )"Commute_Task",
											(uint16_t      )256,
											(void*				 )NULL,
											(UBaseType_t   )2,//�������ȼ�
										  (TaskHandle_t* )&Commute_Task_Handle);
	if(pdPASS==xReturn)
	vTaskDelete(AppTaskCreate_Handle);
	
	taskEXIT_CRITICAL();//�˳��ٽ���
	
}

//����1����
static void Commute_Task(void* parameter)
{
	while(1)
	{		
	}
	
}
//Ӳ����ʼ������
static void HardWare_Init()
{
	//��������
	GPIO_InitTypeDef GPIO;
	TIM_OCInitTypeDef TIM_OC;
	TIM_TimeBaseInitTypeDef TIM_TimeBase;
	DMA_InitTypeDef DMA;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO.GPIO_Pin=GPIO_Pin_0;
	GPIO.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO);
	
	GPIO.GPIO_Pin=GPIO_Pin_1;
	GPIO.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO);
	
	//DMA����
	DMA_DeInit(DMA1_Channel3);
	DMA.DMA_PeripheralBaseAddr=(uint32_t)&(TIM3->DMAR);//TIM��DMAR����DMA
	DMA.DMA_MemoryBaseAddr=(uint32_t)dshotBuffer.DMA_sendBuffer;
	DMA.DMA_DIR=DMA_DIR_PeripheralDST;
	DMA.DMA_BufferSize=64;
	DMA.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	DMA.DMA_Mode=DMA_Mode_Circular;
	DMA.DMA_Priority=DMA_Priority_Medium;
	DMA.DMA_M2M=DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3,&DMA);
	
	//TIM����
	TIM_TimeBase.TIM_Period=30-1;//18M
	TIM_TimeBase.TIM_Prescaler=4-1;//600K,1.67us
	TIM_TimeBase.TIM_ClockDivision=TIM_CKD_DIV1;//ʱ�ӷ�Ƶϵ������Ƶ
	TIM_TimeBase.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBase.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBase);
	
	TIM_OC.TIM_Pulse=0;
	TIM_OC.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OC.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity=TIM_OCPolarity_High;//С��ʱ����ߵ�ƽ
	
	TIM_OC.TIM_OutputNState=TIM_OutputNState_Disable;
	TIM_OC.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OC.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OC.TIM_OCIdleState=TIM_OCIdleState_Reset;
	
	TIM_OC3Init(TIM3,&TIM_OC);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM3,&TIM_OC);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	nvic.NVIC_IRQChannel=DMA1_Channel3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=0;
	nvic.NVIC_IRQChannelSubPriority=0;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	
	TIM_DMAConfig(TIM3,TIM_DMABase_CCR3,TIM_DMABurstLength_2Transfers);
	TIM_DMACmd(TIM3,TIM_DMA_Update,ENABLE);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);			//ʹ��dma1channel3
	
}

//�жϴ���
uint16_t accelerator;
uint8_t BIT[16];
uint32_t ESC_CMD[2*32]={0};
int _i;
void DMA1_Channel3_IRQHandler()
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC3)==SET);
	{
		TIM_DMACmd(TIM3,TIM_DMA_Update,DISABLE);
		DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,DISABLE);
		DMA_Cmd(DMA1_Channel3,DISABLE);
		TIM_SetCompare3(TIM3,0);
		TIM_SetCompare4(TIM3,0);
		
		accelerator=LIMIT_MAX_MIN(accelerator,2000,0);
		BIT[0] =(accelerator&0x400) ?1:0;
		BIT[1] =(accelerator&0x200) ?1:0;
		BIT[2] =(accelerator&0x100) ?1:0;
		BIT[3] =(accelerator&0x80)  ?1:0;
		BIT[4] =(accelerator&0x40)  ?1:0;
		BIT[5] =(accelerator&0x20)  ?1:0;
		BIT[6] =(accelerator&0x10)	?1:0;
		BIT[7] =(accelerator&0x8)		?1:0;
		BIT[8] =(accelerator&0x4)		?1:0;
		BIT[9] =(accelerator&0x2)		?1:0;
		BIT[10]=(accelerator&0x1)		?1:0;
		BIT[11]=0;
		//getCRC
		BIT[12]=BIT[0]^BIT[4]^BIT[8];
		BIT[13]=BIT[1]^BIT[5]^BIT[9];
		BIT[14]=BIT[2]^BIT[6]^BIT[10];
		BIT[15]=BIT[3]^BIT[7]^BIT[11];
		
		for(_i=0;_i<16;_i++)
		{
			dshotBuffer.sendBuffer[2*_i]=(BIT[_i]==1)?20:10;
			dshotBuffer.sendBuffer[2*_i+1]=(BIT[_i]==1)?20:10;			
		}		
	}
	
	DMA_ClearFlag(DMA1_FLAG_TC3);
	TIM_DMACmd(TIM3,TIM_DMA_Update,ENABLE);
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);	
}








