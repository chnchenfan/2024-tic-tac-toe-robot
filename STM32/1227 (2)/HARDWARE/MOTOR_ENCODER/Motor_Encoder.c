#include "Motor_Encoder.h"
#include "sys.h"





















int Read_Encoder(int TIMx)
{
    int Encoder;
    
    switch (TIMx)
    {
    case 5:
        Encoder = (short)TIM5->CNT;
        TIM5->CNT = 0;
        break;
    case 1:
        Encoder = (short)TIM1->CNT;
        TIM1->CNT = 0;
        break;
    case 4:
        Encoder = -(short)TIM4->CNT;
        TIM4->CNT = 0;
        break;
    case 2:
        Encoder = -(short)TIM2->CNT;
        TIM2->CNT = 0;
        break;
    default:
        Encoder = 0;
        break;
    }
    return Encoder;
}






void Encoder_TIM_Init(void)
{
    
    GPIO_InitTypeDef    GPIO_InitStructure;

    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_ICInitTypeDef                 TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

    TIM_TimeBaseStructure.TIM_Period = 65535;         
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM2, ENABLE);

    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM5, ENABLE);

    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM5);

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM4, ENABLE);

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM1, ENABLE);
}






void TIM1_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);   
}






void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   
}






void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);   
}






void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);   
}
