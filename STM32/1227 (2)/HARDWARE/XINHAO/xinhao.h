#ifndef __XINHAO_H
#define __XINHAO_H
#include "sys.h"




#define MMGG1 PGout(4)
#define MMGG2 PGout(5)
#define MMGG3 PGout(6)
#define MMGG4 PGout(7)


#define xitie PGout(2)










void MMGG_init(void);
void tie_init(void);
void mode_Init(void);
void anjian1_Init(void);
void anjian2_Init(void);

#define mode1 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8) 
#define mode2 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)
#define mode3 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)
#define mode4 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define mode5 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define mode6 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)


#define key_target1 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)
#define key_target2 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)


#define HAOMA1 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0) 
#define HAOMA2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)
#define HAOMA3 		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1)
#define HAOMA4 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)
#define HAOMA5 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)
#define HAOMA6 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define HAOMA7 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define HAOMA8 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
#define HAOMA9 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)









#endif
