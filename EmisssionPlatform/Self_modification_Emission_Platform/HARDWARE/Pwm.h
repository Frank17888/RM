#ifndef __PWM_H__
#define __PWM_H__

void Motor_Config(void);
void Motor_Speed_Set(int speed,int polar);
void Motor_Putter_Config(void);
void Putter_Move_Updown(int updown);

#endif
