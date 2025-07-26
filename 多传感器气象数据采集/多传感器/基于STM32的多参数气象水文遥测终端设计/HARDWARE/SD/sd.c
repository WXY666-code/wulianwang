
#include "sd.h"
#include "stdio.h"

#include <string.h>
#include "malloc.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"

#include "delay.h"


extern     FRESULT res_sd; 
extern     FIL fnew;					   /* �ļ����� */
extern	   UINT fnum;            		   /* �ļ��ɹ���д���� */
//extern     BYTE ReadBuffer[1024]={0};     /* �������� */
//           BYTE WriteBuffer[] =              /* д������*/
//          "111128484652niybyibyb\r\n";
//           char WriteBuffer[] =              /* д������*/
//          "111128484652niybyibyb\r\n";
		   
extern char str_t[150];

extern char flag;	//��ͷ��־λ	   

void sd_initialize(void)
{
	res_sd = f_mount(fs[0],"0:",1);
			
    delay_ms(200);
	
	/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_sd == FR_NO_FILESYSTEM)
	{
//		printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
//			delay_ms(50000);
	
		do
		{
		    /* ��ʽ�� */
		   	res_sd=f_mkfs("0:",0,0);
			delay_ms(200);
			
		}while(res_sd != FR_OK);
		
		
		if(res_sd == FR_OK)
		{
//			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_sd = f_mount(NULL,"0:",1);			
      /* ���¹���	*/			
			res_sd = f_mount(fs[0],"0:",1);
		}
		else
		{
//			LED_RED;
//		printf("������ʽ��ʧ�ܡ�����res_sd =%d\r\n",res_sd);
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
//    printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
//    printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
//    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r(%d)\n",res_sd);
  }
}	


void sd_write(void)
{
//	char temp[] ="����ǿ��\t������ָ��\t�¶�\tʪ��\t����\t������\t����\t��������Modbus��\t��������������\r\n";
//	char temp[] ="Ligth\tUVI\tTemp\tHumi\tWind_speed\tGust_speed\tWind_direction\tRainfall_M\tRainfall_F\r\n";
//	char temp[] ="Ligth,UVI,Temp,Humi,Wind_speed,Gust_speed,Wind_direction,Rainfall_M,Rainfall_F\r\n";
	char temp[] ="������,ʱ,��,��,����ǿ��(Lux),������ָ��,�¶�(��C),ʪ��(%),����(m/s),������(m/s),����(��),������_M(mm),������_����(mm),����ѹ(hpa),����������ѹ(hpa),����������ǿ��(Lux),������������ָ��\r\n";
	
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
//	printf("\r\n****** ���������ļ�д��... ******\r\n");
    f_close(&fnew); 
	
//	do
//	{	

	
//		/*�ƶ��ļ���дָ�뵽�ļ����������Ա��������*/
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
//			printf("����/����FatFs��д.csv�ļ��ɹ������ļ�д�����ݡ�\r\n");

			delay_ms(200);
			/* ��ָ���洢������д�뵽�ļ��� */

			
		/* �����ļ��Ľ�β */
			do 
			{   res_sd=f_lseek(&fnew,f_size(&fnew));
				delay_ms(200);
            }while( res_sd != FR_OK );	
			

		
		if(flag == NO_heading)   //��ӡ��ͷ
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
//      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
//      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",str_t);
    }
    else
    {
//      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
    }
	
//	 /* �����ļ��Ľ�β */
//    res_sd=f_lseek(&fnew,f_size(&fnew));
	
//	 do 
//		{ res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//			delay_ms(2000);
//		}while( res_sd != FR_OK );
		
	
		/* ���ٶ�д���ر��ļ� */
	
	//���д�뻺��
	memset(str_t, 0, sizeof(str_t));
	
    f_close(&fnew);
	}
	
	else
	{	
//		LED_RED;
//		printf("������/�����ļ�ʧ�ܡ�(%d)\n",res_sd);
	}
	
	
		
}












	
