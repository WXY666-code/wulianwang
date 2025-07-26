#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include <stdarg.h>//֧��sprintf��vsprintf����

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


extern u8 table_data[9];//������ǰ����һ�������Ž��յ�������
extern u8 table_cp[9];//���Ƕ��ⶨ��һ�����飬�����յ������ݸ��Ƶ�������
extern u16 count;//�������ݼ���

//���ڳ�ʼ������
void uart_init(u32 bound);//��ڲ����ǲ�����

//���ڷ���һ���ֽڵ����ݺ�����һ���ֽ���8λ���ݣ�����Ĳ���������һ��8λ�ģ�
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data);

//���������ֽ����ݺ���
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data);

//����һ����������
void Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num);

//�����ַ���
void Usart_SendStr(USART_TypeDef* pUSARTx,uint8_t *str);//ָ�����ڣ�Ҫ���͵��ַ�������

//int fgetc(FILE *f);


void usart_SendByte(uint8_t Byte);                    //�����ֽ�
void usart_SendArray(uint8_t *Array,uint16_t Length); //��������
void usart_SendString(char *String);                  //�����ַ���
void usart_SendNumber(uint32_t Number,uint8_t Length);//��������

void usart_printf(char *format,...);     			  //ģ��printf���ڴ�ӡ





#define BUFLEN 256      //���黺���С
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;
	
	
void uart_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void UART1_send_byte(char data);
void UART2_send_byte(char data);
void UART3_send_byte(char data);
void Uart1_SendStr(char*SendBuf);
void Uart2_SendStr(char*SendBuf);
void Uart3_SendStr(char*SendBuf);

extern UART_BUF buf_uart1;     //CH340
extern UART_BUF buf_uart2;     //TTL
extern UART_BUF buf_uart3;     //NBIOT








#endif


