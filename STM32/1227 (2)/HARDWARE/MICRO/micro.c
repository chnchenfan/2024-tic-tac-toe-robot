#include "micro.h"
#include "delay.h"


#define MIC_ADC_CHANNEL ADC_Channel_5
#define DAC_CHANNEL DAC_Channel_1

void MAX_Init(void) 
{




































   GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit();
    
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; 
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    
    ADC_RegularChannelConfig(ADC1, MIC_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles);
    ADC_Cmd(ADC1, ENABLE); 
		
		
}

void BIG_Init(void) {




















	
	
	

    DAC_InitTypeDef DAC_InitStructure;

      
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

     
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
     
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None; 
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    
    DAC_Cmd(DAC_Channel_1, ENABLE);
		
	
}

u16 Get_MicSignal(void) {

	ADC_RegularChannelConfig(ADC1, MIC_ADC_CHANNEL, 1, ADC_SampleTime_480Cycles );	
  
	ADC_SoftwareStartConv(ADC1);		
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);	
}


void Audio_PlayBack(u16 signal) {
    
    u32 dacValue = (signal << 4); 
    
    DAC_SetChannel1Data(DAC_Align_12b_R, dacValue);
    
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}


