#include "main.h"
u8 Solenoid_Power_Flag = 1;
u8 FirstTime = 1;
u8 Recharge_1st=1,Recharge_2nd=1,Recharge_3rd=1;//标志是在换上第几个飞镖
u8 Recharge_1st_Get=0,Recharge_2nd_Get=0,Recharge_3rd_Get=0;
u8 RechargeWork_Flag ;//标志换弹开始结束，换弹结束是0，可以开始是1

u8 PullWork_Flag=0;//拉线电机工作标志位，0：不动；1：倒回
u8 PushWork_Flag=0;//推线，同上
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
  * @brief  电磁阀配置
  * @param  None
  * @retval None
  */
/**
  * @brief  倒回动作
  * @param  None
  * @retval None
  */
void PullWork_Task()
{
	if(MissileUp_Flag)			//飞镖射出
	{
		if(PullWork_Flag)			//拉线电机开始工作
		{
			PushPull_Pos_Move(PullPos);
			if(!RealSpeed[0])	//电机拉到底，停转再反转
			{
				PullWork_Flag=0;					//停止拉线
				PushWork_Flag=1;					//开始推线
				RechargeWork_Flag=1;			//可以开始换弹
			}
		}//向回拉线
		
		else if(PushWork_Flag)
		{
			PushPull_Pos_Move(0);//拉线电机恢复位置
			if(Range_In(1000,RealPos[0],PushPullPosInit+PullPos))	//PushPullPosInit 拉线电机初始化值
			{
				PullWork_Flag=1;//恢复拉线状态
				PushWork_Flag=0;//停止推线
				MissileUp_Flag=0;//倒回事件处理完成
			}
		}
		
	}
}



/**
  * @brief  换弹动作
  * @param  None
  * @retval None
  */
void Recharge_Task(void)
{
	if(RechargeWork_Flag)
	{	
		if(Recharge_1st)//第一次换弹是单程，后两次是往返双程。
		{
			Recharge_Pos_Move(Pos1);//换弹电机位置环直接向下移动
			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//现在位置和目标位置角度相差很小，说明换弹完成
			{
				RechargeWork_Flag = 0;//换弹停止工作
				Recharge_1st = 0;
//				AutoCtrl_Flag=0;
			}
		}
		else if(Recharge_2nd)
		{
			if(Recharge_2nd_Get == 0)//没有移动到指定位置
			{
				Recharge_Pos_Move(Pos3);
				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//位置移动完成返回1
			}
			else
			{
				if(PutterDelay<PutterDelayTime)//推杆电机还没放下飞镖
				{
					PutterDelay++;
					Putter_Move_Updown(0);
				}
				else
				{
					PutterDelay=0;
					Recharge_Pos_Move(Pos1);//取弹手归位
					Putter_Move_Updown(1);
					if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
					{
						Recharge_2nd = 0;
						RechargeWork_Flag = 0;//换弹停止工作
					}
				}
			}
		}
		
		else if(Recharge_3rd)
		{
			if(Recharge_3rd_Get == 0)
			{
				Recharge_Pos_Move(Pos4);
				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);//位置移动完成返回1
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
					if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
					{
						Recharge_3rd = 0;
						RechargeWork_Flag = 0;//换弹停止工作
					}
				}
			}
		}
	}
	
	
	
	
	
	
	
//	void Recharge_Task(void)
//{
//	if(RechargeWork_Flag)
//	{	
//		if(Recharge_1st)//第一次换弹是单程，后两次是往返双程。
//		{
//			Recharge_Pos_Move(Pos1);//换弹电机位置环直接向下移动
//			if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//现在位置和目标位置角度相差很小，说明换弹完成
//			{
//				RechargeWork_Flag = 0;//换弹停止工作
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
//				Recharge_2nd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos3);//位置移动完成返回1
//			}
//			else
//			{
//				Recharge_Pos_Move(Pos1);
//				Putter_Move_Updown(1);
//				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
//				{
//					Recharge_2nd = 0;
//					RechargeWork_Flag = 0;//换弹停止工作
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
//				Recharge_3rd_Get = Range_In(1000,RealPos[0],PutterPosInit+Pos4);//位置移动完成返回1
//			}
//			else
//			{
//				Recharge_Pos_Move(Pos1);
//				Putter_Move_Updown(1);
//				if(Range_In(1000,RealPos[0],PutterPosInit+Pos1))//电机移动换弹完成
//				{
//					Recharge_3rd = 0;
//					RechargeWork_Flag = 0;//换弹停止工作
//				}
//			}
//		}
//	}
//}
}



void CheckAutoCtrl()
{
	AutoCtrl_Flag=MissileUp_Flag||RechargeWork_Flag;//发射事件或换弹事件未结束，不受遥控器控制
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
  * @brief  范围判定函数
	* @param  Range:单边范围长度
	*					Actual：实际值
	*					Target:目标值
	* @retval 1:进入范围;0:不在范围内
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

