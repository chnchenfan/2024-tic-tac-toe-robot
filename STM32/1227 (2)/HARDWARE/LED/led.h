#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED1 PEout(3)
#define LED2 PEout(4)
#define LED3 PEout(5)
#define LED4 PEout(6)
#define LEDEMIT PEout(7)
#define 	RGB_LED 	GPIO_Pin_8
#define		RGB_LED_HIGH	(GPIO_SetBits(GPIOB,RGB_LED))
#define 	RGB_LED_LOW		(GPIO_ResetBits(GPIOB,RGB_LED))

void LED_Init(void);
void LED_lsd(void);
void LED_test_open(void);
void LED_test_close(void);
void LEDEMIT_test_open(void);
void LEDEMIT_test_close(void);
void RGB_LED_Init(void);
void RGB_LED_Write0(void);
void RGB_LED_Write1(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue);
void RGB_LED_Red(void);
void RGB_LED_Green(void);
void RGB_LED_Blue(void);

#endif
