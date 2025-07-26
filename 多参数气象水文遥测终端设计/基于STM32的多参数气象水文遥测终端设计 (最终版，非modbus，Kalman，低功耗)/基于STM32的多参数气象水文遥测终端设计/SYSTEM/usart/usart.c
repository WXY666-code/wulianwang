#include "sys.h"
#include "usart.h"	  
#include "stdio.h"
#include "string.h"
#include "stm32f10x_tim.h"



//getchar()�ȼ���scanf()����
//���ʹ��getchar����Ҳ��Ҫ���¶���

////�ض���c�⺯��scanf�����ڣ���д�����ʹ��scanf��getchar����
int fgetc(FILE *f)
{
	//�ȴ�������������
	/* ��������ȴ��Ͳ���Ҫ���ж��н����� */
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	return (int)USART_ReceiveData(USART1);
}
//�������������ʹ��getchar()��Ҫ��������ж����ô���ע�͵���������ͻ

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//    USART1->DR = (u8) ch;      
//	return ch;
//}

#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}

int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
//�������ݷ�װ
void usart_SendByte(uint8_t Byte)   //����һλ����
{
    USART_SendData(USART1,Byte);
    //whileѭ���ȴ�TXE��־λ��1�����TXE��־λ==RESET,��ѭ���ȴ�ֱ��SET�������ȴ�
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
 
}

//���͹̶�������������(ѭ�������ֽ�)
void usart_SendArray(uint8_t *Array,uint16_t Length)
{
    uint16_t i;
    for(i=0;i<Length;i++)
    {
        usart_SendByte(Array[i]);
    } 
}

//�����ַ�������
void usart_SendString(char *String)
{
    uint8_t i;
    for(i=0;i<String[i]!='\0';i++) //�����ڿ��ַ�0x00��0 ��'\0'
    {
        usart_SendByte(String[i]);
    }
}

//// �����ַ�����ʽ������,��Ҫд��ʽ����ͷ�ļ�֧��
//uint32_t Serial_Pow(uint32_t x,uint32_t y)
//{
//    uint32_t Result = 1;
//    while(y--)
//    {
//        Result *= x ;
//    }    
//    return Result;
//}
//void usart_SendNumber(uint32_t Number,uint8_t Length)
//{
//    uint8_t i;
//    for(i=0;i<Length;i++)
//    {
//        usart_SendByte(Number / Serial_Pow(10,Length-i-1)%10 + 0x30);//0x30Ϊƫ��
//    }
//}


void usart_printf(char *format,...)
{
	char String[100];		 //��������ַ���
	va_list arg;			 //����һ�������б����va_list��һ����������arg�Ǳ�����
	va_start(arg,format);	 //��formatλ�ÿ�ʼ���ղ��������arg����
	
	//sprintf��ӡλ����String����ʽ���ַ�����format����������arg�����ڷ�װ��ʽsprintfҪ�ĳ�vsprintf
	vsprintf(String,format,arg);
	va_end(arg);			 //�ͷŲ�����
	usart_SendString(String);//����String
	

}





u8 table_data[9];//������ǰ����һ�������Ž��յ�������
u8 table_cp[9];//���Ƕ��ⶨ��һ�����飬�����յ������ݸ��Ƶ�������
u16 count=0;//�������ݼ���

 
#if EN_USART1_RX   //���ʹ���˽���



//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;//GPIO�ṹ��ָ��
		USART_InitTypeDef USART_InitStructure;//���ڽṹ��ָ��
		NVIC_InitTypeDef NVIC_InitStructure;//�жϷ���ṹ��ָ��
		//1��ʹ�ܴ���ʱ�ӣ���������ʱ�� 
 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		
	//2����λ����	
		USART_DeInit(USART1);  //��λ����1
	
	//3�����ͽ������ŵ�����
	 //USART1_TX   PA.9����ͼ ��֪����Ϊ���츴�������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10����ͼ��֪�������룩
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


   //4��USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
		
#if EN_USART1_RX		  //���ʹ���˽���  
   //5��Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
	  //6���������������ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
		
#endif
		//7��ʹ�ܴ���
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}


//uint8_t��8λ�����ݣ�uint16_t��16λ������
//��uint8_t��8λ���ݴ��ݸ�uint16_t��16λ����ʱ�����Զ�ǿ������ת��


//���ڷ���һ���ֽڵ����ݺ�����һ���ֽ���8λ���ݣ�����Ĳ���������һ��8λ�ģ�
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data)//ÿ��ֻ�ܷ���8λ������
{
	//���ù̼��⺯��
	USART_SendData(pUSARTx,data);//��������д������
	
	//�����������Ǽ��TXE���λ�Ƿ���1���������ݼĴ������ˣ������Ѿ������ݴ��ݵ�������λ�Ĵ�����
	//���TXE���λҲ��Ҫһ���̼��⺯��
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	//������λһֱΪ0�Ļ���һֱ�ȴ���ֻ�е�����ΪSET��Ż��������ѭ������ʾһ���ֽڷ��ͳ�ȥ�ˣ�

}



//��ʱ�򴫸������ݿ�����16λ�ģ���ô���ͣ����������ֽ�
//���������ֽڵ����ݾ���ʮ��λ��
//���ֱ�ʾ16λ�������ֽڣ�������2��ʮ��λ������

//���������ֽ����ݺ���
void Usart_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t data)
{
	//����ʮ��λ����Ҫ��Ϊ���������ͣ��ȶ�����������
	uint8_t temp_h,temp_l;//����8λ�ı������ֱ�洢��8λ�͵�8λ��

	//����ȡ����8λ
	temp_h=(data&0xff00)>>8;//�Ͱ�λ����0��&����8λ��Ϊ0������8λ��0xff00��16λ�����ƣ�
	//��ȡ����8λ
	temp_l=data&0xff;//ȡ����8λ����
	//16λ�����������Ӿͷŵ��������������棨��16λ��
	
	//���ù̼��⺯��
	USART_SendData(pUSARTx,temp_h);//�ȷ��͸�8λ
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);//�ȴ����ݷ������

	USART_SendData(pUSARTx,temp_l);//�ٷ��͵�8λ
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);//�ȴ����ݷ������

}


//��ʱ�����ݺܶ���Ҫ�������飺����������8λ�� 
//����8λ���ݵ�����--��Ҫдһ��ѭ�������÷���һ���ֽڵĺ�������
//����Ļ��������ľ���һ��ָ����

//����һ����������
void Usart_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint8_t num)
{
	//ÿ����Ҫ���Ͷ������ݣ�ͨ���β�num��������num�������8λ�ģ���ô������෢��255��
	int i;
	for(i=0;i<num;i++)
	{
		//���÷���һ���ֽں����������ݣ���������д�������ԣ�
		//Usart_SendByte(USART1,*array++);
		Usart_SendByte(USART1,array[i]);//ÿ��ֻ�ܷ���8λ����
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);//�ȴ��������
}
//�жϷ���һ���ֽڵ����ݱ�־λ��USART_FLAG_TXE
//�жϷ���һ�����ֽڵ����ݱ�־λ��USART_FLAG_TC



//�����ַ���
void Usart_SendStr(USART_TypeDef* pUSARTx,uint8_t *str)//ָ�����ڣ�Ҫ���͵��ַ�������
{
	uint8_t i=0;
	//ʹ��do-whileѭ����do��ʱ���Ѿ���ʼ������
	do{
		//��Ҫ���÷���һ���ֽں���
		Usart_SendByte(USART1,*(str+i));//����һ��֮��ָ���ַ����һ��
		i++;
	}while(*(str+i)!='\0');//����β������'\0'Ϊ�棬��������
	//���='\0'��ʾ�������
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);//�ȴ��������
}



////�жϷ��������������ַ���ʲô���ݣ��ͻ᷵��ʲô���ݣ�
//void USART1_IRQHandler() 
//{
//	u8 ucTemp;
//	if(USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET)
//	{
//		ucTemp=USART_ReceiveData(USART1);
//		USART_SendData(USART1, ucTemp);
//	}
//}



////ʹ���Զ���Э�����ʮ����������

//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//		u8 Res,i;
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
//		{
//			Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//			
//			if(count==0)//����ǽ��յĵ�һ������
//			{
//				table_data[count]=Res;//����һ�����ݴ浽�����е�һԪ��
//				if(table_data[0]==0x2c)//�жϽ��յĵ�һ�������ǲ���ʮ������0X2C
//				  count++;//�����һ��������0X2C���ʾ��ȷ����+1
//			}
//			else if(count==1)//��һ�����ݽ�����ȷ������£��жϵڶ�������
//			{
//				if(Res==0xe4)//����ս��յ�������0xE4���ʾ������ȷ
//				{
//					table_data[count]=Res;//�����ݴ��浽����ڶ���Ԫ��λ��
//					count++;//�������ݼ���+1
//				}
//				else//����ڶ����ַ�����0XE4��������㣬���½���
//					count=0;
//			}
//			else if(count==2&&Res==0)//���ǰ����������ȷ�����յĵ�����������0����������������½�������
//			{
//				count=0;
//			}
//			else if(count>1&&count<9)//���ǿ��Խ������ݵķ�Χ��ֻҪcount�����ݿɽ������ݷ�Χ�ڼ��ɽ��д�������
//			{
//				table_data[count]=Res;
//				count++;
//			}
//			else if(count>=9)//����������ݳ��������С�����������½���
//			{
//				count=0;
//			}		
//   } 
//	 
//		memset(table_cp, 0, sizeof(table_data));//��ʹ������table_cpʱ���
//		for(i=0;i<9;i++)//�ѽ��յ������ݸ��Ƶ�table_cp������
//		{
//			 table_cp[i]= table_data[i];
//		
//	}
//} 



	


UART_BUF buf_uart1;     //CH340
UART_BUF buf_uart2;     //TTL
UART_BUF buf_uart3;     //NBIOT

void UART1_send_byte(char data)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, data);
}
void UART2_send_byte(char data)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, data);
}

void UART3_send_byte(char data)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, data);
}


//��ʼ��IO ��3
//bound:������
void uart3_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ�ܣ�GPIOAʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//USART3
		USART_DeInit(USART3);  //��λ����3
	 //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

   //Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������
	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}
void Uart1_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//�ȴ�������� 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}


void Uart2_SendStr(char*SendBuf)//����2��ӡ����
{
	while(*SendBuf)
	{
        while((USART2->SR&0X40)==0);//�ȴ�������� 
        USART2->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}


void Uart3_SendStr(char*SendBuf)//����3��ӡ����
{
	while(*SendBuf)
	{
        while((USART3->SR&0X40)==0);//�ȴ�������� 
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}


//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
//    {
//        buf_uart1.buf[buf_uart1.index++] =USART_ReceiveData(USART1);//����ģ�������
//    }
//} 

void nbiot_receive_process_event(unsigned char ch )     //����2��nbiot��
{
    if(buf_uart3.index >= BUFLEN)
    {
        buf_uart3.index = 0 ;
    }
    else
    {
        buf_uart3.buf[buf_uart3.index++] = ch;
    }
}



void USART3_IRQHandler(void)                	//����2�жϷ������
	{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //����ģ�鷵�ص�����  �ߵ�2-3
		{    
         nbiot_receive_process_event(USART_ReceiveData(USART3));
    } 
 
} 


#endif


