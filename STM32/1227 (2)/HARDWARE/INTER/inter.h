#ifndef _INTER_H
#define _INTER_H
#include "sys.h"

#define Start() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define Retrun() GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

void inter_init(void);




#endif 



