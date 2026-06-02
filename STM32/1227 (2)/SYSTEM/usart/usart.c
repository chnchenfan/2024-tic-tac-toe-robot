#include "usart.h"
#include "led.h"
#include "oled.h"

unsigned char recv_data[PACKET_SIZE];
unsigned char recv_data_key[PACKET_SIZE];
volatile int data_ready = 0,data_ready_key=0;



#if SYSTEM_SUPPORT_OS
    #include "includes.h"                   
#endif


#if 1
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
    x = x;
}

int fputc(int ch, FILE *f)
{
    while ((USART6->SR & 0X40) == 0); 
    USART6->DR = (u8) ch;
    return ch;
}
#endif

u8 USART1_RX_BUF[USART_REC_LEN];
uint8_t openmv[4];






void USART1_init(u32 bound)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
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
    USART_Init(USART1, &USART_InitStructure); 

    USART_Cmd(USART1, ENABLE);  

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
    NVIC_Init(&NVIC_InitStructure); 
}






























































































































void resetBuffer1(unsigned char* buffer) {
    memset(buffer, 0, PACKET_SIZE);

}

void USART1_IRQHandler(void) {
    static int index = 0;
		static int index_key = 0;
	
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        unsigned char byte = USART_ReceiveData(USART1) & 0xFF;

        if (index == 0 && byte == 123) {
            recv_data[index++] = byte; 
        } else if (index > 0 && index < PACKET_SIZE) {
            recv_data[index++] = byte;
            if (index == PACKET_SIZE) {
                if (recv_data[PACKET_SIZE - 1] == 125) { 
                    data_ready = 1;
                }
                index = 0; 
            }
        }
				
				 if (index_key == 0 && byte == 255) {
            recv_data_key[index_key++] = byte; 
        } else if (index_key > 0 && index_key < PACKET_SIZE) {
            recv_data_key[index_key++] = byte;
            if (index_key == PACKET_SIZE) {
                if (recv_data_key[PACKET_SIZE - 1] == 254) { 
                    data_ready_key = 1;
                }
                index_key = 0; 
            }
        }
				
				
    }
}

void parseData(const unsigned char* data, int* output) {
    for (int i = 0; i < DATA_SIZE; ++i) {
        output[i] = (data[2 * i + 2] << 8) | data[2 * i + 1];
    }
}

void USART_SendHex(USART_TypeDef* USARTx, uint8_t* data, uint16_t size) {
  uint16_t i;  
	for ( i = 0; i < size; ++i) {
        while (!(USARTx->SR & USART_SR_TXE));
        USARTx->DR = data[i];
    }
}

