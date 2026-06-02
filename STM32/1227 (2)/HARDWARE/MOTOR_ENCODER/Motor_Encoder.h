#ifndef __MOTOR_ENCODER_H
#define __MOTOR_ENCODER_H

#include "sys.h"

#define ENCODER_PPR         3750.0f     
#define Perimeter           204.1975f     
#define SpeedTime           100.0f              


void Encoder_TIM_Init(void);
int Read_Encoder(int TIMx);
float Motor_SpeedAcquisition(float EncoderNum);

#endif

