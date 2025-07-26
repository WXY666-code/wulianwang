#include "stm32f10x.h"                  // Device header

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "modbus_timer.h"
#include "rs485.h"
#include "modbus.h"
#include "OLED.h"
#include "MyRTC.h"
#include "CountSensor.h"

#include "data.h"

#include "stdio.h"
#include "stm32f10x_flash.h"
#include "timer.h"
#include "stm32f10x_tim.h"
#include "BC26.h"
#include "wdg.h"

#include "malloc.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"

#include "sd.h"

#include "Serial.h"

#include <kalman.h>


	DATA data;

		FRESULT res_sd;                   /* �ļ�������� */
		FIL fnew;						  /* �ļ����� */
		UINT fnum;            			  /* �ļ��ɹ���д���� */
//		BYTE ReadBuffer[1024]={0};        /* �������� */
//   	BYTE WriteBuffer[] =              /* д������*/
//    	"111128484652niybyibyb\r\n";  
		char str_t[150];

		char flag =0; //��ͷ���ļ�������־λ

		uint16_t Num = 0;			//�����ڶ�ʱ���ж��������ı���
		
		extern MODBUS modbus;
		
		
		char oled_flag = 0 ;  //OLEDҳ���־λ
		
        char flag_oled_print = 1 ;   //OLED��ʾ��־λ

#define DATA_SIZE 21
		uint8_t rx_buffer[21];
		uint16_t rx_counter = 0;
		uint8_t data_ready = 0;
		
		
		double Q = 0.001;        // ��������Э����
		double R = 0.1;          // ��������Э����
		//1.Pressure;//����ѹ
		double init_x1 = 0.0;     // ��ʼ״̬����
		double init_P1 = 1.0;     // ��ʼ�������Э����
        //2.Ligth;//����ǿ��
        double init_x2 = 0.0;     // ��ʼ״̬����
		double init_P2 = 1.0;     // ��ʼ�������Э����
		//3.UVI;//������ָ��
		double init_x3 = 0.0;     // ��ʼ״̬����
		double init_P3 = 1.0;     // ��ʼ�������Э����
         //kalman
	
        KalmanFilter kf1;
	    KalmanFilter kf2;
	    KalmanFilter kf3;
		

		
		
/*

//����1�鿴�ӻ�01������
//����2�鿴�ӻ�02������
//����3�鿴�ӻ�03������
//����4�������л����ӻ�ģʽ�����豸��Ϊ�ӻ���ַ0x02��

//�����鿴ĳ���ӻ���n���Ĵ�������
Host_Read03_slave(0x02,0x0000,0x0006);//����1�ӻ���ַ������2��ʼ��ַ������3ʱ�Ĵ�������




//����İ����л�����ģʽΪ�ӻ�ģʽ
//int slave=0x90;//�ӻ�id
//int host_slave_flag=0;//0-Ĭ������±��豸��������1-���豸�л�Ϊ�ӻ�ģʽ
//uint8_t key_value=0;//��һ������������1-4
//uint8_t key_flag=0;//key_flag����0��ʾ����û�а�������(��ʱһֱ�鿴�ӻ�1������)-----�������Ӵ˱�־�����س������Ҫ��λ����



//����1�鿴�ӻ�01������
//����2�鿴�ӻ�02������
//����3�鿴�ӻ�03������
//����4�������л����ӻ�ģʽ�����豸��Ϊ�ӻ���ַ0x02��
//void key_Send()
//{
//		
//		key_value=KEY_Scan(1);
//		switch(key_value)
//		{
//			case 1:
//				slave=0x90;key_flag=1;break;//�ӻ���ַ01
//			case 2:
//				slave=2;key_flag=1;break;//�ӻ���ַ02
//			case 3:
//				slave=3;key_flag=1;break;//�ӻ���ַ03
//			case 4:
//				host_slave_flag=1;key_flag=1;break;//�л�Ϊ�ӻ�ģʽ
//			
//		}
//}



*/
int main(void)
{	
		RCC->APB2ENR = 0X00000010;
		GPIOC->CRH = 0X00300000;
		GPIOC->ODR = 0X00000000; //Ϩ��0X00002000 ������0X00000000 ���ڴ��Լ�ⵥƬ����������
		
		uart_init(9600);	 //����1��ʼ��Ϊ9600��ֻ������ӡ�������ݣ�
	
		OLED_Init();		//OLED��ʼ��

		MyRTC_Init();		//RTC��ʼ��

		CountSensor_Init();		//������������ʼ��
	
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 OLED_ShowString(4, 1, "BC26 LodING...");
		usart_printf("ready\r\n");
        OLED_ShowString(4, 1, "BC26 LoadING...");

	
		u32 total,free;
	
	    exfuns_init();			//Ϊfatfs��ر��������ڴ�
		mem_init();				//��ʼ���ڴ��
	
	    sd_initialize();        //����fatfs�ļ�ϵͳ
	
	    while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
		{
			OLED_ShowString(0,0,"Fatfs Error!");
			delay_ms(200);
			OLED_Clear();//�����ʾ				  
			delay_ms(200);
		//LED0=!LED0;//DS0��˸
		}
		
		OLED_ShowString(1,1,"FATFS OK!" );
		OLED_ShowString(2,1,"Total:       MB");
		OLED_ShowString(3,1,"Free :       MB");
		OLED_ShowNum(2, 7, total>>10, 5);
		OLED_ShowNum(3, 7, free>>10, 5);
		
		

//		Modbus_uart2_init(9600);//��ʼ��modbus����2��485��������
//		Modbus_TIME3_Init(7200-1,10-1);//��ʱ����ʼ������1����װ����������2�Ƿ�Ƶϵ��//1ms�ж�һ��
//		Modbus_Init();//MODBUS��ʼ��--��������Ϊ�ӻ��豸��ַ������Ҫƥ��Ĵӻ���ַ
	 
	  
    	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		
		LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�


		BC26CTR_Init();        //��ʼ��BC26��PWR����


		uart3_init(115200);
//		Uart1_SendStr("UART3 Init Successful\r\n");
//      IWDG_Init(7,625);    //8Sһ��
    
	   
       OLED_ShowString(4, 1, "BC26 LOADING...");



		while(BC26_Init()); //��Ƭ����ģ�������
		LED0=0; //PA8 ���µ�Ƭ���Ƿ��ģ��ͨ�ųɹ�
		BC26_PDPACT();	//�����Ƿ񼤻�
		BC26_RegALIYUNIOT();//ע�ᵽ��ƽ̨
		LED1=0; //PB15 //����ģ���Ƿ񼤻�ɹ� ע�ᵽ������
	

	    Timer_Init();       //������ʱ�ж�
		
		
		OLED_Clear();
		OLED_ShowString(3, 1, "Date:XXXX-XX-XX");
		OLED_ShowString(4, 1, "Time:XX:XX:XX");
		
/*		
//			data.Ligth =  17670;
//			data.UVI =  1.3;
//			data.Temp = 26.2;
//			data.Humi = 60;
//			data.Wind_speed = 10;
//			data.Gust_speed = 8;
//			data.Wind_direction  = 150;
//			data.Rainfall_M =1.8;
//			data.Rainfall_F =8;
//		
//		
//		data_str();         //ƴ������
//				
//		delay_ms(4000);
//			sd_write();
//		
//		data_str();
//		delay_ms(4000);
//			sd_write();
//		
//		data_str();
//		delay_ms(4000);
//			sd_write();
*/
		
		
		Data_Oled_Init();
		
		Serial_Init();
		
		
		/*IWDG��ʼ��*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//�������Ź�дʹ��
	IWDG_SetPrescaler(IWDG_Prescaler_256);			//����Ԥ��ƵΪ256
	IWDG_SetReload(3124);							//������װֵΪ3124���������Ź��ĳ�ʱʱ��Ϊ20000ms
	IWDG_ReloadCounter();							//��װ��������ι��
	IWDG_Enable();									//�������Ź�ʹ��
	
	//Kalman
	    KalmanFilter_Init(&kf1, init_x1, init_P1, Q, R);
	    KalmanFilter_Init(&kf2, init_x2, init_P2, Q, R);
	    KalmanFilter_Init(&kf3, init_x3, init_P3, Q, R);
		
		/*��ʼ���ж�*/
//		float a = data.K_Pressure;
		int16_t *pa1 = (int16_t *)(&data.K_Pressure);
		int16_t *pa2 = pa1+1;
//		float b = data.K_Ligth;
		int16_t *pb1 = (int16_t *)(&data.K_Ligth);
		int16_t *pb2 = pb1+1;
//		float c = data.K_UVI;
		int16_t *pc1 = (int16_t *)(&data.K_UVI);
		int16_t *pc2 = pc1+1;
	
		if(BKP_ReadBackupRegister(BKP_DR3)!=*pa1 | BKP_ReadBackupRegister(BKP_DR4)!=*pa2)
		{
		*pa1=BKP_ReadBackupRegister(BKP_DR3);
		*pa2=BKP_ReadBackupRegister(BKP_DR4);	
		}
		if(BKP_ReadBackupRegister(BKP_DR5)!=*pb1 | BKP_ReadBackupRegister(BKP_DR6)!=*pb2)
		{
		*pb1=BKP_ReadBackupRegister(BKP_DR5);
		*pb2=BKP_ReadBackupRegister(BKP_DR6);	
		}	
		if(BKP_ReadBackupRegister(BKP_DR7)!=*pc1 | BKP_ReadBackupRegister(BKP_DR8)!=*pc2)
		{
		*pc1=BKP_ReadBackupRegister(BKP_DR7);
		*pc2=BKP_ReadBackupRegister(BKP_DR8);	
		} 
		
		
		
	while(1)
	{ 
/*		
		
//		key_Send();//����ɨ��
//		
//		if(host_slave_flag==1)//���ǰ���4�����ˣ������Ǵӻ�ģʽ��led4��ͣ����˸��
//		{
//			 Modbus_Event();//Modbus�¼�������(ִ�ж�����д���ж�)--�ӻ���ַ01
//			 if(Reg[3]==0x0A)//��Ϊ�ӻ�����Ĵ����ĵ�ַ00 03�յ���0x0A�������LED3
//			 {
//				 LED1=0;
//			 }
//			 if(Reg[3]==0x0B)
//			 {
//				 LED1=1;
//			 }
//			LED4=~LED4;
//			delay_ms(100);
//		}
//		else if(key_flag==0)//��ʾ������û�а������£�����ģʽ�鿴�ӻ���ַ01�����ݣ�
//		{
*/		
					
				
				
				MyRTC_ReadTime();							//RTC��ȡʱ�䣬���µ�ʱ��洢��MyRTC_Time������

				OLED_ShowNum(3, 6, MyRTC_Time[0], 4);		//��ʾMyRTC_Time�����е�ʱ��ֵ����
				OLED_ShowNum(3, 11, MyRTC_Time[1], 2);		//��
				OLED_ShowNum(3, 14, MyRTC_Time[2], 2);		//��
				OLED_ShowNum(4, 6, MyRTC_Time[3], 2);		//ʱ
				OLED_ShowNum(4, 9, MyRTC_Time[4], 2);		//��
				OLED_ShowNum(4, 12, MyRTC_Time[5], 2);		//��
				
		if (Serial_GetRxFlag() == 1)			//��鴮�ڽ������ݵı�־λ
		{
					// �����ﴦ����յ���21�ֽ�����
					// process_data(rx_buffer);

			//ʾ����24 66 65 E2 4D 37 0D 03 00 16 00 00 00 5F 42 31 4D 01 8F 6A FA)
			//����ʵ����24 CB 90 62 B8 49 00 00 00 00 00 27 00 A8 4B 36 32 01 88 CB 54 
			//ʶ�����1st��2nd�� 24 (identify tx type)
			//��ȫ�룬3rd��4th�� 66 (security code)

            if(rx_buffer[0] == 0x24 && rx_buffer[1] == 0xD6)//0x24Ϊ����ʶ�����0x66Ϊ�豸��Ӧ�İ�ȫ��
            {
			//����5th��6th��7th�� 65E (wind direction)
			//(explanation: 65E(HEX) =0110, 0101,1110 (Binary)
			//(Bit8=1, Bit 7=0, Bit 6=1, Bit 5=1, Bit 4=0, Bit 3=0, Bit 2=1, Bit 1=0, Bit 0=1,)
			//Wind direction is:B1 0110 0101= 357 (decimal)
			//So, wind direction is: 357��
                data.Wind_direction =((int)(rx_buffer[3]&0x80)*2+rx_buffer[2]);


			//�¶ȣ�8th,9th,10th�� 24D (Temperature)
			//(explanation:24D (HEX)= B010 0100 1101 = 589(Decimal)
			//calculation�� (589-400)/10=18.9
			//so temperature is: 18.9��
                data.Temp=((float)((rx_buffer[3]&0x07)*256+rx_buffer[4]-400)/10);


			//���ʪ�ȣ�11th��12th��37 (Humidity)
			//��Explanation:37(HEX)=55(D), so it is 55%��
                data.Humi=(int)rx_buffer[5];


			//���٣�13th��14 th��0D (wind speed)
			//(explanation:
			//00 (HEX) = B 0000 1101
			//(Bit8=0, Bit 7=0, Bit 6=0, Bit 5=0, Bit 4=0, Bit 3=1, Bit 2=1, Bit 1=0, Bit 0=1,)
			//So, the data is: B0 0000 1101 = 13 (D)
			//calculation�� 13/8*1.12=1.82
			//So, wind speed is: 1.82 m/s.
                data.Wind_speed=(((float)(rx_buffer[6])/8))*1.12;

			//������ʣ�15th��16th��03 (gust speed)
			//(explanation: gust speed: 3 *1.12= 3.36 m/s )
                data.Gust_speed=((float)((rx_buffer[7])))*1.12;

			//�ۼƽ�������17th-20 th�� 0016 (accumulation rainfall)
			//(explanation: accumulation rainfall: 22*0.3=6.6 mm )
                data.Rainfall_M=((float)((rx_buffer[8]*256+rx_buffer[9])))*0.3;

			//������ָ����21th-24th�� 0000 (UVI)
			//(explanation: UV: 0 uW/cm2)
                data.UVI=(int)(rx_buffer[10]*256+rx_buffer[11]);

			//���նȣ�25th-30th�� 00 5F 42 (LIGHT)
			//(explanation: Light:24386/10=2438.6 LUX)
                data.Ligth=((float)(rx_buffer[12]*256*256+rx_buffer[13]*256+rx_buffer[14])/10);

			//����ѹ��35th-40th: 018F6A (barometric pressure )
			//(explanation: pressure:018F6A=102250, 102250/100=1022.50 hpa)
                data.Pressure=((float)(rx_buffer[17]*256*256+rx_buffer[18]*256+rx_buffer[19])/100);

               			 //kalman
            data.K_Pressure   = KalmanFilter_Update(&kf1, data.Pressure);
            data.K_Ligth      = KalmanFilter_Update(&kf2, data.Ligth);
            data.K_UVI        = KalmanFilter_Update(&kf3, data.UVI);
			
			float A = data.K_Pressure ;
			int16_t *pA1 = (int16_t *)(&A);		//���aǰ16λ��ַ
			int16_t *pA2 = pA1+1;		//���a��16λ��ַ

			BKP_WriteBackupRegister(BKP_DR3,*pA1); //��16λ
			BKP_WriteBackupRegister(BKP_DR4,*pA2); //��16λ
			
			float B = data.K_Ligth;
			int16_t *pB1 = (int16_t *)(&B);
			int16_t *pB2 = pB1+1;
			
			BKP_WriteBackupRegister(BKP_DR5,*pB1); //��16λ
			BKP_WriteBackupRegister(BKP_DR6,*pB2); //��16λ
			
			float C = data.K_UVI;
			int16_t *pC1 = (int16_t *)(&C);
			int16_t *pC2 = pC1+1;
			
			BKP_WriteBackupRegister(BKP_DR7,*pC1); //��16λ
			BKP_WriteBackupRegister(BKP_DR8,*pC2);//��16λ




					
					// ׼����һ�ν���
					rx_counter = 0;
					data_ready = 0;
//					USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
									
				}
		}	
		
				
//				OLED_ShowNum(2, 13, CountSensor_Get(), 2);		//OLED����ˢ����ʾCountSensor_Get�ķ���ֵ

			//��ȡ����ʽ����������
	            data.Rainfall_F = (float) (0.5*CountSensor_Get());
			
		
//            //kalman
//            data.K_Pressure   = KalmanFilter_Update(&kf1, data.Pressure);
//            data.K_Ligth      = KalmanFilter_Update(&kf2, data.Ligth);
//            data.K_UVI        = KalmanFilter_Update(&kf3, data.UVI);


		
			//��ʾ���ݣ���ҳ��
				if(flag_oled_print == NO_PRINT)
				{
					OLED_Clear_half_up();
					data_oled();
					flag_oled_print = YES_PRINT;
				};
				

			// 1. �ر�ȫ���жϣ���ֹ�����־ʱ�����жϴ�ϣ�
				__disable_irq();

			// 2. ���NVIC�������жϹ����־
				for (uint8_t i = 0; i < 8; i++) {
				NVIC->ICPR[i] = 0xFFFFFFFF; // ��������־
//       	  NVIC->ICER[i] = 0xFFFFFFFF; // ���������жϣ����ղ�����
				}
//				NVIC_DisableIRQ(USART2_IRQn);
//				NVIC_DisableIRQ(TIM3_IRQn);                     //�����ж�
		
//	 // 1. �ر�USART2�����ж�
//    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//    
//    // 2. ���USART2�����жϱ�־
//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    
//    // 3. ���NVIC��USART2�жϹ����־
//    NVIC_ClearPendingIRQ(USART2_IRQn);
//	

//    // 3. ��������жϱ�־��������ӣ�
//    TIM2->SR = 0;                // ��ʱ��״̬����
//    EXTI->PR = 0xFFFFFFFF;       // �������EXTI�����־
////    RTC->CRL &= ~RTC_CRL_ALRAF;  // ���RTC���ӱ�־�������������
//    ADC1->SR = 0;                // ���ADC��־��������ADC��

//    // 4. ����ʹ�ܱ�Ҫ���ж�
    NVIC->ISER[EXTI0_IRQn >> 5] |= (1 << (EXTI0_IRQn & 0x1F));    // �ⲿ�ж�
    NVIC->ISER[TIM2_IRQn >> 5] |= (1 << (TIM2_IRQn & 0x1F));      // ��ʱ���ж�
//    NVIC->ISER[RTC_Alarm_IRQn >> 5] |= (1 << (RTC_Alarm_IRQn & 0x1F)); // RTC�ж�




    
	
			// 5. �ָ�ȫ���ж�
				__enable_irq();
	
			// �ر�SysTick��ֱ�ӼĴ���������
				SysTick->CTRL = 0;  // ���ʹ��λ���ж�λ
				__WFI();								//ִ��WFIָ�CPU˯�ߣ����ȴ��жϻ���
		
			//	printf("WFI\n");


/*
//              time0 = RTC_GetCounter();
//				time2 = RTC_GetCounter();
//				time3 = RTC_GetCounter();
				
//				if(time1 + 10 < time2)
//				{
//					time1 = RTC_GetCounter();
				
				
////				//���Զ�ʱmodbus��ȡ
////				for(time1 = RTC_GetCounter();time0  < time1 - 10 ; time0 = RTC_GetCounter())
////				{
//					
//					//����1���鿴��i���ӻ�����
//					Host_Read03_slave(0x90,0x0165,0x0008);//����2��ʼ��ַ������3�Ĵ�������
//					if(modbus.Host_send_flag)
//					{
//					modbus.Host_Sendtime=0;//������Ϻ�������㣨�����ϴε�ʱ�䣩
//					modbus.Host_time_flag=0;//�������ݱ�־λ����
//					modbus.Host_send_flag=0;//��շ��ͽ������ݱ�־λ
//				
//					HOST_ModbusRX();//�������ݽ��д���
//					}
////					LED2=~LED2;
//					delay_ms(1000);
//				
////			    }
//		}
			
//		else
//		{
//				if(modbus.Host_time_flag)//ÿ1s����һ������
//				{
//			
//					//����1���鿴��i���ӻ�����
//					Host_Read03_slave(slave,0x0165,0x0008);//������2��ʼ��ַ������3�Ĵ�������
//					if(modbus.Host_send_flag)
//					{
//						modbus.Host_Sendtime=0;//������Ϻ�������㣨�����ϴε�ʱ�䣩
//						modbus.Host_time_flag=0;//�������ݱ�־λ����
//						modbus.Host_send_flag=0;//��շ��ͽ������ݱ�־λ
//						
//						HOST_ModbusRX();//�������ݽ��д���

//					}
//					LED3=~LED3;
//				}
//			
//		
//		}
		
			
//				//���Զ�ʱ�ϴ�
//				
//				if(time2  < time3 - 30)
//				{
//					time2 = RTC_GetCounter();
				
//				//�ϴ�������
//				BC26_DATA();
				
//				}

*/
		
	
	
	       
		   IWDG_ReloadCounter();						//��װ��������ι��
	
	}
		
}


/**
  * ��    ����TIM2�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//�ж��Ƿ���TIM2�ĸ����¼��������ж�
	{
//		int i;
		Num ++;	
		if(Num == 12)    // Num=1 ->10��  , ÿ120����ա����Ͳ��洢��(Modbus)�������̴�Ŷ�����Ҫ10��
		{	
			    //��ȡ����ʽ����������
//	            data.Rainfall_F = (float) (0.2*CountSensor_Get());
			    data.Rainfall_F = (float) (0.5*CountSensor_Get());
//				delay_ms(100);
			
//			        NVIC_EnableIRQ(USART2_IRQn);
//					NVIC_EnableIRQ(TIM3_IRQn);        //�����ж�
//			
//			  		Modbus_uart2_init(9600);//��ʼ��modbus����2��485��������
//					Modbus_TIME3_Init(7200-1,10-1);//��ʱ����ʼ������1����װ����������2�Ƿ�Ƶϵ��//1ms�ж�һ��
//					Modbus_Init();//MODBUS��ʼ��--��������Ϊ�ӻ��豸��ַ������Ҫƥ��Ĵӻ���ַ
			
//			    //������ȡ5��
//				for(i=0;i<5;i++)
//				{//����1���鿴��k���ӻ�����
//					Host_Read03_slave(0x90,0x0165,0x0008);//����2��ʼ��ַ������3�Ĵ�������
//					if(modbus.Host_send_flag)
//					{
//						modbus.Host_Sendtime=0;//������Ϻ�������㣨�����ϴε�ʱ�䣩
//						modbus.Host_time_flag=0;//�������ݱ�־λ����
//						modbus.Host_send_flag=0;//��շ��ͽ������ݱ�־λ
//				
//						HOST_ModbusRX();//�������ݽ��д���
//					}
//					delay_ms(500);
//				};
                
				
				//ƴ������
				data_str();         

					
//			//�ϴ�������	
			BC26_DATA();
				
		    //д������
			sd_write();	
 
				
					
			Num = 0;
		}			
													
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//���TIM2�����¼����жϱ�־λ
															//�жϱ�־λ�������
															//�����жϽ��������ϵش�����������������
	}
}













