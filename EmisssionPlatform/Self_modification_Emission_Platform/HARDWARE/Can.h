#ifndef __CAN_H
#define __CAN_H

#include "main.h"

void CAN1_Config(void);
void Gimbal_Current_Send(int a, int b, int c, int d);
float Int_To_Float(int Inte);
void CAN2_Config(void);
int Float_To_Int(float Floa);

#endif
