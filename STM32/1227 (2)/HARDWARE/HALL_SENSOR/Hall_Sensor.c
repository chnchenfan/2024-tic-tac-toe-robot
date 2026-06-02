



#include "stm32f4xx.h" 
#include "Hall_Sensor.h"
 
 

#define HALL_SENSOR_DISAPPERAS_SHAKS_COUNT      500000

static uint32_t sHall_Sensor_DisapperasShakes_IdleCount = 0;			
static uint32_t sHall_Sensor_DisapperasShakes_TriggerCount = 0;	
 
 
static void Hall_Sensor_IO_Init(void);
 






static void Hall_Sensor_IO_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
}
 






uint8_t Hall_Sensor_Check_Pin(void)    
{    
  
  if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == SET) 
  {
    
    sHall_Sensor_DisapperasShakes_IdleCount++;
    sHall_Sensor_DisapperasShakes_TriggerCount = 0;
    
    
    if(sHall_Sensor_DisapperasShakes_IdleCount >= HALL_SENSOR_DISAPPERAS_SHAKS_COUNT)
    {
      return HALL_SENSOR_STATUS_IDLE;    
    }
  }
  
  else
  {
    
    sHall_Sensor_DisapperasShakes_IdleCount = 0;
    sHall_Sensor_DisapperasShakes_TriggerCount++;
    
    
    if(sHall_Sensor_DisapperasShakes_TriggerCount >= HALL_SENSOR_DISAPPERAS_SHAKS_COUNT)
    {
      return HALL_SENSOR_STATUS_TRIGGER;    
    }    
  }  
  
  return HALL_SENSOR_STATUS_DISAPPERAS_SHAKS;  
} 
 






void Hall_Sensor_Init(void)
{
  
  Hall_Sensor_IO_Init();	  
}



