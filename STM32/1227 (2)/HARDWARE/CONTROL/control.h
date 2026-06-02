#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.14159265
#define err_value 180     
#define Target_Velocity 800 
#define Target_Position 300
#define ZHONGZHI 1195     
#define Target_distance 200 

extern int Bluedata, OPENMV_Receive, K210_Receive, num;
extern int moto_left_f, moto_left_b, moto_right_f, moto_right_b;
extern int encoder_left_f, encoder_left_b, encoder_right_b, encoder_right_f;
extern int Velocity_cm, distance;
extern int Angle_Balance,Angle_PID_data,Angle_Current;
extern int Velocity_Left_F_Pwm, Velocity_B_Pwm, Velocity_Right_F_Pwm;
extern int Trail_Left_Pwm, Trail_Right_Pwm;
extern int Angle_Left_Pwm, Angle_Right_Pwm;
extern int Distance_Pwm;
extern int Balance_Pwm; 
extern int PWM_P, PWM_V;
extern int CurrentVelocity;
extern float CurrentPosition;
extern float T_err,V_eer;
extern float Pitch, Roll, Yaw;          
extern short gyrox, gyroy, gyroz;       
extern short aacx, aacy, aacz;          

extern int L1,L2,L3,M,R1,R2,R3;				
extern char OLED_buff[20];
extern int road_flage ;
extern uint16_t blue ;
extern uint8_t nHall_Ret,nYL_Ret;
extern char PitchStr[10],RollStr[10],YawStr[10];


struct Parameter_pid
{
    float Velocity_Kp;
    float Velocity_Ki;
    float Trail_Kp;
    float Trail_Ki;
    float Trail_Kd;
    float Balance_Kp;
    float Balance_Kd;
    float angle_Kp;
    float angle_Ki;
    float angle_Kd;
    float Velcity_Kp2;
    float Velcity_Ki2;
    float Velcity_Kd2;
    float Position_Kp;
    float Position_Ki;
    float Position_Kd;
    float distance_Kp;
    float distance_Ki;
    float distance_Kd;
};

void position(float tar_x,float tar_y,float tar_z);
void ALL_Parameter(void);
void send_wave(void);
void getdatas(void);
void get_cmd(void);
void OLED_ShowAll(void);
void Test_function(void);
void Turn_Left(void);
void Turn_Right(void);
void Turn_Larg_Right(void);
void Turn_Larg_Left(void);
void Stop(void);
void Go(void);
void Set_Pwm(int moto_left_f, int moto_left_b, int moto_right_f, int moto_right_b);
void Xianfu_Pwm(int moto_left_f, int moto_left_b, int moto_right_f, int moto_right_b);
int myabs(int a);
int balance(float Angle);
void velocity(void);
void Line_patrol(void);
void Angle_Ring(int angle);
void Position_FeedbackControl(float CurrentPosition);
void Velocity_FeedbackControl(float CurrentVelocity);
int PWM_Restrict(int PWM_P);
void Distance_Contol(int DIS);



extern int haoma_flag1_b,haoma_flag2_b,haoma_flag3_b,haoma_flag4_b,haoma_flag5_b,haoma_flag6_b,haoma_flag7_b,haoma_flag8_b,haoma_flag9_b;
extern int heibai_flage,heibai_flage_b;
extern int haoma_flag1,haoma_flag2,haoma_flag3,haoma_flag4,haoma_flag5,haoma_flag6,haoma_flag7,haoma_flag8,haoma_flag9;
extern float pointsArray[9][2];
extern float pointsmode3[9][2];
extern int heiqiboyi_flag,baiqi_flag,huiqi_flag;
extern int dss_4_key,ess_5_key;
extern int haomabu;
void mode1_control(void);
void mode2_control(void);
void mode3_control_h(int x_3,int y_3);
void mode3_control_b(int x_3,int y_3);
void mode3_control_h_t(int x_3,int y_3);
void mode3_control_b_t(int x_3,int y_3);
void mode4_control(int x_3,int y_3);
void mode5_control(void);
void mode6_control(void);




#endif

