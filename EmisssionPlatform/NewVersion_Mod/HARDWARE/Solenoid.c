#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_1st=1,Recharge_2nd=1,Recharge_3rd=1;//��־���ڻ��ϵڼ�������
u8 Recharge_1st_Get=0,Recharge_2nd_Get=0,Recharge_3rd_Get=0;
u8 RechargeWork_Flag ;//��־������ʼ����������������0�����Կ�ʼ��1

u8 PullWork_Flag=0;//���ߵ��������־λ��0��������1������
u8 PushWork_Flag=0;//���ߣ�ͬ��
int delay = 1000;
u32 PutterDelay=0;
u32 HandlerDelay_2nd=0;
u32 HandlerDelay_3rd=0;
extern u8 AutoCtrl_Flag;
extern u8 MissileUp_Flag;
extern int PutterPosInit;
extern int PushPullPosInit;
extern float RealPos[MotorNum];
extern float RealSpeed[MotorNum];
/**
  * @brief  ��ŷ�����
  * @param  None
  * @retval None
  */
/**
  * @brief  ���ض���
  * @param  None
  * @retval None
  */
void PullWork_Task()
{
	if(MissileUp_Flag)			//�������
	{
		if(PullWork_Flag)			//���ߵ����ʼ����
		{
			PushPull_Pos_Move(PullPos);
			if(!RealSpeed[0])	//��������ף�ͣת�ٷ�ת
			{
				PullWork_Flag=0;					//ֹͣ����
				PushWork_Flag=1;					//��ʼ����
				RechargeWork_Flag=1;			//���Կ�ʼ����
			}
		}//�������
		
		else if(PushWork_Flag)
		{
			PushPull_Pos_Move(0);//���ߵ���ָ�λ��
			if(Range_In(1000,RealPos[0],PushPullPosInit+PullPos))	//PushPullPosInit ���ߵ����ʼ��ֵ
			{
				PullWork_Flag=1;//�ָ�����״̬
				PushWork_Flag=0;//ֹͣ����
				MissileUp_Flag=0;//�����¼��������
			}
		}
		
	}
}



/**
  * @brief  ��������
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
	if(RechargeWork_Flag)
	{	
		if(Recharge_1st)//��һ�λ����ǵ��̣�������������˫�̡�
		{
			Recharge_Pos_Move(Pos1);//�������λ�û�ֱ�������ƶ�
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����λ�ú�Ŀ��λ�ýǶ�����С��˵���������
			{
				RechargeWork_Flag = 0;//����ֹͣ����
				Recharge_1st = 0;
//				AutoCtrl_Flag=0;
			}
		}
		else if(Recharge_2nd)
		{
			if(Recharge_2nd_Get == 0)//û���ƶ���ָ��λ��
			{
				Recharge_Pos_Move(Pos3);
				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//λ���ƶ���ɷ���1
			}
			else
			{
				if(PutterDelay<PutterDelayTime)//�Ƹ˵����û���·���
				{
					PutterDelay++;
					Putter_Move_Updown(0);
				}
				else
				{
					PutterDelay=0;
					Recharge_Pos_Move(Pos1);//ȡ���ֹ�λ
					Putter_Move_Updown(1);
					if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
					{
						Recharge_2nd = 0;
						RechargeWork_Flag = 0;//����ֹͣ����
					}
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
				if(PutterDelay<PutterDelayTime)
				{
					PutterDelay++;
					Putter_Move_Updown(0);
				}
				else
				{
					PutterDelay=0;
					Recharge_Pos_Move(Pos1);
					Putter_Move_Updown(1);
					if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
					{
						Recharge_3rd = 0;
						RechargeWork_Flag = 0;//����ֹͣ����
					}
				}
			}
		}
	}
	
	
	
	
	
	
	
//	void Recharge_Task(void)
//{
//	if(RechargeWork_Flag)
//	{	
//		if(Recharge_1st)//��һ�λ����ǵ��̣�������������˫�̡�
//		{
//			Recharge_Pos_Move(Pos1);//�������λ�û�ֱ�������ƶ�
//			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����λ�ú�Ŀ��λ�ýǶ�����С��˵���������
//			{
//				RechargeWork_Flag = 0;//����ֹͣ����
//				Recharge_1st = 0;
////				AutoCtrl_Flag=0;
//			}
//		}
//		else if(Recharge_2nd)
//		{
//			if(Recharge_2nd_Get == 0)
//			{
//				Recharge_Pos_Move(Pos3);
//				Putter_Move_Updown(0);
//				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//λ���ƶ���ɷ���1
//			}
//			else
//			{
//				Recharge_Pos_Move(Pos1);
//				Putter_Move_Updown(1);
//				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
//				{
//					Recharge_2nd = 0;
//					RechargeWork_Flag = 0;//����ֹͣ����
//				}
//			}
//		}
//		
//		else if(Recharge_3rd)
//		{
//			if(Recharge_3rd_Get == 0)
//			{
//				Recharge_Pos_Move(Pos4);
//				Putter_Move_Updown(0);
//				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);//λ���ƶ���ɷ���1
//			}
//			else
//			{
//				Recharge_Pos_Move(Pos1);
//				Putter_Move_Updown(1);
//				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//����ƶ��������
//				{
//					Recharge_3rd = 0;
//					RechargeWork_Flag = 0;//����ֹͣ����
//				}
//			}
//		}
//	}
//}
}



void CheckAutoCtrl()
{
	AutoCtrl_Flag=MissileUp_Flag||RechargeWork_Flag;//�����¼��򻻵��¼�δ����������ң��������
}



void Flag_Initialize()
{
	AutoCtrl_Flag=0;
	MissileUp_Flag=0;
	PushWork_Flag=0;
	PullWork_Flag=0;
	RechargeWork_Flag=0;
	
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

