#include "main.h"
u8 First_Enter=1;
u8 Gimbal_Lock_Flag;
u16 Vcnt;
u16 Rcnt;
u16 Mousecnt;
int Pitch_PulseSet;//0~14400��Ӧռ�ձ�
extern u8 AutoCtrl_Flag;
extern float RealPos[MotorNum];
extern PID Pid_Speed;
extern PID	Pid_Gimbal_Speed_Yaw,Pid_Gimbal_Speed_Recharge,Pid_Gimbal_Speed_PushPull;
extern PID	Pid_Gimbal_Pos_Yaw,Pid_Gimbal_Pos_Recharge,Pid_Gimbal_Pos_PushPull;
extern u32 Triggercnt;
extern int PutterPosInit;
extern int PushPullPosInit;
extern int Out_Or_Back;

extern rc_ctrl_t rc_ctrl, rc_ctrl_last;
/**
  * @brief  ģ���־����
  * @param  None
  * @retval None
  */
/**
  * @brief  ҡ��ģʽ
  * @param  None
  * @retval None
  */
void Rc_Control(void)
{
	if(!AutoCtrl_Flag)//���Զ�����ģʽ
	{
		Gimbal_Power_Flag = 1;//��̨������
/*----------------------------------ң����------------------------------------*/
		if(rc_ctrl.rc.s1 == 1)//����ǰ����
		{
		/*---------------------------�����λ�õ���-------------------------------*/
			Pitch_PulseSet += (rc_ctrl.rc.ch3 - 1024) * 0.03f;
			Pid_Gimbal_Pos_Yaw.SetPoint += -(rc_ctrl.rc.ch2 - 1024) * 0.05f; 
			Pid_Gimbal_Pos_Recharge.SetPoint += (rc_ctrl.rc.ch1 - 1024) * 0.3f;
			Pid_Gimbal_Pos_PushPull.SetPoint += (rc_ctrl.rc.ch0 - 1024) * 0.10f;
			Pitch_PulseSet=LIMIT_MAX_MIN(Pitch_PulseSet,14000,400);//�����޷�
			TIM_SetCompare1(TIM3,Pitch_PulseSet);//����ռ�ձȣ��ı�pitchλ��
			
		/*---------------------------���������������-----------------------------*/
					Triggercnt = 0;
					Out_Or_Back=0;//����1���м�������

			First_Enter=1;//����ͨ��ҡ��s1�ظ�����
			
		}
		
		else if(rc_ctrl.rc.s1 == 3)//��̨��������ʱpidҪ������ȶ�����
		{
			if(First_Enter)
			{
				PushPullPosInit=RealPos[0];//��¼���ߵ��ȷ���õ�λ��
				PutterPosInit=RealPos[1];//��¼�������ȷ���õ�λ��
				
				First_Enter=0;
			}
/*--------------------------��̨����---------------------------*/
			Pid_Gimbal_Pos_PushPull.SetPoint = RealPos[0];
			Pid_Gimbal_Pos_Recharge.SetPoint=RealPos[1];
			Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[2];


/*----------------------------�ŵ�-----------------------------*/
			if(rc_ctrl.rc.ch1 - 1024 > 500)
			{
				TIM_SetCompare1(TIM5,25);//���ת90��λ��,�ŵ�
//			PushToTrack = 1;
			}
			else
			{
				TIM_SetCompare1(TIM5,15);//�ָ�0��λ��
//			PushToTrack = 0;
			}
		}
}
}

/**
  * @brief  ����ģʽ
  * @param  None
  * @retval None
  */
void Mouse_Key_Control(void)
{	
//	if(!AutoCtrl_Flag)//���Զ�����ģʽ
//	{
//		Gimbal_Power_Flag = 1;
///*******************************��̨�������*****************************/
//		if(Gimbal_Lock_Flag == 0)//δ������̨,���ɿ���
//		{
//			Pid_Gimbal_Pos_Yaw.SetPoint += rc_ctrl.mouse.x * 50.0f;
//			 .SetPoint += rc_ctrl.mouse.y * 50.0f;
//		}
//		else if(Gimbal_Lock_Flag == 1)
//		{
//			Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
//			 .SetPoint = RealPos[0];
//		}
///********************************��̨�����л�***************************/
//		if(rc_ctrl.key.v == 1)//����v
//		{
//			Vcnt++;
//		}
//		else if(rc_ctrl.key.v == 0)
//		{
//			Vcnt = 0;
//		}
//		else
//		{
//			Vcnt = 0;
//		}
//		if(Vcnt == 700)//����700ms
//		{
//			Gimbal_Lock_Flag = 1 - Gimbal_Lock_Flag;//�л���̨����״̬
//		}
///*********************************�Ƶ�****************************/
//		if(rc_ctrl.mouse.press_r == 1)
//		{		
//			if(rc_ctrl_last.mouse.press_l == 0&&rc_ctrl_last.mouse.press_l == 1)
//			{
//				Mousecnt = 0;
//				PushToTrack = 1;
//			}
//		}
//		else if(rc_ctrl.mouse.press_r == 0&&rc_ctrl.mouse.press_l == 0)
//		{
//			Mousecnt++;
//		}
//		else
//		{
//		}
//		if(Mousecnt > 500)
//		{
//			Mousecnt = 501;
//			PushToTrack = 0;
//		}
///*********************************����****************************/
//		if(rc_ctrl.key.r == 1)//����v
//		{
//			Rcnt++;
//		}
//		else if(rc_ctrl.key.r == 0)
//		{
//			Rcnt = 0;
//		}
//		else
//		{
//			Rcnt = 0;
//		}
//		if(Rcnt == 300)//����300ms
//		{
//			Recharge_Task();
//		}
///*********************************�Ĵ���ת����****************************/
//		if(rc_ctrl.key.shift == 1)
//		{
//			if(rc_ctrl.key.w == 1&&rc_ctrl.key.s == 0)
//			{
//				Motor_Speed_Set(2000,1);
//			}
//			else if(rc_ctrl.key.s == 1&&rc_ctrl.key.w == 0)
//			{
//				Motor_Speed_Set(1000,0);
//			}
//		}
//	}
}
/**
  * @brief  ����ģʽ
  * @param  None
  * @retval None
  */
void Power_Off(void)
{
	Gimbal_Power_Flag = 0;//��̨0����
	Solenoid_Power_Flag = 0;//��ŷ�ʧ��
//	PAout(2) = 0;//����ʧ��?�б�Ҫʧ������һ������ƽ�仯��
//	Motor_Speed_Set(0,1);//�Ĵ�ͣת
}
/**
  * @brief  ģʽִ��
  * @param  Sta��ģʽ״̬
  * @retval None
  */
void Status_Act(Status_ Sta)
{
	
		switch(Sta.Rc_States)
		{
			case State0:
				Mouse_Key_Control();
				break;
			case State1:
				Rc_Control();
				break;
			case State2:
				Power_Off();
				break;
			default:
				Power_Off();
				break;
		}
	
}
/**
  * @brief  ��������
  * @param  State������״̬
  * @retval None
  */

/**
  * @brief  ���߼��
  * @param  Discnect_Flag:���߽ṹ��
  * @retval ���ض���id
  */
u32 Offline_Detect(Disconnect_Flag *Discnect_Flag)
{
	u32 Discon_Flag;
/**************CAN1���߼��*******************/
	if(Discnect_Flag->Can1_Connect == 0)
	{
		Discnect_Flag->Can1_Cnt ++;
		Discon_Flag &= 0xfffe;//ʵʱ���߱�־λ����1111 1111 1111 1110
	}
	if(Discnect_Flag->Can1_Cnt > MS)//����ʱ��>0.1s
	{
		Discnect_Flag->Can1_Disonnect_Ever = 1;
		Discnect_Flag->Can1_Connect = 1;
		Discon_Flag |= 0x0001;
	}
/**************CAN2���߼��*******************/
	if(Discnect_Flag->Can2_Connect == 0)
	{
		Discnect_Flag->Can2_Cnt ++;
		Discon_Flag &= 0xfffd;//1111 1111 1111 1101
	}
	if(Discnect_Flag->Can2_Cnt > MS)
	{
		Discnect_Flag->Can2_Disonnect_Ever = 1;
		Discnect_Flag->Can2_Connect = 1;
		Discon_Flag |= 0x0002;
	}
/**************ң�������߼��*******************/
	if(Discnect_Flag->Rc_Connect == 0)
	{
		Discnect_Flag->Rc_Cnt ++;
		Discon_Flag &= 0xfffb;//1111 1111 1111 1011
	}
	if(Discnect_Flag->Rc_Cnt > MS)
	{
		Discnect_Flag->Rc_Disonnect_Ever = 1;
		Discnect_Flag->Rc_Connect = 1;
		Discon_Flag |= 0x0004;
	}
/**************�����Ƕ��߼��*******************/
	if(Discnect_Flag->Gyro_Connect == 0)
	{
		Discnect_Flag->Gyro_Cnt ++;
		Discon_Flag &= 0xfff7;//1111 1111 1111 0111
	}
	if(Discnect_Flag->Gyro_Cnt > MS)
	{
		Discnect_Flag->Gyro_Disonnect_Ever = 1;
		Discnect_Flag->Gyro_Connect = 1;
		Discon_Flag |= 0x0008;
	}
/**************��̨���߼��*******************/
	if(Discnect_Flag->Gimbal_Connect == 0)
	{
		Discnect_Flag->Gimbal_Cnt ++;
		Discon_Flag &= 0xffef;//1111 1111 1110 1111
	}
	if(Discnect_Flag->Gimbal_Cnt > MS)
	{
		Discnect_Flag->Gimbal_Disonnect_Ever = 1;
		Discnect_Flag->Gimbal_Connect = 1;
		Discon_Flag |= 0x0010;
	}	
/**************Usart3���߼��*******************/
	if(Discnect_Flag->Usart3_Connect == 0)
	{
		Discnect_Flag->Usart3_Cnt ++;
		Discon_Flag &= 0xffdf;//1111 1111 1101 1111
	}
	if(Discnect_Flag->Usart3_Cnt > MS)
	{
		Discnect_Flag->Usart3_Disonnect_Ever = 1;
		Discnect_Flag->Usart3_Connect = 1;
		Discon_Flag |= 0x0020;
	}
/*********************************************/
	return Discon_Flag;
}
