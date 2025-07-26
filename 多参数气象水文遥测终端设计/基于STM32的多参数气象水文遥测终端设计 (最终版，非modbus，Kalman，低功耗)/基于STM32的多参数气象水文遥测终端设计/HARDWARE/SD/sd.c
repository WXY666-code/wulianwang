
#include "sd.h"
#include "stdio.h"

#include <string.h>
#include "malloc.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"

#include "delay.h"


extern     FRESULT res_sd; 
extern     FIL fnew;					   /* 文件对象 */
extern	   UINT fnum;            		   /* 文件成功读写数量 */
//extern     BYTE ReadBuffer[1024]={0};     /* 读缓冲区 */
//           BYTE WriteBuffer[] =              /* 写缓冲区*/
//          "111128484652niybyibyb\r\n";
//           char WriteBuffer[] =              /* 写缓冲区*/
//          "111128484652niybyibyb\r\n";
		   
extern char str_t[150];

extern char flag;	//表头标志位	   

void sd_initialize(void)
{
	res_sd = f_mount(fs[0],"0:",1);
			
    delay_ms(200);
	
	/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
//		printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
//			delay_ms(50000);
	
		do
		{
		    /* 格式化 */
		   	res_sd=f_mkfs("0:",0,0);
			delay_ms(200);
			
		}while(res_sd != FR_OK);
		
		
		if(res_sd == FR_OK)
		{
//			printf("》SD卡已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_sd = f_mount(fs[0],"0:",1);
		}
		else
		{
//			LED_RED;
//		printf("《《格式化失败。》》res_sd =%d\r\n",res_sd);
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
//    printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
//    printf("！！可能原因：SD卡初始化不成功。\r\n");
		while(1);
  }
  else
  {
//    printf("》文件系统挂载成功，可以进行读写测试\r(%d)\n",res_sd);
  }
}	


void sd_write(void)
{
//	char temp[] ="光照强度\t紫外线指数\t温度\t湿度\t风速\t阵风风速\t风向\t降雨量（Modbus）\t降雨量（翻斗）\r\n";
//	char temp[] ="Ligth\tUVI\tTemp\tHumi\tWind_speed\tGust_speed\tWind_direction\tRainfall_M\tRainfall_F\r\n";
//	char temp[] ="Ligth,UVI,Temp,Humi,Wind_speed,Gust_speed,Wind_direction,Rainfall_M,Rainfall_F\r\n";
	char temp[] ="年月日,时,分,秒,光照强度(Lux),紫外线指数,温度(°C),湿度(%),风速(m/s),阵风风速(m/s),风向(°),降雨量_M(mm),降雨量_翻斗(mm),大气压(hpa),卡尔曼大气压(hpa),卡尔曼光照强度(Lux),卡尔曼紫外线指数\r\n";
	
	/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
//	printf("\r\n****** 即将进行文件写入... ******\r\n");
    f_close(&fnew); 
	
//	do
//	{	

	
//		/*移动文件读写指针到文件结束处，以便添加数据*/
//		res_sd = f_lseek(file,file->fsize);
        
//		if(flag == NO_CREATE)
//		{
//			do 
//			{  
//			res_sd = f_open(&fnew, "0:/DATA.csv",FA_CREATE_ALWAYS | FA_WRITE );
//			delay_ms(2000);
//			}while( res_sd != FR_OK );
//			
//		}
//		else
//			{
			do 
			{  
			res_sd = f_open(&fnew, "0:/DATA.csv",FA_OPEN_ALWAYS | FA_WRITE );
			delay_ms(2000);
			}while( res_sd != FR_OK );
//		}	
//		if(flag == NO_CREATE)
//		{		
//		}	
//				
		if ( res_sd == FR_OK )
		{
//			printf("》打开/创建FatFs读写.csv文件成功，向文件写入数据。\r\n");

			delay_ms(200);
			/* 将指定存储区内容写入到文件内 */

			
		/* 查找文件的结尾 */
			do 
			{   res_sd=f_lseek(&fnew,f_size(&fnew));
				delay_ms(200);
            }while( res_sd != FR_OK );	
			

		
		if(flag == NO_heading)   //打印表头
		{
			do 
			{   res_sd=f_write(&fnew,temp,sizeof(temp),&fnum);
				delay_ms(2000);
            }while( res_sd != FR_OK );
			flag = YES_heading;
			flag = YES_CREATE;
		}
		
		
	    do 
		{ 

			res_sd=f_write(&fnew,str_t,sizeof(str_t),&fnum);
			delay_ms(2000);

		}while( res_sd != FR_OK );	
			
//	}while( res_sd != FR_OK );	
		
    if(res_sd==FR_OK)
    {
//      printf("》文件写入成功，写入字节数据：%d\n",fnum);
//      printf("》向文件写入的数据为：\r\n%s\r\n",str_t);
    }
    else
    {
//      printf("！！文件写入失败：(%d)\n",res_sd);
    }
	
//	 /* 查找文件的结尾 */
//    res_sd=f_lseek(&fnew,f_size(&fnew));
	
//	 do 
//		{ res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//			delay_ms(2000);
//		}while( res_sd != FR_OK );
		
	
		/* 不再读写，关闭文件 */
	
	//清空写入缓存
	memset(str_t, 0, sizeof(str_t));
	
    f_close(&fnew);
	}
	
	else
	{	
//		LED_RED;
//		printf("！！打开/创建文件失败。(%d)\n",res_sd);
	}
	
	
		
}












	
