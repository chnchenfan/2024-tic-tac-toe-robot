#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 












   	   		   

#define SDA_IN()  {GPIOG->MODER&=~(3<<(2*2));GPIOG->MODER|=0<<2*2;}	
#define SDA_OUT() {GPIOG->MODER&=~(3<<(2*2));GPIOG->MODER|=1<<2*2;} 

#define IIC_SCL    PGout(3) 
#define IIC_SDA    PGout(2) 
#define READ_SDA   PGin(2)  


void IIC_Init(void);                
void IIC_Start(void);				
void IIC_Stop(void);	  			
void IIC_Send_Byte(u8 txd);			
u8 IIC_Read_Byte(unsigned char ack);
u8 IIC_Wait_Ack(void); 				
void IIC_Ack(void);					
void IIC_NAck(void);				

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















