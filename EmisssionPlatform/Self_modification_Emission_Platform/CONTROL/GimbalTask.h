#ifndef __GIMBALTASK_H
#define __GIMBALTASK_H
//10.21Pos1�ⶨ�ȽϺ���
#define Pos1 -1000000
#define Pos2 0
#define Pos3 1000000
#define Pos4 2000000

void Pid_Gimbal_Cal_Rc(void);
void Pid_Gimbal_Pos_Init(void);
void Pid_Gimbal_Speed_Init(void);
void Recharge_Pos_Move(int position);

#endif