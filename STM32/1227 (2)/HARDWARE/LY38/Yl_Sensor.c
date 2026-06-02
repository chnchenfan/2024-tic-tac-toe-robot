



#include "stm32f4xx.h" 
#include "YL_Sensor.h"
 
 

#define YL_SENSOR_DISAPPERAS_SHAKS_COUNT      500000

static uint32_t sYL_Sensor_DisapperasShakes_IdleCount = 0;			
static uint32_t sYL_Sensor_DisapperasShakes_TriggerCount = 0;	
 
 
static void YL_Sensor_IO_Init(void);
 






static void YL_Sensor_IO_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
}
 






uint8_t YL_Sensor_Check_Pin(void)    
{    
  
  if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) == SET) 
  {
    
    sYL_Sensor_DisapperasShakes_IdleCount++;
    sYL_Sensor_DisapperasShakes_TriggerCount = 0;
    
    
    if(sYL_Sensor_DisapperasShakes_IdleCount >= YL_SENSOR_DISAPPERAS_SHAKS_COUNT)
    {
      return YL_SENSOR_STATUS_IDLE;    
    }
  }
  
  else
  {
    
    sYL_Sensor_DisapperasShakes_IdleCount = 0;
    sYL_Sensor_DisapperasShakes_TriggerCount++;
    
    
    if(sYL_Sensor_DisapperasShakes_TriggerCount >= YL_SENSOR_DISAPPERAS_SHAKS_COUNT)
    {
      return YL_SENSOR_STATUS_TRIGGER;    
    }    
  }  
  
  return YL_SENSOR_STATUS_DISAPPERAS_SHAKS;  
} 
 






void YL_Sensor_Init(void)
{
  
  YL_Sensor_IO_Init();	  
}



