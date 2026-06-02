#ifndef __K210_H
#define __K210_H
#include "sys.h"
	 
extern int K210_Receive;
extern int KL1,KL2,KL3,KM,KR1,KR2,KR3;


void USART2_init(u32 bound);
void K210datas(void);
void K210_Receive_Data(int16_t data);

#endif 




