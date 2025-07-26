#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

#define NO_PRINT 0
#define YES_PRINT 1

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);
uint16_t CountSensor_Get11(void);
void RTC_Counter_define (void);
void RTC_Counter_compare (void);
void Data_Oled_Init(void);


#endif
