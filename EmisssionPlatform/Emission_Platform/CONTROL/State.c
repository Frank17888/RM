#include "main.h"

u8 Gimbal_Lock_Flag;
u16 Vcnt;
u16 Rcnt;
u16 Mousecnt;

extern float RealPos[MotorNum];
extern PID Pid_Speed;
/**
  * @brief  ģ���־����
  * @param  None
  * @retval None
  */
void Flag_Refresh(void)
{
	Gimbal_Lock_Flag = 0;//��������̨
}
/**
  * @brief  ҡ��ģʽ
  * @param  None
  * @retval None
  */
void Rc_Control(void)
{
	int MotorSpeed;
	float tempspeed;
/***************************ң����****************************/
	if(rc_ctrl.rc.s1 == 1)
	{
		Pid_Gimbal_Pos_Pitch.SetPoint += (rc_ctrl.rc.ch3 - 1024) * 0.0005f;
		Pid_Gimbal_Pos_Yaw.SetPoint += (rc_ctrl.rc.ch2 - 1024) * 0.0003f;
		Pid_Gimbal_Pos_Pitch.SetPoint += (rc_ctrl.rc.ch1 - 1024) * 0.00015f;
		Pid_Gimbal_Pos_Yaw.SetPoint += (rc_ctrl.rc.ch0 - 1024) * 0.00010f;
	}
	else if(rc_ctrl.rc.s1 == 3)//��̨��������ʱpidҪ������ȶ�����
	{
/***************************��̨����****************************/
		Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
		Pid_Gimbal_Pos_Pitch.SetPoint = RealPos[0];
/***************************�Ĵ����������****************************/
		if(ABS(rc_ctrl.rc.ch3 - 1024) > 20)
		{
			tempspeed += (rc_ctrl.rc.ch3 - 1024) * 0.001f;
		}
		MotorSpeed = (int)tempspeed;
		if(MotorSpeed >= 0)
		{
			Motor_Speed_Set(MotorSpeed,1);
		}
		else
		{
			Motor_Speed_Set(0-MotorSpeed,0);
		}
/***************************�Ƶ�****************************/
		if(rc_ctrl.rc.ch1 - 1024 > 500)
		{
			PushToTrack = 1;
		}
		else
		{
			PushToTrack = 0;
		}
/*****************************����************************/
		if(rc_ctrl_last.rc.ch1 - 1024 <= 500 && rc_ctrl.rc.ch1-1024 > 500)
		{
			Reload_Task();
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
/*******************************��̨�������*****************************/
	if(Gimbal_Lock_Flag == 0)//δ������̨,���ɿ���
	{
		Pid_Gimbal_Pos_Yaw.SetPoint += rc_ctrl.mouse.x * 50.0f;
		Pid_Gimbal_Pos_Pitch.SetPoint += rc_ctrl.mouse.y * 50.0f;
	}
	else if(Gimbal_Lock_Flag == 1)
	{
		Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
		Pid_Gimbal_Pos_Pitch.SetPoint = RealPos[0];
	}
/********************************��̨�����л�***************************/
	if(rc_ctrl.key.v == 1)//����v
	{
		Vcnt++;
	}
	else if(rc_ctrl.key.v == 0)
	{
		Vcnt = 0;
	}
	else
	{
		Vcnt = 0;
	}
	if(Vcnt == 700)//����700ms
	{
		Gimbal_Lock_Flag = 1 - Gimbal_Lock_Flag;//�л���̨����״̬
	}
/*********************************�Ƶ�****************************/
	if(rc_ctrl.mouse.press_r == 1)
	{
		if(rc_ctrl_last.mouse.press_l == 0&&rc_ctrl_last.mouse.press_l == 1)
		{
			Mousecnt = 0;
			PushToTrack = 1;
		}
	}
	else if(rc_ctrl.mouse.press_r == 0&&rc_ctrl.mouse.press_l == 0)
	{
		Mousecnt++;
	}
	else
	{
	}
	if(Mousecnt > 500)
	{
		Mousecnt = 501;
		PushToTrack = 0;
	}
/*********************************����****************************/
	if(rc_ctrl.key.r == 1)//����v
	{
		Rcnt++;
	}
	else if(rc_ctrl.key.r == 0)
	{
		Rcnt = 0;
	}
	else
	{
		Rcnt = 0;
	}
	if(Rcnt == 300)//����300ms
	{
		Reload_Task();
	}
/*********************************�Ĵ���ת����****************************/
	if(rc_ctrl.key.shift == 1)
	{
		if(rc_ctrl.key.w == 1&&rc_ctrl.key.s == 0)
		{
			Motor_Speed_Set(2000,1);
		}
		else if(rc_ctrl.key.s == 1&&rc_ctrl.key.w == 0)
		{
			Motor_Speed_Set(1000,0);
		}
	}
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
	Motor_Speed_Set(0,1);//�Ĵ�ͣת
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
			Flag_Refresh();
			Mouse_Key_Control();
		case State1:
			Flag_Refresh();
			Rc_Control();
		case State2:
			Power_Off();
		default:
			Power_Off();
	}
}
/**
  * @brief  ��������
  * @param  State������״̬
  * @retval None
  */
void Lauch_State_Convert(Lauching_States State)
{
	switch(State)
	{
		case L_State0://�������
			Pid_Speed.SetPoint = ShootSpeed1;
			delay_ms(2000);//�������
		break;
		
		case L_State1://���׽������ƽ��Ĵ�
			PushToTrack = 1;
		break;
		
		case L_State2://���ٹ���1��⵽������ɺ󣬵�����ٷ�ת
			PushToTrack = 0;
			Motor_Speed_Set(1500,0);
			delay_ms(3000);
		break;
		
		case L_State3://������������ر�־����
		break;
		
		default:
		break;
	}
}
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
		Discon_Flag &= 0xfffe;//ʵʱ���߱�־λ����
	}
	if(Discnect_Flag->Can1_Cnt > MS)
	{
		Discnect_Flag->Can1_Disonnect_Ever = 1;
		Discnect_Flag->Can1_Connect = 1;
		Discon_Flag |= 0x0001;
	}
/**************CAN2���߼��*******************/
	if(Discnect_Flag->Can2_Connect == 0)
	{
		Discnect_Flag->Can2_Cnt ++;
		Discon_Flag &= 0xfffd;
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
		Discon_Flag &= 0xfffb;
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
		Discon_Flag &= 0xfff7;
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
		Discon_Flag &= 0xffef;
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
		Discon_Flag &= 0xffdf;
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
