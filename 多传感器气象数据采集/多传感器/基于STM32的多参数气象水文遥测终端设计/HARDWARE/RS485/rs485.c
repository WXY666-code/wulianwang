#include "sys.h"
#include "rs485.h"	  
#include "delay.h"
#include	"modbus_timer.h"
#include "modbus.h"

//��ʱ���ж�����Rx_Data_len
//PD7����485�ķ��ͺͽ���ʹ��
//rs485��ʼ�����һ������Ϊ����ʹ�ܣ�����״̬���ڽ���״̬��ֻ�з��͵�ʱ���ʹ�ܷ��ͣ�


//modbus���ڷ���һ���ֽ�����
void Modbus_Send_Byte(u8 Modbus_byte)
{
  USART_SendData(USART2,Modbus_byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC); 
}



////485���ڳ�ʼ��
////��ʼ��IO ����2 
////bound:������
//void Modbus_uart2_init(u32 bound){
//    //GPIO�˿�����
//		GPIO_InitTypeDef GPIO_InitStructure;//GPIO�ṹ��ָ��
//		USART_InitTypeDef USART_InitStructure;//���ڽṹ��ָ��
//		NVIC_InitTypeDef NVIC_InitStructure;//�жϷ���ṹ��ָ��
//		//1��ʹ�ܴ���ʱ�ӣ���������ʱ�� ����2���ص�APB1��
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��
//		
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ�ܴ���ʱ�Ӻ��շ�ʹ��ʱ��
//	
//     //	"|RCC_APB2Periph_GPIOB"ɾȥ
//	
//	//2����λ����	
//		USART_DeInit(USART2);  //��λ����1
//	
//	//3�����ͽ������ŵ�����
//	 //USART2_TX   PA.2����ͼ ��֪����Ϊ���츴�������
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
//    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
//   
//    //USART2_RX	  PA.3����ͼ��֪�������룩
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
//		
////		//485�շ���������PD7
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PA.9
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���������
////    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA9

//   //4��USART ��ʼ������

//		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//    USART_Init(USART2, &USART_InitStructure); //��ʼ������
// 
//   //5��Usart1 NVIC ����
//		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//   
//	  //6���������������ж�
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
//		
//		//7��ʹ�ܴ���
//    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

////		RS485_RX_ENABLE;//ʹ�ܽ������ţ���̬�´��ڽ���״̬��
//}


////modbus�����жϷ������
//void USART2_IRQHandler(void)                
//{
//  u8 st,Res;
//	st = USART_GetITStatus(USART2, USART_IT_RXNE);
//	if(st == SET)//�����ж�
//	{
//		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
////	  USART_SendData(USART1, Res);//���ܵ�����֮�󷵻ظ�����1
//	 if( modbus.reflag==1)  //�����ݰ����ڴ���
//	  {
//		   return ;
//		}		
//	  modbus.rcbuf[modbus.recount++] = Res;
//		modbus.timout = 0;
//		if(modbus.recount == 1)  //�Ѿ��յ��˵ڶ����ַ�����
//		{
//		  modbus.timrun = 1;  //����modbus��ʱ����ʱ
//		}
//	}
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ���������־��
//		}
//		
//} 


