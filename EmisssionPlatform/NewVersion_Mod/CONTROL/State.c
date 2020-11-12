#include "main.h"
u8 First_Enter=1;
u8 Gimbal_Lock_Flag;
u16 Vcnt;
u16 Rcnt;
u16 Mousecnt;
int Pitch_PulseSet;//0~14400对应占空比
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
  * @brief  模块标志清零
  * @param  None
  * @retval None
  */
/**
  * @brief  摇杆模式
  * @param  None
  * @retval None
  */
void Rc_Control(void)
{
	if(!AutoCtrl_Flag)//非自动控制模式
	{
		Gimbal_Power_Flag = 1;//云台给电流
/*----------------------------------遥控器------------------------------------*/
		if(rc_ctrl.rc.s1 == 1)//发射前任务
		{
		/*---------------------------各电机位置调整-------------------------------*/
			Pitch_PulseSet += (rc_ctrl.rc.ch3 - 1024) * 0.03f;
			Pid_Gimbal_Pos_Yaw.SetPoint += -(rc_ctrl.rc.ch2 - 1024) * 0.05f; 
			Pid_Gimbal_Pos_Recharge.SetPoint += (rc_ctrl.rc.ch1 - 1024) * 0.3f;
			Pid_Gimbal_Pos_PushPull.SetPoint += (rc_ctrl.rc.ch0 - 1024) * 0.10f;
			Pitch_PulseSet=LIMIT_MAX_MIN(Pitch_PulseSet,14000,400);//调节限幅
			TIM_SetCompare1(TIM3,Pitch_PulseSet);//调节占空比，改变pitch位置
			
		/*---------------------------发射变量计数调整-----------------------------*/
					Triggercnt = 0;
					Out_Or_Back=0;//光电管1所有计数清零

			First_Enter=1;//可以通过摇杆s1重复调整
			
		}
		
		else if(rc_ctrl.rc.s1 == 3)//云台锁定。此时pid要变成最稳定配置
		{
			if(First_Enter)
			{
				PushPullPosInit=RealPos[0];//记录拉线电机确定好的位置
				PutterPosInit=RealPos[1];//记录换弹电机确定好的位置
				
				First_Enter=0;
			}
/*--------------------------云台锁定---------------------------*/
			Pid_Gimbal_Pos_PushPull.SetPoint = RealPos[0];
			Pid_Gimbal_Pos_Recharge.SetPoint=RealPos[1];
			Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[2];


/*----------------------------放弹-----------------------------*/
			if(rc_ctrl.rc.ch1 - 1024 > 500)
			{
				TIM_SetCompare1(TIM5,25);//舵机转90度位置,放弹
//			PushToTrack = 1;
			}
			else
			{
				TIM_SetCompare1(TIM5,15);//恢复0度位置
//			PushToTrack = 0;
			}
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
//	if(!AutoCtrl_Flag)//非自动控制模式
//	{
//		Gimbal_Power_Flag = 1;
///*******************************云台两轴控制*****************************/
//		if(Gimbal_Lock_Flag == 0)//未锁定云台,自由控制
//		{
//			Pid_Gimbal_Pos_Yaw.SetPoint += rc_ctrl.mouse.x * 50.0f;
//			 .SetPoint += rc_ctrl.mouse.y * 50.0f;
//		}
//		else if(Gimbal_Lock_Flag == 1)
//		{
//			Pid_Gimbal_Pos_Yaw.SetPoint = RealPos[1];
//			 .SetPoint = RealPos[0];
//		}
///********************************云台锁定切换***************************/
//		if(rc_ctrl.key.v == 1)//按下v
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
//		if(Vcnt == 700)//等于700ms
//		{
//			Gimbal_Lock_Flag = 1 - Gimbal_Lock_Flag;//切换云台锁定状态
//		}
///*********************************推弹****************************/
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
///*********************************换弹****************************/
//		if(rc_ctrl.key.r == 1)//按下v
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
//		if(Rcnt == 300)//等于300ms
//		{
//			Recharge_Task();
//		}
///*********************************履带正转定速****************************/
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
  * @brief  掉电模式
  * @param  None
  * @retval None
  */
void Power_Off(void)
{
	Gimbal_Power_Flag = 0;//云台0电流
	Solenoid_Power_Flag = 0;//电磁阀失能
//	PAout(2) = 0;//光电管失能?有必要失能吗？万一产生电平变化？
//	Motor_Speed_Set(0,1);//履带停转
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
  * @brief  发射任务
  * @param  State：任务状态
  * @retval None
  */

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
		Discon_Flag &= 0xfffe;//实时断线标志位清零1111 1111 1111 1110
	}
	if(Discnect_Flag->Can1_Cnt > MS)//断线时间>0.1s
	{
		Discnect_Flag->Can1_Disonnect_Ever = 1;
		Discnect_Flag->Can1_Connect = 1;
		Discon_Flag |= 0x0001;
	}
/**************CAN2断线检测*******************/
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
/**************遥控器断线检测*******************/
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
/**************陀螺仪断线检测*******************/
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
/**************云台断线检测*******************/
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
/**************Usart3断线检测*******************/
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
