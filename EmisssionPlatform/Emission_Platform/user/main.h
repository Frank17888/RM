#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "math.h"
#include "string.h"

#include "Delay.h"
#include "Led.h"
#include "nvic.h"
#include "Can.h"
#include "Pwm.h"
#include "Usart.h"
#include "Remote.h"
#include "Dma.h"
#include "Gpio.h"
#include "Tim.h"
#include "Solenoid.h"

#include "State.h"
#include "Pid.h"
#include "Motor_Task.h"
#include "GimbalTask.h"
#include "ZeroCheck.h"

#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#define Current_Max 6000
#define MS 100
#define MotorNum 2
//////////////////////////////////

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n) 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n) 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n) 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n) 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n) 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n) 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n) 

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n) 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  

///////////////////////////

#define PushToTrack PAout(4) //推弹气缸
#define LockMissle	PAout(5)=1//发射锁锁上
#define UnlockMissle	PAout(5)=0//发射锁解开
#define ReceiveLock	PAout(6)=0//接收锁锁上
#define ReceiveUnlock	PAout(6)=1//接收锁打解开


typedef struct
{
  s16 angle;					//角度返回值
	s16 last_angle;				//前一次角度返回值
	s16 cycle;					//圈数
  s16 speed;					//实时速度
	s16 flow;					//实时电流
	s16 temperature;			//实时温度
	s32 real_posion;			//实际位置
}rmc620_t;
typedef struct
{
	unsigned short angle;  //0~8191(0x1fff)
	short speed;       //     RPM
	short last_angle;
	s16 cycle;
	s16 real_posion;
	short Disconnect;
}rmc610_t;

extern u8 us10_on_off;
extern u8 Solenoid_Power_Flag;
extern u8 Gimbal_Power_Flag;
extern Status_ Status;
extern Disconnect_Flag Discnect_Flag;
extern rc_ctrl_t rc_ctrl, rc_ctrl_last;
extern Lauching_States LauchState;
extern PID	Pid_Gimbal_Pos_Yaw,Pid_Gimbal_Pos_Pitch;

void SysTick_Configuration(void);
u32 GetSyscnt(void);
u32 Get_u32_nth(u32 num32,u8 n);

#endif
