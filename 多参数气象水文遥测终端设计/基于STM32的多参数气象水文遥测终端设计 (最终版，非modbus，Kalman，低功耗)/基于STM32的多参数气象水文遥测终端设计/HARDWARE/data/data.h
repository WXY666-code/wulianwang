#include "stm32f10x.h"




typedef struct 
{
	
    float  Ligth;        //光照强度
	float  UVI;          //紫外线指数
	float  Temp;       //温度
	uint16_t  Humi;      //湿度
	float  Wind_speed;       //风速
	float  Gust_speed;       //阵风风速
	uint16_t  Wind_direction; //风向
	float  Rainfall_M; //降雨量（Modbus）
	
	float  Rainfall_F; //降雨量（翻斗）
	
	float  Pressure; //大气压
	
	//kalman
	float  K_Pressure; //1.大气压
	float  K_Ligth;    //2.光照强度
	float  K_UVI;      //3.紫外线指数
	

}DATA;

void data_str (void) ;
	
void data_oled (void);


