#include "main.h"

u8 Gimbal_Lock_Flag;
u16 Vcnt;
u16 Rcnt;
u16 Mousecnt;

extern float RealPos[MotorNum];
extern PID Pid_Speed;
/**
  * @brief  模块标志清零
  * @param  None
  * @retval None
  */
void Flag_Refresh(void)
{
	Gimbal_Lock_Flag = 0;//不锁定云台
}
/**
  * @brief  摇杆模式
  * @param  None
  * @retval None
  */
void Rc_Control(void)
{
	int MotorSpeed;
	float tempspeed;
/***************************遥控器****************************/
	if(rc_ctrl.rc.s1 == 1)
	{
		Pid_Gimbal_Pos_Pitch.SetPoint += (rc_ctrl.rc.ch3 - 1024) * 0.0005f;
		Pid_Gimbal_Pos_Yaw.SetPoint += (rc_ctrl.rc.ch2 - 1024) * 0.0003f;
		Pid_Gimbal_Pos_Pitch.SetPoint += (rc_ctrl.rc.ch1 - 1024) * 0.00015f;
		Pid_Gimbal_Pos_Yaw.SetPoint += (rc_ctrl.rc.ch0 - 1024) * 0.00010f;
	}
	else if(rc_ctrl.rc.s1 == 3)//云台锁定。此时pid要变成最稳定配置
	{
/***************************云台锁定****************************/
		Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
		Pid_Gimbal_Pos_Pitch.SetPoint = RealPos[0];
/***************************履带正反向变速****************************/
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
/***************************推弹****************************/
		if(rc_ctrl.rc.ch1 - 1024 > 500)
		{
			PushToTrack = 1;
		}
		else
		{
			PushToTrack = 0;
		}
/*****************************换弹************************/
		if(rc_ctrl_last.rc.ch1 - 1024 <= 500 && rc_ctrl.rc.ch1-1024 > 500)
		{
			Reload_Task();
		}
	}
}

/**
  * @brief  键鼠模式
  * @param  None
  * @retval None
  */
void Mouse_Key_Control(void)
{
/*******************************云台两轴控制*****************************/
	if(Gimbal_Lock_Flag == 0)//未锁定云台,自由控制
	{
		Pid_Gimbal_Pos_Yaw.SetPoint += rc_ctrl.mouse.x * 50.0f;
		Pid_Gimbal_Pos_Pitch.SetPoint += rc_ctrl.mouse.y * 50.0f;
	}
	else if(Gimbal_Lock_Flag == 1)
	{
		Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
		Pid_Gimbal_Pos_Pitch.SetPoint = RealPos[0];
	}
/********************************云台锁定切换***************************/
	if(rc_ctrl.key.v == 1)//按下v
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
	if(Vcnt == 700)//等于700ms
	{
		Gimbal_Lock_Flag = 1 - Gimbal_Lock_Flag;//切换云台锁定状态
	}
/*********************************推弹****************************/
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
/*********************************换弹****************************/
	if(rc_ctrl.key.r == 1)//按下v
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
	if(Rcnt == 300)//等于300ms
	{
		Reload_Task();
	}
/*********************************履带正转定速****************************/
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
  * @brief  掉电模式
  * @param  None
  * @retval None
  */
void Power_Off(void)
{
	Gimbal_Power_Flag = 0;//云台0电流
	Solenoid_Power_Flag = 0;//电磁阀失能
//	PAout(2) = 0;//光电管失能?有必要失能吗？万一产生电平变化？
	Motor_Speed_Set(0,1);//履带停转
}
/**
  * @brief  模式执行
  * @param  Sta：模式状态
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
  * @brief  发射任务
  * @param  State：任务状态
  * @retval None
  */
void Lauch_State_Convert(Lauching_States State)
{
	switch(State)
	{
		case L_State0://电机提速
			Pid_Speed.SetPoint = ShootSpeed1;
			delay_ms(2000);//提速完成
		break;
		
		case L_State1://气缸将底座推进履带
			PushToTrack = 1;
		break;
		
		case L_State2://测速光电管1检测到发射完成后，电机低速反转
			PushToTrack = 0;
			Motor_Speed_Set(1500,0);
			delay_ms(3000);
		break;
		
		case L_State3://换弹，任务相关标志重置
		break;
		
		default:
		break;
	}
}
/**
  * @brief  断线检测
  * @param  Discnect_Flag:断线结构体
  * @retval 返回断线id
  */
u32 Offline_Detect(Disconnect_Flag *Discnect_Flag)
{
	u32 Discon_Flag;
/**************CAN1断线检测*******************/
	if(Discnect_Flag->Can1_Connect == 0)
	{
		Discnect_Flag->Can1_Cnt ++;
		Discon_Flag &= 0xfffe;//实时断线标志位清零
	}
	if(Discnect_Flag->Can1_Cnt > MS)
	{
		Discnect_Flag->Can1_Disonnect_Ever = 1;
		Discnect_Flag->Can1_Connect = 1;
		Discon_Flag |= 0x0001;
	}
/**************CAN2断线检测*******************/
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
/**************遥控器断线检测*******************/
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
/**************陀螺仪断线检测*******************/
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
/**************云台断线检测*******************/
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
/**************Usart3断线检测*******************/
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
