#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_1st=1,Recharge_2nd=1,Recharge_3rd=1;//��־���ڻ��ϵڼ�������
u8 Recharge_1st_Get=0,Recharge_2nd_Get=0,Recharge_3rd_Get=0;
u8 RechargeStartOver ;//��־������ʼ����������������0�����Կ�ʼ��1
int delay = 1000;
extern u8 Auto_Ctrl_Flag;
extern u32 Triggercnt2;
extern int PutterPosInit;
extern float RealPos[MotorNum];
/**
  * @brief  ��ŷ�����
  * @param  None
  * @retval None
  */
void Solenoid_Config(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}
/**
  * @brief  ���ض���
  * @param  None
  * @retval None
  */
void Putter_Back(void)
{
	if(!RechargeStartOver)
	{
		//����ϲ����ܼ�⵽����------------����0����1
		if(Photocell_Detect(1));
		{
			Motor_Speed_Set(2000,0);	//�Ĵ������ת
			PushToTrack = 0;					//�Ƶ������˻�
			delay_ms(2000);
			//ReceiveUnlock;						//���������� 

			Motor_Speed_Set(0,1);				//�Ĵ����ͣת
			RechargeStartOver = 1;//���ؽ��������Կ�ʼ����
		}
		//����²����ܼ�⵽����
//		if(Photocell_Detect(2));//���Ǽ�⵽����
//		{
//			delay_ms(delay);
//			Motor_Speed_Set(0,1);				//�Ĵ����ͣת
//			RechargeStartOver = 1;//���ؽ��������Կ�ʼ����
//		}
		
		
	}
}

/**
  * @brief  ��������
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
		if(Recharge_1st)//��һ�λ����ǵ��̣�������������˫�̡�
		{
			Recharge_Pos_Move(Pos1);//�������λ�û�ֱ�������ƶ�
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����λ�ú�Ŀ��λ�ýǶ�����С��˵���������
			{
				RechargeStartOver = 0;//��������
				Recharge_1st = 0;
				Auto_Ctrl_Flag=0;
			}
		}
		
		
		
		else if(Recharge_2nd)
		{
			if(Recharge_2nd_Get == 0)
			{
				Recharge_Pos_Move(Pos3);

				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//λ���ƶ���ɷ���1
			}
			else
			{
				Recharge_Pos_Move(Pos1);

				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
				{
					Recharge_2nd = 0;
					RechargeStartOver = 0;//��������
					Auto_Ctrl_Flag=0;
				}
			}
		}
		
		else if(Recharge_3rd)
		{
			if(Recharge_3rd_Get == 0)
			{
				Recharge_Pos_Move(Pos4);

				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);//λ���ƶ���ɷ���1
			}
			else
			{
				Recharge_Pos_Move(Pos1);

				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
				{
					Recharge_3rd = 0;
					RechargeStartOver = 0;//��������
					Auto_Ctrl_Flag=0;
				}
			}
		}

}


/**
  * @brief  ��Χ�ж�����
	* @param  Range:���߷�Χ����
	*					Actual��ʵ��ֵ
	*					Target:Ŀ��ֵ
	* @retval 1:���뷶Χ;0:���ڷ�Χ��
  */
int Range_In(float Range,float Actual,float Target)
{
	if(ABS(Actual-Target) <= Range)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

