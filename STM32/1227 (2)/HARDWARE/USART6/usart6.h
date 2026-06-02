#ifndef __USART6_H
#define __USART6_H
#include "sys.h"

extern int Extern_Receive;

extern uint8_t uav_flag;
extern uint8_t fire_flag;
extern float uart4_rec_x[];
extern float uart4_rec_y[];
extern uint8_t uav_cnt;
extern float uart4_flight_dist;
extern float uart4_fire[];


void USART6_init(u32 bound);
void USART6_SendString(const char *string);
void USART6_SendBinary(void);
void USART6_IRQHandler(void);
void draw(void);
void uav_point_write(void);

#endif
