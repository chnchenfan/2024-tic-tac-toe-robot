#ifndef _MOTOR_PID_H_
#define _MOTOR_PID_H_

#include "sys.h"




struct _Speed_Pid{
	
float Kp_lift;
float Kp_right;
float Ki_lift;
float Ki_right;

float Kd_lift;
float Kd_right;

int error_now_lift;
int error_now_right;
	
int error_last_lift;
int error_last_right;
	

int error_iteg_lift;
int error_iteg_right;
	
int Out_lift;
int Out_right;



};



void Speed_Pid_init(void);
int Speed_Pid_right(int exp_spd,int exp_now);
int Speed_Pid_lift(int exp_spd,int exp_now);




#endif







