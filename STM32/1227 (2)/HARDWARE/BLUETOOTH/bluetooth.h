#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "sys.h"

#define USART_REC_LEN           200

extern int Bluedata;
extern int blue_x ,blue_y ,blue_flag;
void USART3_init(u32 bound);

u8 usart_get_data(char *cmd, float *d1, float *d2, float *d3);
void usart_senddatas(USART_TypeDef *USARTx, u8 *addr, int size);

#endif


