#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	

#define DATA_SIZE 18
#define PACKET_SIZE (2 * DATA_SIZE + 2)



extern unsigned char recv_data[PACKET_SIZE];
extern unsigned char recv_data_key[PACKET_SIZE];
extern volatile int data_ready,data_ready_key;



void parseData(const unsigned char* data, int* output);

extern u8  USART1_RX_BUF[USART_REC_LEN]; 
extern int OPENMV_Receive,num;
extern int L1,L2,L3,M,R1,R2,R3;
extern int op_x ,op_y ,op_flag; 
uint16_t Char2int(uint8_t high_byte, uint8_t low_byte);
void USART1_init(u32 bound);
void Openmv_Receive_Data(int16_t data);
void USART1_IRQHandler(void) ;
void openmvdatas(void);
void USART_SendHex(USART_TypeDef* USARTx, uint8_t* data, uint16_t size);
void resetBuffer1(unsigned char* buffer);
	
#endif


