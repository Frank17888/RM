#include "main.h"
u8 Gimbal_Power_Flag = 1;
int PutterPosInit;

float PitchSpeedLlimit = 10000;
float YawSpeedLimit = 3000;
float RechargeSpeedLimit=7000;
float RealPos[MotorNum];
float LastPos[MotorNum];
int CurrentSend[MotorNum];
float RealSpeed[MotorNum];

PID	Pid_Gimbal_Speed_Yaw,Pid_Gimbal_Speed_Pitch,Pid_Gimbal_Speed_Recharge;
PID	Pid_Gimbal_Pos_Yaw,Pid_Gimbal_Pos_Pitch,Pid_Gimbal_Pos_Recharge;

extern rmc620_t Gimbal_Motor[2];//[0]pitch[1]yaw[2]pitch

void Pid_Gimbal_Cal_Rc(void)
{
	//位置环
//	Pid_Gimbal_Speed_Pitch.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Gimbal_Pos_Pitch,RealPos[2]),PitchSpeedLlimit,-PitchSpeedLlimit);//跑完全程440 0000的距离
	Pid_Gimbal_Speed_Yaw.SetPoint = LIMIT_MAX_MIN(PID_Calc(&Pid_Gimbal_Pos_Yaw,RealPos[1]),YawSpeedLimit,-YawSpeedLimit);
	Pid_Gimbal_Speed_Recharge.SetPoint =LIMIT_MAX_MIN(PID_Calc(&Pid_Gimbal_Pos_Recharge,RealPos[0]),RechargeSpeedLimit,-RechargeSpeedLimit);
	//速度环
	RealSpeed[0] = Int_To_Float(Gimbal_Motor[0].speed);
	CurrentSend[0] = (int)PID_Calc(&Pid_Gimbal_Speed_Recharge,RealSpeed[0]);
	
	RealSpeed[1] = Int_To_Float(Gimbal_Motor[1].speed);
	CurrentSend[1] = (int)PID_Calc(&Pid_Gimbal_Speed_Yaw,RealSpeed[1]);
	
//	RealSpeed[2] = Int_To_Float(Gimbal_Motor[2].speed);
//	CurrentSend[2] = PID_Calc(&Pid_Gimbal_Speed_Pitch,RealSpeed[2]);

	
	//201换弹，202pitch，203yaw
	//掉电模式发送电流为0；
	if(Gimbal_Power_Flag == 0)
	{
		for(int i=0;i<MotorNum;i++)
		{
			CurrentSend[i]=0;
		}
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
//	Pid_Gimbal_Pos_Pitch.SetPoint = 0;//Gimbal_Motor[2].angle;
//	Pid_Gimbal_Pos_Pitch.P = 0.5f;
//	Pid_Gimbal_Pos_Pitch.I = 0.0f;
//	Pid_Gimbal_Pos_Pitch.D = 0;
//	Pid_Gimbal_Pos_Pitch.IMax = 500;	
	
	Pid_Gimbal_Pos_Yaw.SetPoint = Gimbal_Motor[1].angle;
	Pid_Gimbal_Pos_Yaw.P = 0.3f;
	Pid_Gimbal_Pos_Yaw.I = 0.003f;
	Pid_Gimbal_Pos_Yaw.D = 0;
	Pid_Gimbal_Pos_Yaw.IMax = 100;
	
	Pid_Gimbal_Pos_Recharge.SetPoint = Gimbal_Motor[0].angle;
	Pid_Gimbal_Pos_Recharge.P = 0.75f;
	Pid_Gimbal_Pos_Recharge.I = 0.015f;
	Pid_Gimbal_Pos_Recharge.D = 0.0f;
	Pid_Gimbal_Pos_Recharge.IMax = 500;
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
	
	Pid_Gimbal_Speed_Recharge.SetPoint = 0;
	Pid_Gimbal_Speed_Recharge.P = 11.0f;
	Pid_Gimbal_Speed_Recharge.I = 0.0f;
	Pid_Gimbal_Speed_Recharge.D = 0.0f;
	Pid_Gimbal_Speed_Recharge.IMax = 500;
}
/**
  * @brief  换弹电机定位移动
  * @param  None
  * @retval None
  */ 
void Recharge_Pos_Move(int position)
{
	Pid_Gimbal_Pos_Recharge.SetPoint = PutterPosInit + position;
}
