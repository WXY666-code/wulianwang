#ifndef __BC26_H
#define __BC26_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
#include "usart.h"
void Clear_Buffer(void);//清空缓存	
int BC26_Init(void);
void BC26_PDPACT(void);
void BC26_RECData(void);
void BC26_RegALIYUNIOT(void);
//void BC26_ALYIOTSenddata(u8 *len,u8 *data);
//void BC26_ALYIOTSenddata1(u8 *len,u8 *data);

void BC26_ALYIOTSenddata_Light(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_UVI(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_Temp(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_Humi(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_Wind_speed(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_Gust_speed(u8 *len,u8 *data) ;
void BC26_ALYIOTSenddata_Wind_direction(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_Rainfall_M(u8 *len,u8 *data) ;
void BC26_ALYIOTSenddata_Rainfall_F(u8 *len,u8 *data) ;
void BC26_ALYIOTSenddata_Pressure(u8 *len,u8 *data);
void BC26_ALYIOTSenddata_K_Pressure(u8 *len,u8 *data) ;
void BC26_ALYIOTSenddata_K_Ligth(u8 *len,u8 *data) ;
void BC26_ALYIOTSenddata_K_UVI(u8 *len,u8 *data)  ;

void BC26_DATA(void);
	


typedef struct
{
   uint8_t CSQ;    
   uint8_t Socketnum;   //编号
   uint8_t reclen[10];   //获取到数据的长度
   uint8_t res;      
   uint8_t recdatalen[10];
   uint8_t recdata[100];
} BC26;

#endif


