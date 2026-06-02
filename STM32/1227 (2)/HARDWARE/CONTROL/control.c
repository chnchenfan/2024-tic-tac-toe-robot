#include "control.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "moto.h"
#include "beep.h"
#include "servo.h"
#include "Motor_Encoder.h"
#include "hc_sr04.h"
#include "bluetooth.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "xinhao.h"
#include "Hall_Sensor.h"
#include "YL_Sensor.h"


#include <stdio.h>
#include <math.h>
#include <string.h>


float datas[10];
float B_P, B_I, B_D;
float V_P, V_I, V_D;
float A_P, A_I, A_D;
float X_P, X_I, X_D;
float D_P, D_I, D_D;
float P_P, P_I, P_D;

int Velocity_Left_F_Pwm, Velocity_B_Pwm, Velocity_Right_F_Pwm;
int Trail_Left_Pwm, Trail_Right_Pwm;
int Angle_Left_Pwm, Angle_Right_Pwm;
int Balance_Pwm;
int Distance_Pwm;
int PWM_P, PWM_V;
struct Parameter_pid ALL_pid;





void ALL_Parameter(void)
{
    
    ALL_pid.Velocity_Kp = 0;
    ALL_pid.Velocity_Ki = 0;
    
    ALL_pid.Trail_Kp = 0;
    ALL_pid.Trail_Ki = 0;
    ALL_pid.Trail_Kd = 0;
    
    ALL_pid.Balance_Kp = 0;
    ALL_pid.Balance_Kd = 0;
    
    ALL_pid.angle_Kp = 100;
    ALL_pid.angle_Ki = 0.2;
    ALL_pid.angle_Kd = 250;
	    
	
    ALL_pid.Velcity_Kp2 = 0;
    ALL_pid.Velcity_Ki2 = 0;
		ALL_pid.Velcity_Kd2 = 0;
    
    ALL_pid.Position_Kp = 0;
    ALL_pid.Position_Ki = 0;
    ALL_pid.Position_Kd = 0;









		
		ALL_pid.distance_Kp = 0;
		ALL_pid.distance_Ki = 0;
		ALL_pid.distance_Kd = 0;
}





void send_wave(void)
{
    
    u8 frameNameHead[] = "AABBCC";
    u8 frameNameEnd[] = "CCBBAA";

    
    u8 frameDataHead[] = "DDEEFF";
    u8 frameDataEnd[] = "FFEEDD";

    
    u8 name[] = {"V_err,Yaw,OPENMV_Receive,distance,CurrentPosition,CurrentVelocity,T_err,PWM_P,V_eer"};

    
    float channels[9];
    channels[0] = datas[0];
    channels[1] = datas[1];
    channels[2] = datas[2];
    channels[3] = datas[3];
		channels[4] = datas[4];
		channels[5] = datas[5];
		channels[6] = datas[6];
		channels[7] = datas[7];
		channels[8] = datas[8];

    
    usart_senddatas(USART3, frameNameHead, sizeof(frameNameHead) - 1);
    usart_senddatas(USART3, name, sizeof(name) - 1);
    usart_senddatas(USART3, frameNameEnd, sizeof(frameNameEnd) - 1);

    usart_senddatas(USART3, frameDataHead, sizeof(frameDataHead) - 1);
    usart_senddatas(USART3, (u8 *)channels, sizeof(channels));
    usart_senddatas(USART3, frameDataEnd, sizeof(frameDataEnd) - 1);
}

void getdatas(void)
{
    datas[0] = CurrentVelocity - Target_Velocity;
    datas[1] = Yaw;
    datas[2] = (float)OPENMV_Receive;
		datas[3] = distance;
		datas[4] = CurrentPosition;
		datas[5] = CurrentVelocity;
		datas[6] = T_err;
	  datas[7] = PWM_P;
		datas[8] = V_eer;
}

void get_cmd(void)
{
    char u_buff[10];
    float u_d1, u_d2, u_d3;
    if (usart_get_data(u_buff, &u_d1, &u_d2, &u_d3))
    {
        if (strcmp(u_buff, "PID") == 0) 
        {
            B_P = u_d1;
            B_I = u_d2;
            B_D = u_d3;
            ALL_pid.Balance_Kp = B_P;
            ALL_pid.Balance_Kd = B_D;
        }
        else if (strcmp(u_buff, "VVV") == 0) 
        {
            V_P = u_d1;
            V_I = u_d2;
            V_D = u_d3;
            ALL_pid.Velcity_Kp2 = V_P;
            ALL_pid.Velcity_Ki2 = V_I;
						ALL_pid.Velcity_Kd2 = V_D;
        }
        else if (strcmp(u_buff, "MPU") == 0) 
        {
            A_P = u_d1;
            A_I = u_d2;
            A_D = u_d3;
            ALL_pid.angle_Kp = A_P;
            ALL_pid.angle_Ki = A_I;
            ALL_pid.angle_Kd = A_D;
        }
        else if (strcmp(u_buff, "XUN") == 0) 
        {
            X_P = u_d1;
            X_I = u_d2;
            X_D = u_d3;
            ALL_pid.Trail_Kp = X_P;
            ALL_pid.Trail_Ki = X_I;
            ALL_pid.Trail_Kd = X_D;
            if (X_P == 0)
                Velocity_B_Pwm = 0;
            else
                Velocity_B_Pwm = 7000;
        }
				else if (strcmp(u_buff, "DIS") == 0) 
        {
            D_P = u_d1;
            D_I = u_d2;
            D_D = u_d3;
            ALL_pid.distance_Kp = D_P;
            ALL_pid.distance_Ki = D_I;
            ALL_pid.distance_Kd = D_D;
        }
				else if (strcmp(u_buff, "POS") == 0) 
        {
            P_P = u_d1;
            P_I = u_d2;
            P_D = u_d3;
            ALL_pid.Position_Kp = P_P;
            ALL_pid.Position_Ki = P_I;
            ALL_pid.Position_Kd = P_D;
        }
    }
    memset(u_buff, 0, sizeof(u_buff));
}






void OLED_ShowAll(void)
{
    OLED_Refresh();
    OLED_ShowString(0, 12, "MODE:", 12);
    OLED_ShowNum(30, 12, key_mode, 1, 12);
    OLED_ShowString(46, 12, "KEY2:", 12);
    OLED_ShowNum(76, 12, key2_num, 1, 12);
    OLED_ShowString(92, 12, "KEY3:", 12);
    OLED_ShowNum(122, 12, key3_num, 1, 12);
    OLED_ShowString(0, 24, "DIS:", 12);
    OLED_ShowNum(30, 24, distance, 5, 12);
    OLED_ShowString(0, 36, "OPMV:", 12);
    OLED_ShowNum(30, 36, OPENMV_Receive, 3, 12);
    OLED_ShowString(54, 36, "HZ:", 12);
    OLED_ShowNum(78, 36, num, 3, 12);
}





void Test_function(void)
{

















































    
		
		
		















		















		



		














































			
			
			
















			LED_lsd();
			OLED_Refresh();
			sprintf((char *)OLED_buff," Intellgent Car");
			OLED_ShowString(0,0,(uint8_t *)OLED_buff,12);
			delay_ms(1000);
			OLED_Refresh();
			sprintf((char *)OLED_buff,"Init Waittng..");
			OLED_ShowString(0,24,(uint8_t *)OLED_buff,12);
			delay_ms(1000);
			OLED_Clear();
			BEEP_ON();
			delay_ms(1000);
			BEEP_OFF();
			delay_ms(1000);
			MUSIC1_Play();
			delay_ms(1000);
			
			


































		
		
}

void position(float tar_x,float tar_y,float tar_z)
{
	float t1,t2,t3;
	int num1,num2,num3;
	if(tar_x!=0)
	{
		num1 = 1;
	}
		if(tar_y!=0)
	{
		num2 = 1;
	}
		if(tar_z!=0)
	{
		num3 = 1;
	}
	t1 = tar_x/( 3.9/2000);
	t2 = tar_y/( 0.8/2000);
	t3 = tar_z/( 2.5/2000);
	float  min_value = (t1 < t2) ? ((t1 < t3) ? t1 : t3) : ((t2 < t3) ? t2 : t3);
	if(min_value==t1)
	{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t1);
		num1=0;
		if(t2<t3)
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t2-t1);
		num2=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t3-t2);
		num3=0;
		}
		else
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t3-t1);
		num3=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t2-t3);
		num2=0;
		}
		
	}
	else if(min_value==t2)
	{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t2);
		num2=0;
		if(t1<t3)
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t1-t2);
		num1=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t3-t1);
		num3=0;
		}
		else
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t3-t2);
		num3=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t1-t3);
		num1=0;
		}
	}
	else
	{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t3);
		num3=0;	
		if(t1<t2)
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t1-t3);
		num1=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t2-t1);
		num2=0;
		}
		else
		{
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t2-t3);
		num2=0;
		Set_Pwm(-4000*num1,-4000*num1,-4000*num2,-4000*num3);
		delay_ms(t1-t2);
		num1=0;
		}		
	}
  
}



void mode1_control()
{
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
						MMGG4 = 1;
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);

	
}

void mode2_control()
{
	if(heibai_flage<2)
	{
			if(HAOMA1==0&&haoma_flag1==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(9959);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(9959);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag1++;
	}
			else if(HAOMA2==0&&haoma_flag2==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(11720);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(11720);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag2++;
	}
			else if(HAOMA3==0&&haoma_flag3==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(13280);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
			MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(13280);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag3++;
	}
			else if(HAOMA4==0&&haoma_flag4==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(2359);		
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(2359);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag4++;
	}
			else if(HAOMA5==0&&haoma_flag5==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);




						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);
							MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag5++;
	}
			else if(HAOMA6==0&&haoma_flag6==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(5680);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(5680);	
							MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag6++;
	}
			else if(HAOMA7==0&&haoma_flag7==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(5891);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 0;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(5891);
			MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag7++;
	}
			else if(HAOMA8==0&&haoma_flag8==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(4130);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 0;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(4130);
			MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag8++;
	}
			else if(HAOMA9==0&&haoma_flag9==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 1;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(2570);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
				MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 0;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(2570);
									MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		Set_Pwm(0,0,0,0);
						heibai_flage++;
						haoma_flag9++;
	}
	else
	{
		Set_Pwm(0,0,0,0);
		LED1=0;
		LED2=0;
		LED3=0;
		LED4=0;
		delay_ms(200);
		LED1=1;
		LED2=1;
		LED3=1;
		LED4=1;	
		delay_ms(200);		
	}
	}
	else
	{
				if(HAOMA1==0&&haoma_flag1==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(13280);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
			MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(13280);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag1++;
	}
			else if(HAOMA2==0&&haoma_flag2==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(11720);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(11720);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag2++;
	}
			else if(HAOMA3==0&&haoma_flag3==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(9959);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(9959);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag3++;
	}
			else if(HAOMA4==0&&haoma_flag4==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(5680);	
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(2570);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(5680);	
							MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag4++;
	}
			else if(HAOMA5==0&&haoma_flag5==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);




						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(4130);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(4120);
							MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
Set_Pwm(0,0,0,0);						
		heibai_flage++;
		haoma_flag5++;
	}
			else if(HAOMA6==0&&haoma_flag6==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(2359);		
												MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,-4000,0);
						delay_ms(5891);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(2359);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
		heibai_flage++;
		haoma_flag6++;
	}
			else if(HAOMA7==0&&haoma_flag7==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(2570);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 1;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(2570);
			MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag7++;
	}
			else if(HAOMA8==0&&haoma_flag8==0)
	{		
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(4130);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 1;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(4130);
			MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		heibai_flage++;
		haoma_flag8++;
	}
			else if(HAOMA9==0&&haoma_flag9==0)
	{
						xitie = 1;
		Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG3 = 0;
								MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,-4000,0);
						delay_ms(5891);
														MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
		Set_Pwm(0,0,0,0);
						delay_ms(1000);
		MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG3 = 1;					
						Set_Pwm(0,0,-4000,0);
						delay_ms(5891);
			MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
		Set_Pwm(0,0,0,0);
		heibai_flage++;
		haoma_flag9++;
	}
	else
	{
		Set_Pwm(0,0,0,0);
		LED1=0;
		LED2=0;
		LED3=0;
		LED4=0;
		delay_ms(200);
		LED1=1;
		LED2=1;
		LED3=1;
		LED4=1;		
		delay_ms(200);
	}
	}

}
void mode3_control_h(int x_3,int y_3)
{
				
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,0,0);
						dss_4_key++;
						haomabu =0;
	
}
void mode3_control_h_t(int x_3,int y_3)
{
				
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;									
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(0,0,0,0);
	
}

void mode3_control_b(int x_3,int y_3)
{
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 0;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);		
						ess_5_key++;						
	
}
void mode3_control_b_t(int x_3,int y_3)
{
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(2000);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);	
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);
						MMGG1 = 0;
						MMGG2 = 0;
						MMGG3 = 1;									
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(x_3);
						Set_Pwm(0,0,-4000,0);
						delay_ms(y_3);		
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(1500);		
						Set_Pwm(0,0,0,0);				
	
}
void mode4_control(int x_3,int y_3)
{
	
}





void mode6_control()
{
	
	
}





void Go(void)
{
		TIM_Cmd(TIM7, DISABLE);
    Set_Pwm(2000, 2000, 2000, 2000);
    delay_ms(330);
    TIM_Cmd(TIM7, ENABLE); 
}
void Turn_Left(void)
{
		TIM_Cmd(TIM7, DISABLE);
    Set_Pwm(-1000, -1000, 3500, 3500);
    delay_ms(330);
    TIM_Cmd(TIM7, ENABLE); 
}
void Turn_Right(void)
{ 
    TIM_Cmd(TIM7, DISABLE);
    Set_Pwm(3500, 3500, -1000, -1000);
    delay_ms(330);
    TIM_Cmd(TIM7, ENABLE);
}
void Turn_Larg_Left(void)
{
    TIM_Cmd(TIM7, DISABLE);
    Set_Pwm(-1500, -1500, 4000, 4000);
    delay_ms(330);
    TIM_Cmd(TIM7, ENABLE);
}
void Turn_Larg_Right(void)
{
    TIM_Cmd(TIM7, DISABLE);
    Set_Pwm(4000, 4000, -1500, -1500);
    delay_ms(330);
    TIM_Cmd(TIM7, ENABLE);
}
void Stop(void)
{
	TIM_Cmd(TIM7,DISABLE);
  Set_Pwm(0, 0, 0, 0);


}





void Set_Pwm(int moto_left_f, int moto_left_b, int moto_right_b, int moto_right_f)
{
    if (moto_left_f > 0)           L_Bin1 = 0,     L_Bin2 = 1;
    else                           L_Bin1 = 1,     L_Bin2 = 0;
    PWM_L_F = myabs(moto_left_f);
    if (moto_left_b > 0)           L_Ain1 = 1,     L_Ain2 = 0;
    else                           L_Ain1 = 0,     L_Ain2 = 1;
    PWM_L_B = myabs(moto_left_b);
    if (moto_right_f > 0)          R_Ain1 = 0,     R_Ain2 = 1;
    else                           R_Ain1 = 1,     R_Ain2 = 0;
    PWM_R_F = myabs(moto_right_f);
    if (moto_right_b > 0)          R_Bin1 = 1,     R_Bin2 = 0;
    else                           R_Bin1 = 0,     R_Bin2 = 1;
    PWM_R_B = myabs(moto_right_b);
}





void Xianfu_Pwm(int moto_left_f, int moto_left_b, int moto_right_b, int moto_right_f)
{
    int Amplitude = 6000;  
    if (moto_left_f < -Amplitude) moto_left_f = -Amplitude;
    if (moto_left_f > Amplitude)  moto_left_f = Amplitude;
    if (moto_left_b < -Amplitude) moto_left_b = -Amplitude;
    if (moto_left_b > Amplitude)  moto_left_b = Amplitude;
    if (moto_right_b < -Amplitude) moto_right_b = -Amplitude;
    if (moto_right_b > Amplitude)  moto_right_b = Amplitude;
    if (moto_right_f < -Amplitude) moto_right_f = -Amplitude;
    if (moto_right_f > Amplitude)  moto_right_f = Amplitude;
}





int myabs(int a)
{
    int temp;
    if (a < 0)  temp = -a;
    else temp = a;
    return temp;
}






int balance(float Angle)
{
    float Bias;                       
    static float Last_Bias, D_Bias;   
    int balance;                      
    
    Bias = Angle - ZHONGZHI;          
    D_Bias = Bias - Last_Bias;        
    balance = -ALL_pid.Balance_Kp * Bias - D_Bias * ALL_pid.Balance_Kd; 
    Last_Bias = Bias;                 
    return balance;
}





void velocity(void)
{
    static float Encoder_left_f_err, Encoder_left_f_Integral;
    static float Encoder_Least, Encoder, Encoder_Integral;
    static float Encoder_right_f_err, Encoder_right_f_Integral;
    
    Velocity_cm = ((encoder_left_b + encoder_right_b) * 4.8 * PI) / (2 * 13 * 0.005 * 20 * 4  );  
    
    Encoder_left_f_err = CurrentVelocity - Target_Velocity;                        
    Encoder_left_f_Integral += Encoder_left_f_err;                                
    if (Encoder_left_f_Integral > 8000)    Encoder_left_f_Integral = 8000;      
    if (Encoder_left_f_Integral < -8000)   Encoder_left_f_Integral = -8000;     
    Velocity_Left_F_Pwm = Encoder_left_f_err * ALL_pid.Velocity_Kp + Encoder_left_f_Integral * ALL_pid.Velocity_Ki; 
    
    Encoder_Least = CurrentVelocity - Target_Velocity;                        
    Encoder *= 0.7f;                                                     
    Encoder += Encoder_Least * 0.3f;                                     
    Encoder_Integral += Encoder;                                
    if (Encoder_Integral > 8000)    Encoder_Integral = 8000;      
    if (Encoder_Integral < -8000)   Encoder_Integral = -8000;     
    Velocity_B_Pwm = Encoder * ALL_pid.Velocity_Kp + Encoder_Integral * ALL_pid.Velocity_Ki; 
    
    Encoder_right_f_err = encoder_right_f - Target_Velocity;                        
    Encoder_right_f_Integral += Encoder_right_f_err;                                
    if (Encoder_right_f_Integral > 8000)    Encoder_right_f_Integral = 8000;      
    if (Encoder_right_f_Integral < -8000)   Encoder_right_f_Integral = -8000;     
    Velocity_Right_F_Pwm = Encoder_right_f_err * ALL_pid.Velocity_Kp + Encoder_right_f_Integral * ALL_pid.Velocity_Ki; 
}





void Line_patrol(void)
{
    static float err, Left_err, Right_err, Left_Line_Integral, Right_Line_Integral, Last_Left_Line_err, Last_Right_Line_err;
    
    err = err_value - OPENMV_Receive;
    if (err > 0)
        Left_err = err, Right_err = 0, Right_Line_Integral = 0;
    else
        Right_err = -err, Left_err = 0, Left_Line_Integral = 0;
    
    Left_Line_Integral = Left_err + Left_Line_Integral;
    if (Left_Line_Integral > 4000)     Left_Line_Integral = 4000;          
    if (Left_Line_Integral < -4000)      Left_Line_Integral = -4000;         
    Trail_Left_Pwm = ALL_pid.Trail_Kp * Left_err + ALL_pid.Trail_Ki * Left_Line_Integral + ALL_pid.Trail_Kd * (Left_err - Last_Left_Line_err);
    Last_Left_Line_err = Left_err;
    
    Right_Line_Integral = Right_err + Right_Line_Integral;
    if (Right_Line_Integral > 4000)    Right_Line_Integral = 4000;          
    if (Right_Line_Integral < -4000)     Right_Line_Integral = -4000;         
    Trail_Right_Pwm = ALL_pid.Trail_Kp * Right_err + ALL_pid.Trail_Ki * Right_Line_Integral + ALL_pid.Trail_Kd * (Right_err - Last_Right_Line_err);
    Last_Right_Line_err = Right_err;
}






void Angle_Ring(int angle)
{
    static float Yaw_err, Yaw_err_Integral, Last_Yaw_err; 
    
    mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
		Angle_Current = Yaw;
    
    Yaw_err = Yaw - angle;                        
    Yaw_err_Integral += Yaw_err;                                
    if (Yaw_err_Integral > 2000)    Yaw_err_Integral = 2000;      
    if (Yaw_err_Integral < -2000)   Yaw_err_Integral = -2000;     
    Angle_Left_Pwm = ALL_pid.angle_Kp * Yaw_err + ALL_pid.angle_Ki * Yaw_err_Integral + ALL_pid.angle_Kd * (Yaw_err - Last_Yaw_err);
		Angle_Right_Pwm = -Angle_Left_Pwm;
    Last_Yaw_err = Yaw_err;
}






void Velocity_FeedbackControl(float CurrentVelocity)
{
    float eer;  
    static float Integral_eer, Last_eer;
    
    V_eer = Target_Velocity - CurrentVelocity; 
		eer = V_eer;
		Integral_eer += eer;                                
    if (Integral_eer > 1000)    Integral_eer = 1000;      
    if (Integral_eer < -1000)   Integral_eer = -1000;     
    PWM_V = ALL_pid.Velcity_Kp2 * eer + ALL_pid.Velcity_Ki2 * Integral_eer + ALL_pid.Velcity_Kd2 * (eer - Last_eer);
    Last_eer = eer;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         






void Position_FeedbackControl(float CurrentPosition)
{
    float Bias;    
    static float Last_bias, Integral_Bias;          
    
    T_err = Target_Position - CurrentPosition; 
		Bias = T_err;
    
    Integral_Bias += Bias;
    if (Integral_Bias > 970) Integral_Bias = 970; 
    if (Integral_Bias < -970) Integral_Bias = -970; 
    PWM_P = ALL_pid.Position_Kp * Bias + ALL_pid.Position_Ki * Integral_Bias + ALL_pid.Position_Kd * (Bias - Last_bias); 
    Last_bias = Bias;
}






int PWM_Restrict(int PWM_P)
{
    if (PWM_P > 5000 ) PWM_P = +5000;
    else if (PWM_P < -5000 ) PWM_P = -5000;
    else PWM_P = PWM_P;
    return PWM_P;
}






void Distance_Contol(int DIS)
{
    static float distance_err, distance_err_Integral, distance_Last_err;
    
    distance_err = DIS - Target_distance;
    
    distance_err_Integral += distance_err;                                
    if (distance_err_Integral > 2000)    distance_err_Integral = 2000;      
    if (distance_err_Integral < -2000)   distance_err_Integral = -2000;     
    Distance_Pwm = distance_err * ALL_pid.distance_Kp + distance_err_Integral * ALL_pid.distance_Ki + (distance_err - distance_Last_err) * ALL_pid.distance_Kd; 
		distance_Last_err = distance_err;
}
