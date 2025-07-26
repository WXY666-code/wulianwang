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

		FRESULT res_sd;                   /* 文件操作结果 */
		FIL fnew;						  /* 文件对象 */
		UINT fnum;            			  /* 文件成功读写数量 */
//		BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//   	BYTE WriteBuffer[] =              /* 写缓冲区*/
//    	"111128484652niybyibyb\r\n";  
		char str_t[150];

		char flag =0; //表头与文件创建标志位

		uint16_t Num = 0;			//定义在定时器中断里自增的变量
		
		extern MODBUS modbus;
		
		
		char oled_flag = 0 ;  //OLED页码标志位
		
        char flag_oled_print = 1 ;   //OLED显示标志位

#define DATA_SIZE 21
		uint8_t rx_buffer[21];
		uint16_t rx_counter = 0;
		uint8_t data_ready = 0;
		
		
		double Q = 0.001;        // 过程噪声协方差
		double R = 0.1;          // 测量噪声协方差
		//1.Pressure;//大气压
		double init_x1 = 0.0;     // 初始状态估计
		double init_P1 = 1.0;     // 初始估计误差协方差
        //2.Ligth;//光照强度
        double init_x2 = 0.0;     // 初始状态估计
		double init_P2 = 1.0;     // 初始估计误差协方差
		//3.UVI;//紫外线指数
		double init_x3 = 0.0;     // 初始状态估计
		double init_P3 = 1.0;     // 初始估计误差协方差
         //kalman
	
        KalmanFilter kf1;
	    KalmanFilter kf2;
	    KalmanFilter kf3;
		

		
		
/*

//按键1查看从机01的数据
//按键2查看从机02的数据
//按键3查看从机03的数据
//按键4由主机切换到从机模式（此设备作为从机地址0x02）

//主机查看某个从机的n个寄存器数据
Host_Read03_slave(0x02,0x0000,0x0006);//参数1从机地址，参数2起始地址，参数3时寄存器个数




//加入的按键切换主机模式为从机模式
//int slave=0x90;//从机id
//int host_slave_flag=0;//0-默认情况下本设备是主机，1-本设备切换为从机模式
//uint8_t key_value=0;//哪一个按键按下了1-4
//uint8_t key_flag=0;//key_flag等于0表示从来没有按键按下(此时一直查看从机1的数据)-----如果不添加此标志，下载程序后需要复位操作



//按键1查看从机01的数据
//按键2查看从机02的数据
//按键3查看从机03的数据
//按键4由主机切换到从机模式（此设备作为从机地址0x02）
//void key_Send()
//{
//		
//		key_value=KEY_Scan(1);
//		switch(key_value)
//		{
//			case 1:
//				slave=0x90;key_flag=1;break;//从机地址01
//			case 2:
//				slave=2;key_flag=1;break;//从机地址02
//			case 3:
//				slave=3;key_flag=1;break;//从机地址03
//			case 4:
//				host_slave_flag=1;key_flag=1;break;//切换为从机模式
//			
//		}
//}



*/
int main(void)
{	
		RCC->APB2ENR = 0X00000010;
		GPIOC->CRH = 0X00300000;
		GPIOC->ODR = 0X00000000; //熄灭：0X00002000 点亮：0X00000000 用于粗略检测单片机正常工作
		
		uart_init(9600);	 //串口1初始化为9600（只用来打印测试数据）
	
		OLED_Init();		//OLED初始化

		MyRTC_Init();		//RTC初始化

		CountSensor_Init();		//计数传感器初始化
	
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 OLED_ShowString(4, 1, "BC26 LodING...");
		usart_printf("ready\r\n");
        OLED_ShowString(4, 1, "BC26 LoadING...");

	
		u32 total,free;
	
	    exfuns_init();			//为fatfs相关变量申请内存
		mem_init();				//初始化内存池
	
	    sd_initialize();        //挂载fatfs文件系统
	
	    while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
		{
			OLED_ShowString(0,0,"Fatfs Error!");
			delay_ms(200);
			OLED_Clear();//清除显示				  
			delay_ms(200);
		//LED0=!LED0;//DS0闪烁
		}
		
		OLED_ShowString(1,1,"FATFS OK!" );
		OLED_ShowString(2,1,"Total:       MB");
		OLED_ShowString(3,1,"Free :       MB");
		OLED_ShowNum(2, 7, total>>10, 5);
		OLED_ShowNum(3, 7, free>>10, 5);
		
		

//		Modbus_uart2_init(9600);//初始化modbus串口2和485控制引脚
//		Modbus_TIME3_Init(7200-1,10-1);//定时器初始化参数1是重装载数，参数2是分频系数//1ms中断一次
//		Modbus_Init();//MODBUS初始化--本机做作为从机设备地址，本机要匹配的从机地址
	 
	  
    	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		
		LED_Init();		  		//初始化与LED连接的硬件接口


		BC26CTR_Init();        //初始化BC26的PWR引脚


		uart3_init(115200);
//		Uart1_SendStr("UART3 Init Successful\r\n");
//      IWDG_Init(7,625);    //8S一次
    
	   
       OLED_ShowString(4, 1, "BC26 LOADING...");



		while(BC26_Init()); //单片机和模块的链接
		LED0=0; //PA8 看下单片机是否和模块通信成功
		BC26_PDPACT();	//看下是否激活
		BC26_RegALIYUNIOT();//注册到云平台
		LED1=0; //PB15 //看下模块是否激活成功 注册到网络了
	

	    Timer_Init();       //开启定时中断
		
		
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
//		data_str();         //拼接数据
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
		
		
		/*IWDG初始化*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//独立看门狗写使能
	IWDG_SetPrescaler(IWDG_Prescaler_256);			//设置预分频为256
	IWDG_SetReload(3124);							//设置重装值为3124，独立看门狗的超时时间为20000ms
	IWDG_ReloadCounter();							//重装计数器，喂狗
	IWDG_Enable();									//独立看门狗使能
	
	//Kalman
	    KalmanFilter_Init(&kf1, init_x1, init_P1, Q, R);
	    KalmanFilter_Init(&kf2, init_x2, init_P2, Q, R);
	    KalmanFilter_Init(&kf3, init_x3, init_P3, Q, R);
		
		/*初始化判断*/
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
		
//		key_Send();//按键扫描
//		
//		if(host_slave_flag==1)//这是按键4按下了，表明是从机模式（led4不停的闪烁）
//		{
//			 Modbus_Event();//Modbus事件处理函数(执行读或者写的判断)--从机地址01
//			 if(Reg[3]==0x0A)//作为从机如果寄存器的地址00 03收到了0x0A数据则打开LED3
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
//		else if(key_flag==0)//表示开机后没有按键按下（主机模式查看从机地址01的数据）
//		{
*/		
					
				
				
				MyRTC_ReadTime();							//RTC读取时间，最新的时间存储到MyRTC_Time数组中

				OLED_ShowNum(3, 6, MyRTC_Time[0], 4);		//显示MyRTC_Time数组中的时间值，年
				OLED_ShowNum(3, 11, MyRTC_Time[1], 2);		//月
				OLED_ShowNum(3, 14, MyRTC_Time[2], 2);		//日
				OLED_ShowNum(4, 6, MyRTC_Time[3], 2);		//时
				OLED_ShowNum(4, 9, MyRTC_Time[4], 2);		//分
				OLED_ShowNum(4, 12, MyRTC_Time[5], 2);		//秒
				
		if (Serial_GetRxFlag() == 1)			//检查串口接收数据的标志位
		{
					// 在这里处理接收到的21字节数据
					// process_data(rx_buffer);

			//示例：24 66 65 E2 4D 37 0D 03 00 16 00 00 00 5F 42 31 4D 01 8F 6A FA)
			//本机实例：24 CB 90 62 B8 49 00 00 00 00 00 27 00 A8 4B 36 32 01 88 CB 54 
			//识别符，1st、2nd： 24 (identify tx type)
			//安全码，3rd、4th： 66 (security code)

            if(rx_buffer[0] == 0x24 && rx_buffer[1] == 0xD6)//0x24为发送识别符，0x66为设备对应的安全码
            {
			//风向，5th、6th、7th： 65E (wind direction)
			//(explanation: 65E(HEX) =0110, 0101,1110 (Binary)
			//(Bit8=1, Bit 7=0, Bit 6=1, Bit 5=1, Bit 4=0, Bit 3=0, Bit 2=1, Bit 1=0, Bit 0=1,)
			//Wind direction is:B1 0110 0101= 357 (decimal)
			//So, wind direction is: 357°
                data.Wind_direction =((int)(rx_buffer[3]&0x80)*2+rx_buffer[2]);


			//温度，8th,9th,10th： 24D (Temperature)
			//(explanation:24D (HEX)= B010 0100 1101 = 589(Decimal)
			//calculation： (589-400)/10=18.9
			//so temperature is: 18.9℃
                data.Temp=((float)((rx_buffer[3]&0x07)*256+rx_buffer[4]-400)/10);


			//相对湿度，11th、12th：37 (Humidity)
			//（Explanation:37(HEX)=55(D), so it is 55%）
                data.Humi=(int)rx_buffer[5];


			//风速，13th、14 th：0D (wind speed)
			//(explanation:
			//00 (HEX) = B 0000 1101
			//(Bit8=0, Bit 7=0, Bit 6=0, Bit 5=0, Bit 4=0, Bit 3=1, Bit 2=1, Bit 1=0, Bit 0=1,)
			//So, the data is: B0 0000 1101 = 13 (D)
			//calculation： 13/8*1.12=1.82
			//So, wind speed is: 1.82 m/s.
                data.Wind_speed=(((float)(rx_buffer[6])/8))*1.12;

			//阵风速率，15th、16th：03 (gust speed)
			//(explanation: gust speed: 3 *1.12= 3.36 m/s )
                data.Gust_speed=((float)((rx_buffer[7])))*1.12;

			//累计降雨量，17th-20 th： 0016 (accumulation rainfall)
			//(explanation: accumulation rainfall: 22*0.3=6.6 mm )
                data.Rainfall_M=((float)((rx_buffer[8]*256+rx_buffer[9])))*0.3;

			//紫外线指数，21th-24th： 0000 (UVI)
			//(explanation: UV: 0 uW/cm2)
                data.UVI=(int)(rx_buffer[10]*256+rx_buffer[11]);

			//光照度，25th-30th： 00 5F 42 (LIGHT)
			//(explanation: Light:24386/10=2438.6 LUX)
                data.Ligth=((float)(rx_buffer[12]*256*256+rx_buffer[13]*256+rx_buffer[14])/10);

			//大气压，35th-40th: 018F6A (barometric pressure )
			//(explanation: pressure:018F6A=102250, 102250/100=1022.50 hpa)
                data.Pressure=((float)(rx_buffer[17]*256*256+rx_buffer[18]*256+rx_buffer[19])/100);

               			 //kalman
            data.K_Pressure   = KalmanFilter_Update(&kf1, data.Pressure);
            data.K_Ligth      = KalmanFilter_Update(&kf2, data.Ligth);
            data.K_UVI        = KalmanFilter_Update(&kf3, data.UVI);
			
			float A = data.K_Pressure ;
			int16_t *pA1 = (int16_t *)(&A);		//获得a前16位地址
			int16_t *pA2 = pA1+1;		//获得a后16位地址

			BKP_WriteBackupRegister(BKP_DR3,*pA1); //高16位
			BKP_WriteBackupRegister(BKP_DR4,*pA2); //低16位
			
			float B = data.K_Ligth;
			int16_t *pB1 = (int16_t *)(&B);
			int16_t *pB2 = pB1+1;
			
			BKP_WriteBackupRegister(BKP_DR5,*pB1); //高16位
			BKP_WriteBackupRegister(BKP_DR6,*pB2); //低16位
			
			float C = data.K_UVI;
			int16_t *pC1 = (int16_t *)(&C);
			int16_t *pC2 = pC1+1;
			
			BKP_WriteBackupRegister(BKP_DR7,*pC1); //高16位
			BKP_WriteBackupRegister(BKP_DR8,*pC2);//低16位




					
					// 准备下一次接收
					rx_counter = 0;
					data_ready = 0;
//					USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
									
				}
		}	
		
				
//				OLED_ShowNum(2, 13, CountSensor_Get(), 2);		//OLED不断刷新显示CountSensor_Get的返回值

			//读取翻斗式雨量计数据
	            data.Rainfall_F = (float) (0.5*CountSensor_Get());
			
		
//            //kalman
//            data.K_Pressure   = KalmanFilter_Update(&kf1, data.Pressure);
//            data.K_Ligth      = KalmanFilter_Update(&kf2, data.Ligth);
//            data.K_UVI        = KalmanFilter_Update(&kf3, data.UVI);


		
			//显示数据（翻页）
				if(flag_oled_print == NO_PRINT)
				{
					OLED_Clear_half_up();
					data_oled();
					flag_oled_print = YES_PRINT;
				};
				

			// 1. 关闭全局中断（防止清除标志时被新中断打断）
				__disable_irq();

			// 2. 清除NVIC中所有中断挂起标志
				for (uint8_t i = 0; i < 8; i++) {
				NVIC->ICPR[i] = 0xFFFFFFFF; // 清除挂起标志
//       	  NVIC->ICER[i] = 0xFFFFFFFF; // 禁用所有中断（保险操作）
				}
//				NVIC_DisableIRQ(USART2_IRQn);
//				NVIC_DisableIRQ(TIM3_IRQn);                     //禁用中断
		
//	 // 1. 关闭USART2接收中断
//    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//    
//    // 2. 清除USART2接收中断标志
//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    
//    // 3. 清除NVIC中USART2中断挂起标志
//    NVIC_ClearPendingIRQ(USART2_IRQn);
//	

//    // 3. 清除外设中断标志（按需添加）
//    TIM2->SR = 0;                // 定时器状态清零
//    EXTI->PR = 0xFFFFFFFF;       // 清除所有EXTI挂起标志
////    RTC->CRL &= ~RTC_CRL_ALRAF;  // 清除RTC闹钟标志（需解锁备份域）
//    ADC1->SR = 0;                // 清除ADC标志（若启用ADC）

//    // 4. 重新使能必要的中断
    NVIC->ISER[EXTI0_IRQn >> 5] |= (1 << (EXTI0_IRQn & 0x1F));    // 外部中断
    NVIC->ISER[TIM2_IRQn >> 5] |= (1 << (TIM2_IRQn & 0x1F));      // 定时器中断
//    NVIC->ISER[RTC_Alarm_IRQn >> 5] |= (1 << (RTC_Alarm_IRQn & 0x1F)); // RTC中断




    
	
			// 5. 恢复全局中断
				__enable_irq();
	
			// 关闭SysTick（直接寄存器操作）
				SysTick->CTRL = 0;  // 清除使能位和中断位
				__WFI();								//执行WFI指令，CPU睡眠，并等待中断唤醒
		
			//	printf("WFI\n");


/*
//              time0 = RTC_GetCounter();
//				time2 = RTC_GetCounter();
//				time3 = RTC_GetCounter();
				
//				if(time1 + 10 < time2)
//				{
//					time1 = RTC_GetCounter();
				
				
////				//测试定时modbus读取
////				for(time1 = RTC_GetCounter();time0  < time1 - 10 ; time0 = RTC_GetCounter())
////				{
//					
//					//参数1：查看第i个从机数据
//					Host_Read03_slave(0x90,0x0165,0x0008);//参数2起始地址，参数3寄存器个数
//					if(modbus.Host_send_flag)
//					{
//					modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//					modbus.Host_time_flag=0;//发送数据标志位清零
//					modbus.Host_send_flag=0;//清空发送结束数据标志位
//				
//					HOST_ModbusRX();//接收数据进行处理
//					}
////					LED2=~LED2;
//					delay_ms(1000);
//				
////			    }
//		}
			
//		else
//		{
//				if(modbus.Host_time_flag)//每1s发送一次数据
//				{
//			
//					//参数1：查看第i个从机数据
//					Host_Read03_slave(slave,0x0165,0x0008);//，参数2起始地址，参数3寄存器个数
//					if(modbus.Host_send_flag)
//					{
//						modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//						modbus.Host_time_flag=0;//发送数据标志位清零
//						modbus.Host_send_flag=0;//清空发送结束数据标志位
//						
//						HOST_ModbusRX();//接收数据进行处理

//					}
//					LED3=~LED3;
//				}
//			
//		
//		}
		
			
//				//测试定时上传
//				
//				if(time2  < time3 - 30)
//				{
//					time2 = RTC_GetCounter();
				
//				//上传阿里云
//				BC26_DATA();
				
//				}

*/
		
	
	
	       
		   IWDG_ReloadCounter();						//重装计数器，喂狗
	
	}
		
}


/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
//		int i;
		Num ++;	
		if(Num == 12)    // Num=1 ->10秒  , 每120秒接收、发送并存储，(Modbus)完整流程大概额外需要10秒
		{	
			    //读取翻斗式雨量计数据
//	            data.Rainfall_F = (float) (0.2*CountSensor_Get());
			    data.Rainfall_F = (float) (0.5*CountSensor_Get());
//				delay_ms(100);
			
//			        NVIC_EnableIRQ(USART2_IRQn);
//					NVIC_EnableIRQ(TIM3_IRQn);        //启用中断
//			
//			  		Modbus_uart2_init(9600);//初始化modbus串口2和485控制引脚
//					Modbus_TIME3_Init(7200-1,10-1);//定时器初始化参数1是重装载数，参数2是分频系数//1ms中断一次
//					Modbus_Init();//MODBUS初始化--本机做作为从机设备地址，本机要匹配的从机地址
			
//			    //连续读取5次
//				for(i=0;i<5;i++)
//				{//参数1：查看第k个从机数据
//					Host_Read03_slave(0x90,0x0165,0x0008);//参数2起始地址，参数3寄存器个数
//					if(modbus.Host_send_flag)
//					{
//						modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//						modbus.Host_time_flag=0;//发送数据标志位清零
//						modbus.Host_send_flag=0;//清空发送结束数据标志位
//				
//						HOST_ModbusRX();//接收数据进行处理
//					}
//					delay_ms(500);
//				};
                
				
				//拼接数据
				data_str();         

					
//			//上传阿里云	
			BC26_DATA();
				
		    //写入数据
			sd_write();	
 
				
					
			Num = 0;
		}			
													
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}













