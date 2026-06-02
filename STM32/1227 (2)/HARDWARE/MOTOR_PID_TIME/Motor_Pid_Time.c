#include "Motor_Pid_Time.h"
#include "delay.h"
#include "Motor_Encoder.h"
#include "control.h"
#include "adc.h"
#include "led.h"
#include <stdio.h>



volatile uint32_t stepCounter = 0;




void TIM7_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitstruct;
    NVIC_InitTypeDef NVIC_InitStrecut;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseInitstruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitstruct.TIM_Period = arr;
    TIM_TimeBaseInitstruct.TIM_Prescaler = psc;
    TIM_TimeBaseInitstruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitstruct);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);

    NVIC_InitStrecut.NVIC_IRQChannel = TIM7_IRQn  ;
    NVIC_InitStrecut.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStrecut.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStrecut.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_Init(&NVIC_InitStrecut);
}










void TIM7_IRQHandler(void)
{


    if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
    {












				num = 0;
				TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }



}
