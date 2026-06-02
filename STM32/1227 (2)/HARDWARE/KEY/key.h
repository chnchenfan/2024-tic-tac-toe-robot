#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY1        GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)
#define KEY2        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
#define KEY3        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)
#define KEY4        GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)

extern int key_mode, key_enter, key2_num, key3_num;
extern int Key_mode1, Key_mode2, Key_mode3, Key_mode4;

void KEY_Init(void);    
void Key_Detect(void);  
void KEY_Scan(void);






void EXTI15_10_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI_Configuration(void);
void GPIO_Configuration(void);


#endif

