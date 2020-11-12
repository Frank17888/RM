#ifndef __GIMBALTASK_H
#define __GIMBALTASK_H
//10.21Pos1测定比较合适
#define Pos1 -1000000
#define Pos2 0
#define Pos3 1000000
#define Pos4 2000000
#define PullPos -1000000
#define PutterDelayTime 500
#define HandlerDelayTime_2nd 2200
#define HandlerDelayTime_3rd 3000
void Pid_Gimbal_Cal_Rc(void);
void Pid_Gimbal_Pos_Init(void);
void Pid_Gimbal_Speed_Init(void);
void Recharge_Pos_Move(int position);
void PushPull_Pos_Move(int position);
#endif
