#ifndef __SOLENOID_H
#define __SOLENOID_H

void Reload_Task(void);
void PullWork_Task(void);
void Recharge_Task(void);
void CheckAutoCtrl(void);
void Flag_Initialize(void);
int Range_In(float Range,float Actual,float Target);

#endif
