#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct PID
{
	float SetPoint;	
	float SetPointLast;
	
	float P;				
	float I;				
	float D;				
	
	float LastError;		
	float PreError;			
	float SumError;			
	float dError;

	float IMax;				
	
	float POut;				
	float IOut;				
	float DOut;				
	
}PID;			



float PID_Calc(PID *P,float ActualValue);
void Pid_Init(void);

#endif
