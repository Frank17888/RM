#include "main.h"
/**
  * @brief  Pid算法
  * @param  P:Pid结构体
	*					ActualValue:控制量实时值
  * @retval 增量式Pid反馈值
  */ 
float PID_Calc(PID *P, float ActualValue)
{
	P->PreError = P->SetPoint - ActualValue;
	P->dError = P->PreError - P->LastError;
	
	P->SetPointLast = P->SetPoint;

	P->SumError += P->PreError;
	P->LastError = P->PreError;
	
	if(P->SumError >= P->IMax)
		P->SumError = P->IMax;
	else if(P->SumError <= -P->IMax)
		P->SumError = -P->IMax;
	
	P->POut = P->P * P->PreError;
	P->IOut = P->I * P->SumError;
	P->DOut = P->D * P->dError;
	
	return P->POut+P->IOut+P->DOut;
}

