#include "main.h"

/*--------------------------内部变量----------------------------*/
ZeroCheck_Typedef ZeroCheck_Motor[MotorNum];

int Motor_Angle[MotorNum];
float Temp[MotorNum],Hold[MotorNum];
/*--------------------------外部引用变量----------------------------*/
extern rmc620_t Gimbal_Motor[MotorNum];
extern float RealPos[MotorNum];
extern float LastPos[MotorNum];
/*--------------------------函数部分----------------------------*/

/**
  * @brief  位置式和速度式过零检测
	           Zero->ActualValue 表示检测量当前值
						 Zero->LastValue 表示检测量上一次值
						 Zero->CountCycle 表示检测量过零时越变值，即计数周期
						 Zero->PreError 表示检测量差值
						 使用此函数前要申明对应检测量结构体的 Zero->CountCycle与Zero->LastValue
  * @param  ZeroCheck_Typedef *Zero  过零检测结构体
  *         float value  待检测量
            short Zerocheck_mode：取值Position或Speed
  * @retval 取决于Zerocheck_mode，分别输出过零检测后位置值或速度值
  */
float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
{
	Zero->ActualValue=value;
	
	Zero->PreError=Zero->ActualValue-Zero->LastValue;
	Zero->LastValue=Zero->ActualValue;
	
	if(Zero->PreError>0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError-Zero->CountCycle;
		Zero->Circle++;
	}
	if(Zero->PreError<-0.7f*Zero->CountCycle)
	{
		Zero->PreError=Zero->PreError+Zero->CountCycle;
		Zero->Circle--;
	}
	
	if(Zerocheck_mode==Position)
		return Zero->ActualValue - Zero->Circle*Zero->CountCycle;
	else if(Zerocheck_mode==Speed)
	  return Zero->PreError;
	else 
		return 0;
}


/**
  * @brief  过零检测执行函数
  * @param  None
  * @retval None
  */
void ZeroCheck_cal(void)
{
	short i;
	for(i=0;i<MotorNum;i++)
	{
		Hold[i] = (float)Gimbal_Motor[i].angle;
		Temp[i] = ZeroCheck(&ZeroCheck_Motor[i],Hold[i],Position);
		Motor_Angle[i]=(int)Temp[i];
	}

}


/**
  * @brief  过零检测结构体参数初始化
  * @param  None
  * @retval None
  */
void ZeroCheck_Init(void)
{
	short i;
	for(i=0;i<MotorNum;i++)
	{
		ZeroCheck_Motor[i].CountCycle=8192;
		ZeroCheck_Motor[i].LastValue=Gimbal_Motor[i].angle;
		ZeroCheck_Motor[i].Circle=0;
	}

}

/**
  * @brief  底盘过零检测值
  * @param  None
  * @retval 底盘角度过零检测后输出值
  */

int ZeroCheck_Motor_PosOutput(short i)
{
	switch(i)
	{
		case 0:
			return Motor_Angle[0];
		case 1:
			return Motor_Angle[1];
		default:
			break;
	}
	return 0;
}
/**
  * @brief  过零检测浮点输出
  * @param  None
  * @retval None
  */
void ZeroCheck_Excution(void)
{
	int i = 0;
	for(i = 0;i < MotorNum;i ++)
	{
		LastPos[i] = RealPos[i];
		RealPos[i] = (float)ZeroCheck_Motor_PosOutput(i);//读取当前过零检测后位置
	}
}
