



#ifndef _YL_SENSOR_H_
#define _YL_SENSOR_H_
 
    

#define YL_SENSOR_STATUS_TRIGGER                      0		
#define YL_SENSOR_STATUS_IDLE                         1		
#define YL_SENSOR_STATUS_DISAPPERAS_SHAKS             2		
 
 
uint8_t YL_Sensor_Check_Pin(void);  
void YL_Sensor_Init(void);

 
#endif

