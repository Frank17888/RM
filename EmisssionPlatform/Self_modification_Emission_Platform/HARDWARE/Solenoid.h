#ifndef __SOLENOID_H
#define __SOLENOID_H

void Solenoid_Config(void);
void Reload_Task(void);
void Putter_Back(void);
void Recharge_Task(void);
int Range_In(float Range,float Actual,float Target);

#endif
