#include "Patrol_pid.h"
#include "Motor_Drive.h"
#include "Motor_Pid.h"
#include "delay.h"
#include "Motor_Encoder.h"
#include "usart.h"
#include "JY901.h"
#include "jy901_u2.h"
#include "led.h"
#include "Motor_Pid_Time.h"
#include "led.h"
struct _Pid Patrol_Pid;
struct _Pid Distance_Pid;

int flag2 =0;
unsigned char pwmc[20];
unsigned char pwmot[20];
extern int dis;
extern u8 Res;












extern int L2, L1, M, R2, R1;



void Patrol_Pid_init()
{
	Patrol_Pid.Kd = 15;           
	Patrol_Pid.Ki = -25;                  
	Patrol_Pid.Kp = 90;                  
}

void Patrol_Pid_init1()
{
	Patrol_Pid.Kd = 20;           
	Patrol_Pid.Ki = -25;                  
	Patrol_Pid.Kp = 110;                  
}

void Distance_Pid_init(void)
{
	Distance_Pid.Kd = 5;
	Distance_Pid.Ki = -5;
	Distance_Pid.Kp = 27;
}


void infra_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	Patrol_Pid_init();
	Speed_Pid_init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);			   

}


int  Pid_Lin_Patrol(void)
{
	
	int Pid_Out;
	int total;

	total = L2 + L1 + R2 + R1 + M;











	if(total>3) Patrol_Pid.error =0;
	else 
	{
	
		if((L2 == 0&&L1 == 0&&R1 == 0&&R2 ==0&&M==0)||M==1) Patrol_Pid.error = 0;
		
		
		if(L1==1) Patrol_Pid.error = -5;
		else {
		
			if(L2 ==1) Patrol_Pid.error = -7;
			
		}
		
		
		if(R1==1) Patrol_Pid.error = 5;
		else {
		
			if(R2 ==1) Patrol_Pid.error = 7;
			
		}
	}


	Patrol_Pid.iteg_error = Patrol_Pid.error;
	Pid_Out = Patrol_Pid.Kp*Patrol_Pid.error + Patrol_Pid.Kd*(Patrol_Pid.error - Patrol_Pid.last_error)+Patrol_Pid.Ki*Patrol_Pid.iteg_error;
	Patrol_Pid.last_error = Patrol_Pid.error;
	return Pid_Out;
}

int  Pid_Lin_Patrol1(void)
{
	
	int Pid_Out;
	int total;

	total = L2 + L1 + R2 + R1 + M;

	if(total>=3)	 Patrol_Pid.error = 0;
	else{
		if(L2==1) 
	{
		Patrol_Pid.error = -7;
		LED0 = ~LED0;
	} 
	else if(L1==1&&R2==1)
	{
		Patrol_Pid.error = -5;
		LED0 = ~LED0;
		
	}
		else if(L1==1) Patrol_Pid.error = -5;
			else if((L2 == 0&&L1 == 0&&R1 == 0&&R2 ==0&&M==0)||M==1||(L1==1&&R1==1)) Patrol_Pid.error = 0;
				else if(R1==1) Patrol_Pid.error = 5;
					else if(R2 ==1) Patrol_Pid.error = 7;
	
}

	Patrol_Pid.iteg_error = Patrol_Pid.error;
	Pid_Out = Patrol_Pid.Kp*Patrol_Pid.error + Patrol_Pid.Kd*(Patrol_Pid.error - Patrol_Pid.last_error)+Patrol_Pid.Ki*Patrol_Pid.iteg_error;
	Patrol_Pid.last_error = Patrol_Pid.error;
	return Pid_Out;
}

int Pid_Distance(int Distance_now)
{
	int Disatance_Out;
	static int chase_flag;
	static int res_flag;
	Distance_now=dis;
	if( Distance_now < 35 && Distance_now >=5 )
		{
			Distance_Pid.error = 0;
			chase_flag=1;
		}
		if(Res==2&&res_flag==0)
		{
			chase_flag=0;
			res_flag=1;
		}
	if(chase_flag==1)
	{
		if( Distance_now < 55 && Distance_now >=30)
		{
			Distance_Pid.error = 4;
		}
		
		else if( Distance_now < 30 && Distance_now >=19)
		{
			Distance_Pid.error = 2;
		}
		
		else if( Distance_now < 19 && Distance_now >=16 )
		{
			Distance_Pid.error = 0;
		}
		
		else if( Distance_now < 16 && Distance_now >=10)
		{
			Distance_Pid.error = -2;
		}
		
		else if( Distance_now < 10 && Distance_now >=0)
		{
			Distance_Pid.error = -4;
		}
		else
		{
			Disatance_Out=200;     
			return Disatance_Out;
		}
	}
	else
	{
		Distance_Pid.error = 4;
	}
	Disatance_Out = Distance_Pid.Kp*Distance_Pid.error + Distance_Pid.Kd*(Distance_Pid.error - Distance_Pid.last_error)+Distance_Pid.Ki*Distance_Pid.iteg_error;
	Distance_Pid.last_error = Distance_Pid.error;
	

	if(Disatance_Out>=120)
	{
		Disatance_Out = 120;
	}
	
	if(Disatance_Out<-120)
	{
		Disatance_Out = -120;
	}
	  
	return Disatance_Out;

}




