#include "main.h"

u8 Foreback;
int MotorSpeed;
float Target_Speed;
float RSpeed_Change;

extern float RSpeed;
extern float Encoder_Speed;

PID Pid_Speed;
/**
  * @brief  遥控履带
  * @param  None
  * @retval None
  */ 
void Motor_Cal(void)
{
//	//油门
//	{
//		if (rc_ctrl.rc.ch3 > 1044 || rc_ctrl.rc.ch3 < 1004)
//			Pid_Speed.SetPoint = ( - 1024 + rc_ctrl.rc.ch3) * 11.2f;//7392+7000左右极限油门
//		else 
//			Pid_Speed.SetPoint = 0;
//	}
//	//正反转
//	{
//		if(rc_ctrl.rc.ch1 > 1044 || rc_ctrl.rc.ch1 < 1004)
//		{
//			if(( - 1024 + rc_ctrl.rc.ch1) > 0)	Foreback = 1;//正转模式
//			else if(( - 1024 + rc_ctrl.rc.ch1) < 0)	Foreback = 0;//反转模式
//			else Pid_Speed.SetPoint = 0;
//		}
//	}
}
/**
  * @brief  履带电机闭环控速
  * @param  None
  * @retval None
  */ 
void Pid_Speed_Cal(void)
{
	if(Pid_Speed.SetPoint == 0) Pid_Speed.SumError = 0;
	
	RSpeed_Change = PID_Calc(&Pid_Speed,Encoder_Speed);//反馈速度增量
	Target_Speed += RSpeed_Change;
	MotorSpeed = (int)Target_Speed;
	
	Motor_Speed_Set(MotorSpeed,1);
}
/**
  * @brief  履带电机Pid初始化
  * @param  None
  * @retval None
  */ 
void Pid_Speed_Init(void)
{
	Pid_Speed.SetPoint = 0;
	Pid_Speed.P = 0;
	Pid_Speed.I = 0;
	Pid_Speed.D = 0;
	Pid_Speed.IMax = 5000;
}
