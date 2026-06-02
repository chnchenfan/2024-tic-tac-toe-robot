#include "key.h"
#include "delay.h"
#include "led.h"
#include "xinhao.h"
#include "control.h"




void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG, ENABLE); 

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(GPIOG, &GPIO_InitStructure); 

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

void KEY_Scan(void)
{
		int sure = 0;
	
    if (KEY1 == 0) 
    {
        delay_ms(10);
        while (KEY1 == 0)
            sure = 1;
        if (sure == 1)
        {
					LED1 = 0;
					delay_ms(500);
					LED1 = 1;
					Key_mode1 = 1;
					Key_mode3 = 0;
					Key_mode3 = 0;
					Key_mode4 = 0;
            sure = 0;
        }
    }
    if (KEY2 == 0) 
    {
        delay_ms(10);
        while (KEY2 == 0)
            sure = 1;
        if (sure == 1)
        {
					LED2 = 0;
					delay_ms(500);
					LED2 = 1;
					Key_mode1 = 0;
					Key_mode3 = 1;
					Key_mode3 = 0;
					Key_mode4 = 0;
            sure = 0;
        }
    }
    if (KEY3 == 0) 
    {
        delay_ms(10);
        while (KEY3 == 0)
            sure = 1;
        if (sure == 1)
        {
					LED3 = 0;
					delay_ms(500);
					LED3 = 1;
					Key_mode1 = 0;
					Key_mode3 = 0;
					Key_mode3 = 1;
					Key_mode4 = 0;
            sure = 0;
        }
    }
	    if (KEY4 == 0) 
    {
        delay_ms(10);
        while (KEY4 == 0)
            sure = 1;
        if (sure == 1)
        {
					LED4 = 0;
					delay_ms(500);
					LED4 = 1;
					Key_mode1 = 0;
					Key_mode3 = 0;
					Key_mode3 = 0;
					Key_mode4 = 1;
            sure = 0;
        }
    }
	
}























































































































