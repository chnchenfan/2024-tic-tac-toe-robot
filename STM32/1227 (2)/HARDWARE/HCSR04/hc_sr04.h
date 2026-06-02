#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "sys.h"

#define Trig PGout(12)
#define Echo GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13)

extern int distance, hcsr04_flag;

void HCSR04_Init(void);
void Distance_Init(void);
void Distance_get(void);
void TIM6_Int_Init(u16 arr, u16 psc);

#endif 
