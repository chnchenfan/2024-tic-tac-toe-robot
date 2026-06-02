#ifndef __JY901_U2_H
#define __JY901_U2_H

#include "sys.h" 

#define USART_REC_LEN  			200 


void uart2_init(u32 bound);
void UART2_Put_Char(unsigned char DataToSend);
void UART2_Put_String(unsigned char *Str);
void sendcmd(char cmd[]);
void CopeSerial2Data(unsigned char ucData);
void CopeSerial1Data(unsigned char ucData);
float jy901_test(void);


#endif


