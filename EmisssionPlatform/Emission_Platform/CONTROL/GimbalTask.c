#include "main.h"
u8 Gimbal_Power_Flag = 1;
int PutterPosInit;

float PitchSpeedLlimit = 10000;
float YawSpeedLimit = 3000;
float RealPos[MotorNum];
float LastPos[MotorNum];
float CurrentSend[MotorNum];
float RealSpeed[MotorNum];

PID	Pid_Gimbal_Speed_Yaw,Pid_Gimbal_Speed_Pitch;
PID	Pid_Gimbal_Pos_Yaw,Pid_Gimbal_Pos_Pitch;

extern rmc620_t Gimbal_Motor[2];//[0]pitch[1]yaw

void Pid_Gimbal_Cal_Rc(void)
{
	//位置环
	Pid_Gimbal_Speed_Pitch.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Gimbal_Pos_Pitch,RealPos[0]),PitchSpeedLlimit,-PitchSpeedLlimit);//跑完全程440 0000的距离
	Pid_Gimbal_Speed_Yaw.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Gimbal_Pos_Yaw,RealPos[1]),YawSpeedLimit,-YawSpeedLimit);
	//速度环
	RealSpeed[0] = Int_To_Float(Gimbal_Motor[0].speed);
	CurrentSend[0] = PID_Calc(&Pid_Gimbal_Speed_Pitch,RealSpeed[0]);
	
	RealSpeed[1] = Int_To_Float(Gimbal_Motor[1].speed);
	CurrentSend[1] = PID_Calc(&Pid_Gimbal_Speed_Yaw,RealSpeed[1]);
	//发送电流
	if(Gimbal_Power_Flag == 1)
	{
		Gimbal_Current_Send(Float_To_Int(CurrentSend[0]),0,Float_To_Int(CurrentSend[1]),0);//201是换弹，203是yaw
	}
	else
	{
		Gimbal_Current_Send(0,0,0,0);
	}
}
/**
  * @brief  云台电机位置Pid初始化
  * @param  None
  * @retval None
  */ 
void Pid_Gimbal_Pos_Init(void)
{
	PutterPosInit = Gimbal_Motor[0].angle;
	Pid_Gimbal_Pos_Pitch.SetPoint = Gimbal_Motor[0].angle;
	Pid_Gimbal_Pos_Pitch.P = 0.5f;
	Pid_Gimbal_Pos_Pitch.I = 0.0f;
	Pid_Gimbal_Pos_Pitch.D = 0;
	Pid_Gimbal_Pos_Pitch.IMax = 500;	
	
	Pid_Gimbal_Pos_Yaw.SetPoint = Gimbal_Motor[1].angle;
	Pid_Gimbal_Pos_Yaw.P = 0.3f;
	Pid_Gimbal_Pos_Yaw.I = 0.003f;
	Pid_Gimbal_Pos_Yaw.D = 0;
	Pid_Gimbal_Pos_Yaw.IMax = 100;
}
/**
  * @brief  云台电机速度Pid初始化
  * @param  None
  * @retval None
  */ 
void Pid_Gimbal_Speed_Init(void)
{
	Pid_Gimbal_Speed_Pitch.SetPoint = 0;
	Pid_Gimbal_Speed_Pitch.P = 4.0f;
	Pid_Gimbal_Speed_Pitch.I = 0.1f;
	Pid_Gimbal_Speed_Pitch.D = 1.0f;
	Pid_Gimbal_Speed_Pitch.IMax = 500;
	
	Pid_Gimbal_Speed_Yaw.SetPoint = 0;
	Pid_Gimbal_Speed_Yaw.P = 9.0f;
	Pid_Gimbal_Speed_Yaw.I = 0.0f;
	Pid_Gimbal_Speed_Yaw.D = 0.0f;
	Pid_Gimbal_Speed_Yaw.IMax = 500;
}
/**
  * @brief  换弹电机定位移动
  * @param  None
  * @retval None
  */ 
void Recharge_Pos_Move(int position)
{
	Pid_Gimbal_Pos_Pitch.SetPoint = PutterPosInit + position;
}
