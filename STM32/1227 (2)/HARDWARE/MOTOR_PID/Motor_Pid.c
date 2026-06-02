#include "Motor_Pid.h"
#include "Motor_Drive.h"
#include "Motor_Encoder.h"
#include "usart.h"


struct _Speed_Pid Speed_Pid;

void Speed_Pid_init(void)
{
	
	Speed_Pid.Kp_lift = -150;
	Speed_Pid.Ki_lift = 0;
	Speed_Pid.Kd_lift = -50;
	



	
	
	Speed_Pid.Kd_right = -19;
	Speed_Pid.Kp_right = -110;
	Speed_Pid.Ki_right = -2;
	



}

int Speed_Pid_right(int exp_spd,int exp_now)
{

	
	Speed_Pid.error_now_right = exp_now - exp_spd;
	Speed_Pid.error_iteg_right += Speed_Pid.error_now_right;
	Speed_Pid.Out_right = Speed_Pid.Kp_right*Speed_Pid.error_now_right + Speed_Pid.Ki_right*Speed_Pid.error_iteg_right+ Speed_Pid.Kd_right*(Speed_Pid.error_now_right -Speed_Pid.error_last_right);
	Speed_Pid.error_last_right = Speed_Pid.error_now_right;
	
	if(Speed_Pid.Out_right>=1000) Speed_Pid.Out_right=1000;
	if(Speed_Pid.Out_right<=-1000) Speed_Pid.Out_right=-1000;
	
	
	return (int)Speed_Pid.Out_right;
	
}





int Speed_Pid_lift(int exp_spd,int exp_now)
{
	

	
	
	Speed_Pid.error_now_lift = exp_now - exp_spd;

	Speed_Pid.error_iteg_lift += Speed_Pid.error_now_lift;
	Speed_Pid.Out_lift = Speed_Pid.Kp_lift*Speed_Pid.error_now_lift + Speed_Pid.Ki_lift*Speed_Pid.error_iteg_lift + Speed_Pid.Kd_lift*(Speed_Pid.error_now_lift -Speed_Pid.error_last_lift);
	Speed_Pid.error_last_lift = Speed_Pid.error_now_lift;
	
	if(Speed_Pid.Out_lift>=1000) Speed_Pid.Out_lift=1000;
	if(Speed_Pid.Out_lift<=-1000) Speed_Pid.Out_lift=-1000;
	
	return (int)Speed_Pid.Out_lift;
	
}



