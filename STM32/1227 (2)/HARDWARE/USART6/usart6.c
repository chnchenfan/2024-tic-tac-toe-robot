#include "usart6.h"
#include "delay.h"

#include <stdio.h>
#include <math.h>
#include <string.h>






void USART6_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART6, &USART_InitStructure);

    USART_Cmd(USART6, ENABLE);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART6_IRQHandler(void)
{
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(USART6, USART_FLAG_RXNE);
        Extern_Receive = USART_ReceiveData(USART6);
        USART_SendData(USART6, Extern_Receive);
    }
}

void USART6_SendString(const char *string)
{
    while (*string)
    {
        
        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
        
       
        USART_SendData(USART6, *string++);
        
        
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
    }
}
void USART6_SendBinary(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++)
    {

        while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
        
    
        USART_SendData(USART6, 0xFF);
        
  
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
    }
}


void draw(void)
{
		char cmd[50];
    const int point_num = 5;
    int UAVwaypoint[point_num][2] = {{185, 246},
                                    {185, 70},
                                    {400, 70},
                                    {400, 246},
                                    {185, 246}};
                                    
    USART6_SendString("rest");    
    USART6_SendBinary();
		delay_ms(4000);
    for(int i = 0; i < point_num; i++)
    {
        sprintf(cmd, "line %d,%d,%d,%d,BLUE", UAVwaypoint[i][0], UAVwaypoint[i][1], UAVwaypoint[i+1][0], UAVwaypoint[i+1][1]);
        USART6_SendString(cmd);
        USART6_SendBinary();
				delay_ms(4000);
		}			
		
}

void uav_point_write(void)
{


	if(uav_flag == 1)  
    {
        uav_flag = 0;
        char cmd[30];
        float x_fixed, y_fixed;
        uint16_t x_pixel, y_pixel;

        memset(cmd, 0, sizeof(cmd));
        
			







        if (uart4_rec_x[uav_cnt-1] > 480)
            x_fixed = 480;
        else if (uart4_rec_x[uav_cnt-1] < 0)
            x_fixed = 0;
        else
            x_fixed = uart4_rec_x[uav_cnt-1];
						x_pixel = (uint16_t)(x_fixed * 480 / 480);
        







        if (uart4_rec_y[uav_cnt-1] > 400)
            y_fixed = 400;
        else if (uart4_rec_y[uav_cnt-1] < 0)
            y_fixed = 0;
        else
            y_fixed = uart4_rec_y[uav_cnt-1];
						y_pixel = (uint16_t)(y_fixed * 240 / 400);
        
        sprintf(cmd, "cirs %d,%d,1,BLUE", 160 + x_pixel, 262 - y_pixel);
        USART6_SendString(cmd);  
        USART6_SendBinary();
        
        sprintf(cmd, "t16.txt=\"(%d,%d)\"", (int)(x_fixed / 10), (int)(y_fixed / 10));
        USART6_SendString(cmd);  
        USART6_SendBinary();
        
        sprintf(cmd, "t10.txt=\"%.3f\"", uart4_flight_dist / 100);
        USART6_SendString(cmd);  
        USART6_SendBinary();
    }
		
    if (fire_flag == 1)  
    {
        fire_flag = 0;
        char cmd[30];
        float x_fixed, y_fixed;
        uint16_t x_pixel, y_pixel;

        memset(cmd, 0, sizeof(cmd));

        if (uart4_fire[0] > 480)
            x_fixed = 480;
        else if (uart4_fire[0] < 0)
            x_fixed = 0;
        else
            x_fixed = uart4_fire[0];
						x_pixel = (uint16_t)(x_fixed * 480 / 480);
        
        if (uart4_fire[1] > 400)
            y_fixed = 400;
        else if (uart4_fire[1] < 0)
            y_fixed = 0;
        else
            y_fixed = uart4_fire[1];
						y_pixel = (uint16_t)(y_fixed * 240 / 400);
        
        sprintf(cmd, "pic %d,%d,0", 160 + x_pixel, 262 - y_pixel);
        USART6_SendString(cmd);  
        USART6_SendBinary();
        
        sprintf(cmd, "t8.txt=\"(%d,%d)\"", (int)(x_fixed / 10), (int)(y_fixed / 10));
        USART6_SendString(cmd);  
        USART6_SendBinary();
    }
}






