#ifndef  _MOTO_H
#define  _MOTO_H
#include "sys.h"

#define PWM_L_F   TIM3->CCR4
#define PWM_L_B   TIM3->CCR3
#define PWM_R_F   TIM3->CCR2
#define PWM_R_B   TIM3->CCR1

#define L_Ain1  PCout(2)  
#define L_Ain2  PCout(3)

#define L_Bin1  PCout(1)  
#define L_Bin2  PCout(0)

#define R_Ain1  PFout(8)  
#define R_Ain2  PFout(7)

#define R_Bin1  PFout(9)  
#define R_Bin2  PFout(10)


void PWM_Init_TIM(u16 arr, u16 psc);
void Motor_Init(void);

#endif
