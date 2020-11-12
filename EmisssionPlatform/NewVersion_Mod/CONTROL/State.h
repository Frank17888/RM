#ifndef __STATE_H
#define __STATE_H

#include "stm32f10x.h"

typedef enum
{
	L_State0 = 0, L_State1 , L_State2 , L_State3 
}Lauching_States;

typedef struct
{
	u8 Can1_Connect;
	u8 Can1_Disonnect_Ever;
	u32 Can1_Cnt;
	
	u8 Can2_Connect;
	u8 Can2_Disonnect_Ever;
	u32 Can2_Cnt;
	
	u8 Rc_Connect;
	u8 Rc_Disonnect_Ever;
	u32 Rc_Cnt;
	
	u8 Gyro_Connect;
	u8 Gyro_Disonnect_Ever;
	u32 Gyro_Cnt;
	
	u8 Usart1_Connect;
	u8 Usart1_Disonnect_Ever;
	u32 Usart1_Cnt;
	
	u8 Usart2_Connect;
	u8 Usart2_Disonnect_Ever;
	u32 Usart2_Cnt;
	
	u8 Usart3_Connect;
	u8 Usart3_Disonnect_Ever;
	u32 Usart3_Cnt;
	
	u8 Uart4_Connect;
	u8 Uart4_Disonnect_Ever;
	u32 Uart4_Cnt;
	
	u8 Uart5_Connect;
	u8 Uart5_Disonnect_Ever;
	u32 Uart5_Cnt;
	
	u8 Chassis_Connect;
	u8 Chassis_Disonnect_Ever;
	u32 Chassis_Cnt;
	
	u8 Gimbal_Connect;
	u8 Gimbal_Disonnect_Ever;
	u32 Gimbal_Cnt;
	
	u8 Pc_Connect;
	u8 Pc_Disonnect_Ever;
	u32 Pc_Cnt;
}Disconnect_Flag;
typedef enum
{
	State0 = 0, State1 , State2  
}States;
typedef struct
{
	States Rc_States;
}Status_;

#define ShootSpeed1 60 * 1024

u32 Offline_Detect(Disconnect_Flag *Discnect_Flag);
void Rc_Control(void);
void Mouse_Key_Control(void);
void Status_Act(Status_ Sta);

#endif
