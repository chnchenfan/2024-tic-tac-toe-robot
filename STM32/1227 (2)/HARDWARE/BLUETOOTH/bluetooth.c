#include "bluetooth.h"
#include "led.h"
#include <stdio.h>
#include <string.h>

extern int Bluedata;

u8 USART3_RX_BUF[USART_REC_LEN] = {"hello"};
unsigned int USART3_RX_STA = 0;

u8 first_find_state = 0;
u8 state_start = 0; //ÓÃÓÚ±êÊ¶Ð¡³µÊÇ·ñÆô¶

/**************************************************************************
º¯Êý¹¦ÄÜ£º³õÊ¼»¯´®¿Ú3
Èë¿Ú²ÎÊý£ºbound²¨ÌØÂÊ
×¢£ºPB10->TX   PB11->RX
**************************************************************************/
void USART3_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
		
    USART_Cmd(USART3, ENABLE);
		
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//char usart_readbuff[6] = {0}; //´®¿Ú½ÓÊÜ»º´æÊý×é
//u8 usart_readok = 0; //Ò»Ö¡Êý¾Ý´¦Àí±êÖ¾
void USART3_IRQHandler(void)
{
//    u16 temp;
//		int i;
////    static u8 count = 0;  // ½ÓÊÕÊý×é¿ØÖÆ±äÁ¿
//    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //ÅÐ¶ÏÊÇ·ñÎª½ÓÊÕÖÐ¶Ï
//    {
//        temp = USART_ReceiveData(USART3);   //¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý,²¢Çå³ýÖÐ¶Ï±êÖ¾
//	if(usart_readok==0&&temp==0xFF)//µÚÒ»¸öÖ¡Í·
//	{
//		usart_readok=1;
//		usart_readbuff[0]=temp;
//	}
//	else if(usart_readok==1)//µÚÒ»¸öÓÐÐ§Êý¾ÝµÍÎ»
//	{
//		usart_readok=2;
//		usart_readbuff[1]=temp;
//	}
//		else if(usart_readok==2)//µÚÒ»¸öÓÐÐ§Êý¾Ý¸ßÎ»
//	{
//		usart_readok=3;
//		usart_readbuff[2]=temp;
//	}
//		else if(usart_readok==3)//µÚ¶þ¸öÓÐÐ§Êý¾ÝµÍÎ»
//	{
//		usart_readok=4;
//		usart_readbuff[3]=temp;
//	}
//			else if(usart_readok==4)//µÚ¶þ¸öÓÐÐ§Êý¾Ý¸ßÎ»
//	{
//		usart_readok=5;
//		usart_readbuff[4]=temp;
//	}
//		else if(usart_readok==5)
//	{
//		if(temp==0xFE)
//		{
//			blue_flag = 1;
//			usart_readok = 6;
//		  usart_readbuff[5]=temp;
//			blue_x = Char2int(usart_readbuff[2],usart_readbuff[1]);
//			blue_y = Char2int(usart_readbuff[4],usart_readbuff[3]);
//		}
//		if(temp!=0xFE)
//		{
//			  temp = 0x00;
//        for( i=0;i<6;i++)
//        {
//        usart_readbuff[i]=0x00;
//				blue_x = 0;
//			  blue_y = 0;
//        }           
//		}
//	}
//	else
//	{
//		temp = 0x00;
//    for( i=0;i<6;i++)
//    {
//    usart_readbuff[i]=0x00;
//				blue_x = 0;
//			  blue_y = 0;
//    }
//	}
//    }
}

u8 usart_get_data(char *cmd, float *d1, float *d2, float *d3)
{
//    u8 flag = 0;
//    if (usart_readok == 1)
//    {
//        if (sscanf(usart_readbuff, "%3s=%f,%f,%f#",
//                   cmd, d1, d2, d3) == 4)
//        {
//            flag = 1;
//        }
//        //Çå³ý½ÓÊÕÍê³É±êÖ¾
//        memset(usart_readbuff, 0, sizeof(usart_readbuff));
//        usart_readok = 0;
//    }
//    return flag;
}


void usart_senddatas(USART_TypeDef *USARTx, u8 *addr, int size)
{
    while (size--) //ÅÐ¶ÏÊý¾Ý·¢ËÍÍêÃ»ÓÐ
    {
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); //µÈ´ýÉÏÒ»¸öbyteµÄÊý¾Ý·¢ËÍ½áÊø¡£
        USART_SendData(USARTx, *addr); //µ÷ÓÃSTM32±ê×¼¿âº¯Êý·¢ËÍÊý¾Ý
        addr++; //µØÖ·Æ«ÒÆ
    }
}
