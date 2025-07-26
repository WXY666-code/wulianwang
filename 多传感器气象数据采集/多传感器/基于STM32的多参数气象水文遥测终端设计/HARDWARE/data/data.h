#include "stm32f10x.h"




typedef struct 
{
	
    float  Ligth;        //����ǿ��
	float  UVI;          //������ָ��
	float  Temp;       //�¶�
	uint16_t  Humi;      //ʪ��
	float  Wind_speed;       //����
	float  Gust_speed;       //������
	uint16_t  Wind_direction; //����
	float  Rainfall_M; //��������Modbus��
	
	float  Rainfall_F; //��������������
	
	float  Pressure; //����ѹ
	
	//kalman
	float  K_Pressure; //1.����ѹ
	float  K_Ligth;    //2.����ǿ��
	float  K_UVI;      //3.������ָ��
	

}DATA;

void data_str (void) ;
	
void data_oled (void);


