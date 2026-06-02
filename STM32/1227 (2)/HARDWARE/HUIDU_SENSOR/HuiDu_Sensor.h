



#ifndef _HALL_SENSOR_H_
#define _HALL_SENSOR_H_
 
    

#define HALL_SENSOR_STATUS_TRIGGER                      0		
#define HALL_SENSOR_STATUS_IDLE                         1		
#define HALL_SENSOR_STATUS_DISAPPERAS_SHAKS             2		
 
 
uint8_t Hall_Sensor_Check_Pin(void);  
void Hall_Sensor_Init(void);

 
#endif

