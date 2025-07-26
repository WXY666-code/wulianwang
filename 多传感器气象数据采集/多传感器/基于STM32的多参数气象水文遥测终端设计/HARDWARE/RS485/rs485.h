#ifndef __RS485_H
#define __RS485_H
#include "sys.h" 

//#define RS485_RX_ENABLE GPIO_ResetBits(GPIOB,GPIO_Pin_3) //接收使能、低电平有效
//#define RS485_TX_ENABLE GPIO_SetBits(GPIOB,GPIO_Pin_3) //发送使能、高电平有效

void Modbus_uart2_init(u32 bound);//初始化函数
void Modbus_Send_Byte(u8 Modbus_byte);//发送一个字节函数




#endif


