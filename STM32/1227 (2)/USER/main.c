#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "iwdg.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "bluetooth.h"
#include "oled.h"
#include "myiic.h"
#include "control.h"
#include "moto.h"
#include "Motor_Encoder.h"
#include "Motor_Pid_Time.h"
#include "k210.h"
#include "usart6.h"
#include "servo.h"
#include "adc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "hc_sr04.h"

#include "xinhao.h"

#define DATA_SIZE 18
int Bluedata, OPENMV_Receive, K210_Receive, Extern_Receive;
int key_mode, key_enter, key2_num, key3_num;              
int moto_left_f, moto_left_b, moto_right_b, moto_right_f; 
int encoder_left_f, encoder_left_b, encoder_right_b, encoder_right_f;
int Velocity_cm;
int Angle_Balance,Angle_PID_data,Angle_Current;
int distance = 0; 
int num ,num_m,num_start; 
int CurrentVelocity;
float CurrentPosition;
int hcsr04_flag = 0;
float T_err,V_eer;
float Pitch, Roll, Yaw;          
short gyrox, gyroy, gyroz;       
short aacx, aacy, aacz;          

int L1=0,L2=0,L3=0,M=1,R1=0,R2=0,R3=0;				
int KL1=0,KL2=0,KL3=0,KM=1,KR1=0,KR2=0,KR3=0;				
char OLED_buff[20];
int road_flage = 0;
int blue_x = 0 , blue_y = 0,blue_flag = 0;
int op_x = 0 , op_y = 0,op_flag = 0;
uint8_t nHall_Ret,nYL_Ret;
char PitchStr[10],RollStr[10],YawStr[10];

int parsed_data[DATA_SIZE] = {0};
int parsed_data_key[DATA_SIZE] = {0};
typedef struct {
    double x;
    double y;
} Point;

Point points_3[9];
uint8_t uav_flag;
uint8_t fire_flag;
float uart4_rec_x[];
float uart4_rec_y[];
uint8_t uav_cnt;
float uart4_flight_dist;
float uart4_fire[];
int MAX_STEPS;
int key_mode, key_enter, key2_num, key3_num;
int Key_mode1, Key_mode2, Key_mode3, Key_mode4;
int out_num=0; 

int heibai_flage = 0,heibai_flage_b =0;
int haoma_flag1=0,haoma_flag2=0,haoma_flag3=0,haoma_flag4=0,haoma_flag5=0,haoma_flag6=0,haoma_flag7=0,haoma_flag8=0,haoma_flag9=0;
int haoma_flag1_b=0,haoma_flag2_b=0,haoma_flag3_b=0,haoma_flag4_b=0,haoma_flag5_b=0,haoma_flag6_b=0,haoma_flag7_b=0,haoma_flag8_b=0,haoma_flag9_b=0;
int ass=0,bss=0,mode1ss=0,css_3=0,dss_4=0,dss_4_key=0,ess_5=0,ess_5_key=0;
int haomabu=0;
int heiqiboyi_flag=0,baiqi_flag=0,huiqi_flag=0;
int x_3=0,y_3=0;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    USART1_init(115200);      



    LED_Init();              

    BEEP_Init();             
	
	
    KEY_Init();              


	


	
	



    OLED_Init();             



   PWM_Init_TIM(8399, 9);   
   Motor_Init();            
   Encoder_TIM_Init();      

    Angle_Adc_Init();                








			MMGG_init();
			tie_init();
			mode_Init();
			anjian1_Init();
			anjian2_Init();
			num_m=0;
			MMGG1 = 0;
			MMGG2 = 0;
			MMGG3 = 1;
			MMGG4 = 1;
			xitie = 1;
		 MAX_STEPS = 1000;


   while (1)      
    {
		 if(mode1==0)
			{
				if(mode1ss==0)
				{
					mode1_control();
					mode1ss++;
				}
				Set_Pwm(0,0,0,0);
				LED_lsd();
				MUSIC1_Play();
			}
			else if(mode2==0)
			{
				mode2_control();
				if(heibai_flage==1)
				{
					
					if(ass==0){
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ass++;
					}
						Set_Pwm(0,0,0,0);
				}
				if(heibai_flage==2)
				{
					
					if(ass==1)
					{
					MMGG3 = 1;
					MMGG4 = 1;
					Set_Pwm(0,0,-4000,-4000);
					delay_ms(2205);
					MMGG3 = 1;
					Set_Pwm(0,0,-4000,0);
					delay_ms(6150);
					MMGG4 = 0;
					Set_Pwm(0,0,0,-4000);
					delay_ms(2205);
					ass++;
					}

					Set_Pwm(0,0,0,0);
				}
				if(heibai_flage==3)
				{
					
					if(ass==2){
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ass++;
					}
						Set_Pwm(0,0,0,0);
				}
				Set_Pwm(0,0,0,0);
				if(heibai_flage==4)
				{
					
					if(ass==3)
					{
					MMGG3 = 0;
					MMGG4 = 1;
					Set_Pwm(0,0,-4000,-4000);
					delay_ms(2205);
					MMGG3 = 0;
					Set_Pwm(0,0,-4000,0);
					delay_ms(6150);
					MMGG4 = 0;
					Set_Pwm(0,0,0,-4000);
					delay_ms(2205);
					ass++;
					}
					Set_Pwm(0,0,0,0);
					
				LED_lsd();
				MUSIC1_Play();
				}

			}
			else if(mode3==0)
			{
			Set_Pwm(0,0,0,0);
				if(css_3==0)
				{
					uint8_t hexData[]={0x7b, 0x03, 0x7d}; 
					USART_SendHex(USART1, hexData, sizeof(hexData));
					haoma_flag1=0;haoma_flag2=0;haoma_flag3=0;haoma_flag4=0;haoma_flag5=0;haoma_flag6=0;haoma_flag7=0;haoma_flag8=0;haoma_flag9=0;
					haoma_flag1_b=0;haoma_flag2_b=0;haoma_flag3_b=0;haoma_flag4_b=0;haoma_flag5_b=0;haoma_flag6_b=0;haoma_flag7_b=0;haoma_flag8_b=0;haoma_flag9_b=0;
					if (data_ready) 
        {
					LED_test_open();   
          parseData(recv_data, parsed_data);
          data_ready = 0;
					css_3=1;
				
				
				
				
        }
				
				}
				
				if(css_3==1)
				{
				for (int i = 0; i < 9; i++) {
        points_3[i].y = parsed_data[i + 9]; 
				}

				points_3[0].x = parsed_data[8]; 
				points_3[1].x = parsed_data[7]; 
				points_3[2].x = parsed_data[6]; 
				points_3[3].x = parsed_data[5]; 
				points_3[4].x = parsed_data[4]; 
				points_3[5].x = parsed_data[3]; 
				points_3[6].x = parsed_data[2]; 
				points_3[7].x = parsed_data[1]; 
				points_3[8].x = parsed_data[0]; 
				css_3=2;
				}
				
				if(css_3==2||css_3==4)
				{
					 x_3=0;
					 y_3=0;
					if(HAOMA1==0&&haoma_flag1==0)
					{
						css_3 ++;
						haoma_flag1++;
						x_3=(points_3[0].x-150) * 70;
						y_3=(points_3[0].y+74) * 14;
						mode3_control_h(x_3,y_3);
					}
					else if(HAOMA2==0&&haoma_flag2==0)
					{
						css_3 ++;
						haoma_flag2++;
						x_3=(points_3[1].x-150) * 70;
						y_3=(points_3[1].y+74) * 14;
						mode3_control_h(x_3,y_3);
					}
					else if(HAOMA3==0&&haoma_flag3==0)
					{
						css_3 ++;
						haoma_flag3++;
						x_3=(points_3[2].x-150) * 70;
						y_3=(points_3[2].y+74) * 14;
						mode3_control_h(x_3,y_3);
					}
					else if(HAOMA4==0&&haoma_flag4==0)
					{
						css_3 ++;
						haoma_flag4++;
						x_3=(points_3[3].x-150) * 70;
						y_3=(points_3[3].y+74) * 14;
						mode3_control_h(x_3,y_3);
					}
					else if(HAOMA5==0&&haoma_flag5==0)
					{
						css_3 ++;
						haoma_flag5++;
						x_3=(points_3[4].x-150) * 70;
						y_3=(points_3[4].y+74) * 14;
						mode3_control_h(x_3,y_3);						
					}
					else if(HAOMA6==0&&haoma_flag6==0)
					{
						css_3 ++;
						haoma_flag6++;
						x_3=(points_3[5].x-150) * 70;
						y_3=(points_3[5].y+74) * 14;
						mode3_control_h(x_3,y_3);
					}
					else if(HAOMA7==0&&haoma_flag7==0)
					{
						css_3 ++;
						haoma_flag7++;
						x_3=(points_3[6].x-150) * 70;
						y_3=(points_3[6].y+74) * 14;
						if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_h_t(x_3,y_3);
						}
						else{
							mode3_control_h(x_3,y_3);
						}
						
					}
					else if(HAOMA8==0&&haoma_flag8==0)
					{
						css_3 ++;
						haoma_flag8++;
						x_3=(points_3[7].x-150) * 70;
						y_3=(points_3[7].y+74) * 14;
						if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_h_t(x_3,y_3);
						}
						else{
							mode3_control_h(x_3,y_3);
						}
					}
					else if(HAOMA9==0&&haoma_flag9==0)
					{
						css_3 ++;
						haoma_flag9++;
						x_3=(points_3[8].x-150) * 70;
						
						y_3=(points_3[8].y+74) * 14;
						
						if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_h_t(x_3,y_3);
						}
						else{
							mode3_control_h(x_3,y_3);
						}
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
				else if(css_3==3)
				{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						css_3=4;
				}
				else if(css_3==5)
				{
					
					MMGG3 = 1;
					MMGG4 = 1;
					Set_Pwm(0,0,-4000,-4000);
					delay_ms(2205);
					MMGG3 = 1;
					Set_Pwm(0,0,-4000,0);
					delay_ms(6150);
					MMGG4 = 0;
					Set_Pwm(0,0,0,-4000);
					delay_ms(2205);
					css_3=6;
				}
				else if(css_3==6||css_3==8)
				{
					
					 x_3=0;
					 y_3=0;
					if(HAOMA1==0&&haoma_flag1==0)
					{
						css_3 ++;
						haoma_flag1++;
						x_3=(points_3[0].x-150) * 70;
						y_3=8355-(points_3[0].y+74) * 14;
						mode3_control_b(x_3,y_3);
					}
					else if(HAOMA2==0&&haoma_flag2==0)
					{
						css_3 ++;
						haoma_flag2++;
						x_3=(points_3[1].x-150) * 70;
						y_3=8355-(points_3[1].y+74) * 14;
						mode3_control_b(x_3,y_3);
					}
					else if(HAOMA3==0&&haoma_flag3==0)
					{
						css_3 ++;
						haoma_flag3++;
						x_3=(points_3[2].x-150) * 70;
						y_3=8355-(points_3[2].y+74) * 14;
						mode3_control_b(x_3,y_3);
					}
					else if(HAOMA4==0&&haoma_flag4==0)
					{
						css_3 ++;
						haoma_flag4++;
						x_3=(points_3[3].x-150) * 70;
						y_3=8355-(points_3[3].y+74) * 14;
						mode3_control_b(x_3,y_3);
					}
					else if(HAOMA5==0&&haoma_flag5==0)
					{
						css_3 ++;
						haoma_flag5++;
						x_3=(points_3[4].x-150) * 70;
						y_3=8355-(points_3[4].y+74) * 14;
						mode3_control_b(x_3,y_3);					
					}
					else if(HAOMA6==0&&haoma_flag6==0)
					{
						css_3 ++;
						haoma_flag6++;
						x_3=(points_3[5].x-150) * 70;
						y_3=8355-(points_3[5].y+74) * 14;
						mode3_control_b(x_3,y_3);		
					}
					else if(HAOMA7==0&&haoma_flag7==0)
					{
						css_3 ++;
						haoma_flag7++;
						x_3=(points_3[6].x-150) * 70;
						y_3=8355-(points_3[6].y+74) * 14;
						if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_b_t(x_3,y_3);
						}
						else{
						mode3_control_b(x_3,y_3);	
						}						
					}
					else if(HAOMA8==0&&haoma_flag8==0)
					{
						css_3 ++;
						haoma_flag8++;
						x_3=(points_3[7].x-150) * 70;
						y_3=8355-(points_3[7].y+74) * 14;
												if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_b_t(x_3,y_3);
						}
						else{
						mode3_control_b(x_3,y_3);	
						}								
					}
					else if(HAOMA9==0&&haoma_flag9==0)
					{
						css_3 ++;
						haoma_flag9++;
						x_3=(points_3[8].x-150) * 70;
						y_3=8355-(points_3[8].y+74) * 14;

						if(x_3<0)
						{
							x_3=abs(x_3);
							mode3_control_b_t(x_3,y_3);
						}
						else{
						mode3_control_b(x_3,y_3);	
						}						
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
				else if(css_3==7)
				{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						css_3 = 8;
				}
				else if(css_3==9)
				{
					MMGG3 = 0;
					MMGG4 = 1;
					Set_Pwm(0,0,-4000,-4000);
					delay_ms(2205);
					MMGG3 = 0;
					Set_Pwm(0,0,-4000,0);
					delay_ms(6150);
					MMGG4 = 0;
					Set_Pwm(0,0,0,-4000);
					delay_ms(2205);
					Set_Pwm(0,0,0,0);
					
					LED_lsd();
					MUSIC1_Play();
					css_3=10;
				}
			else
			{
				Set_Pwm(0,0,0,0);
			}
				
				
			}
			else if(mode4==0)
			{
				Set_Pwm(0,0,0,0);
				if(dss_4==0)
				{
					
					uint8_t hexData[]={0x7b, 0x04, 0x7d}; 
					USART_SendHex(USART1, hexData, sizeof(hexData));
					haomabu=0;
					if (data_ready) 
        {
					LED_test_open();   
          parseData(recv_data, parsed_data);
          data_ready = 0;
					dss_4=1;
					dss_4_key=0;
        }
				
				}
				else if(dss_4==1)
				{
					
				for (int i = 0; i < 9; i++) {
        points_3[i].y = parsed_data[i + 9]; 
				}

				points_3[0].x = parsed_data[8]; 
				points_3[1].x = parsed_data[7]; 
				points_3[2].x = parsed_data[6]; 
				points_3[3].x = parsed_data[5]; 
				points_3[4].x = parsed_data[4]; 
				points_3[5].x = parsed_data[3]; 
				points_3[6].x = 150; 
				points_3[7].x = 150; 
				points_3[8].x = 150; 
				dss_4=2;
				}
				else 
				{
					Set_Pwm(0,0,0,0);
					if(key_target1==0&&(dss_4_key==0||dss_4_key==3||dss_4_key==6||dss_4_key==9||dss_4_key==12))
					{
					
					uint8_t hexkey[]={0xff, 0x44, 0xfe}; 
					USART_SendHex(USART1, hexkey, sizeof(hexkey));
					if (data_ready_key) 
					{
					  
          parseData(recv_data_key, parsed_data_key);
					resetBuffer1(recv_data_key);
          data_ready_key = 0;
					dss_4_key++;
					haomabu = parsed_data_key[0];
						if(haomabu == 3)
						{
							LED_test_open(); 
						}
						
					}
					else 
					{
						haomabu=0;
					}
					
					}
				 if(dss_4_key==1||dss_4_key==4||dss_4_key==7||dss_4_key==10||dss_4_key==13)
					{
						if(haomabu!=0)
						{
						haomabu=haomabu-1;
						x_3=(points_3[haomabu].x-150) * 70;
						y_3=(points_3[haomabu].y+74) * 14;
						mode3_control_h(x_3,y_3);
						}
						else
						{
						Set_Pwm(0,0,0,0);
						}

					}
					else if(dss_4_key==2)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						dss_4_key++;
					}
					else if(dss_4_key==5)
					{
						
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						dss_4_key++;
					}
					else if(dss_4_key==8)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						dss_4_key++;
					}
					else if(dss_4_key==11)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						dss_4_key++;
						
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
			else if(mode5==0)
			{
				Set_Pwm(0,0,0,0);
				if(ess_5==0)
				{
					
					uint8_t hexData[]={0x7b, 0x55, 0x7d}; 
					USART_SendHex(USART1, hexData, sizeof(hexData));
					if (data_ready) 
        {
					LED_test_open();   
          parseData(recv_data, parsed_data);
          data_ready = 0;
					ess_5=1;
					ess_5_key=0;
					haomabu=0;
        }
				
				}
				else if(ess_5==1)
				{	
				for (int i = 0; i < 9; i++) {
        points_3[i].y = parsed_data[i + 9]; 
				}

				points_3[0].x = parsed_data[8]; 
				points_3[1].x = parsed_data[7]; 
				points_3[2].x = parsed_data[6]; 
				points_3[3].x = parsed_data[5]; 
				points_3[4].x = parsed_data[4]; 
				points_3[5].x = parsed_data[3]; 
				points_3[6].x = 150; 
				points_3[7].x = 150; 
				points_3[8].x = 150; 
				ess_5=2;
				}
				else 
				{

					if(key_target1==0&&(ess_5_key==0||ess_5_key==3||ess_5_key==6||ess_5_key==9||ess_5_key==12))
					{
					
					uint8_t hexkey[]={0xff, 0x04, 0xfe}; 
					USART_SendHex(USART1, hexkey, sizeof(hexkey));
					if (data_ready_key) 
					{
					LED_test_open();   
          parseData(recv_data, parsed_data_key);
          data_ready_key = 0;
					ess_5_key++;
					haomabu = parsed_data_key[1];
					}
					
					}
				 if(ess_5_key==1||ess_5_key==4||ess_5_key==7||ess_5_key==10||ess_5_key==13)
					{
						x_3=(points_3[haomabu].x-150) * 70;
						y_3=8355-(points_3[haomabu].y+74) * 14;
						mode3_control_b(x_3,y_3);
					}
					else if(ess_5_key==2)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ess_5_key++;
					}
					else if(ess_5_key==5)
					{
						
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ess_5_key++;
					}
					else if(ess_5_key==8)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ess_5_key++;
					}
					else if(ess_5_key==11)
					{
					
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						MMGG4 = 1;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						MMGG1 = 0;
						MMGG2 = 0;
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						MMGG4 = 0;
						Set_Pwm(0,0,0,-4000);
						delay_ms(500);
						xitie = 1;
						Set_Pwm(0,0,0,0);
						delay_ms(1500);
						MMGG1 = 1;
						MMGG2 = 1;
						MMGG3 = 0;
						MMGG4 = 0;						
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						Set_Pwm(-4000,-4000,0,0);
						delay_ms(6250);
						xitie = 0;
						Set_Pwm(0,0,0,0);
						delay_ms(500);
						ess_5_key++;
						
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
			else if(mode6==0)
			{
				
			}
			else
			{
				Set_Pwm(0,0,0,0);
			}




























				
			
    }

}


