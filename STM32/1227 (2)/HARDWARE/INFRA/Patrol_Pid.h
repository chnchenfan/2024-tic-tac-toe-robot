#ifndef _PATROL_PID_H
#define _PATROL_PID_H
#include "sys.h"

#define HL2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4) 
#define HL1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
#define HM 			GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define HR1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define HR2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)



struct _Pid
{

	float Kp;
	float Ki;
	float Kd;
	
	float error;
	float last_error;
	float iteg_error;
	
};


void Patrol_Pid_init(void);
void Patrol_Pid_init1(void);
void Distance_Pid_init(void);
void infra_init(void);
int  Pid_Lin_Patrol(void);
int  Pid_Lin_Patrol1(void);
int Pid_Distance(int Distance_now);



#endif





