#include "k210.h"


uint8_t k210[4];





void USART2_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void K210datas(void)
{
		KL1 = ((u8)k210[2] & 0x40) >> 6;	
		KL2 = ((u8)k210[2] & 0x20) >> 5;
		KL3 = ((u8)k210[2] & 0x10) >> 4;
		KM = ((u8)k210[2] & 0x08) >> 3;
		KR1 = ((u8)k210[2] & 0x04) >> 2;
		KR2 = ((u8)k210[2] & 0x02) >> 1;
		KR3 = (u8)k210[2] & 0x01;
	
}
void K210_Receive_Data(int16_t data)
{ 
	int i;
	static u8 state = 0;
	if(state==0&&data==0xA7)
	{
		state=1;
		k210[0]=data;
	}
	else if(state==1&&data==0xA8)
	{
		state=2;
		k210[1]=data;
	}
	else if(state==2)
	{
		state=3;
		k210[2]=data;
	}
	else if(state==3)
	{
		if(data==0xAB)
		{
			state = 0;
		  k210[3]=data;
			K210datas();
			
		}
		if(data!=0xAB)
		{
			  state = 0;
            for( i=0;i<5;i++)
            {
                k210[i]=0x00;
            }           
		}
	}
	else
	{
		state = 0;
            for( i=0;i<5;i++)
            {
                k210[i]=0x00;
            }
	}

}


void USART2_IRQHandler(void)
{
		u8 com_data_k;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearFlag(USART2, USART_FLAG_RXNE);


				com_data_k = USART2->DR;
				K210_Receive_Data(com_data_k);     
				K210datas();
    }
}


