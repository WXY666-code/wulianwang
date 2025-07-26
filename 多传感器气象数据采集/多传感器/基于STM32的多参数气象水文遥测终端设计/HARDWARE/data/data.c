
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "data.h"
#include "MyRTC.h"
#include "OLED.h"

extern DATA data;
extern char str_t[150];

extern char oled_flag;


char strt[10];


void data_str (void)
{
//	 char str_t[150];
	 char str2[10];
	int len ;
	int i;
    for(i=0;i<19;i++)
	{
		if((i!=0)&&(i!=1)&&(i!=2)){  len = strlen(str_t);    
		str_t[len] = ',';       } 

		switch(i)
		{
			case 0 : 
			sprintf(str_t, "%d", MyRTC_Time[0]);      break;
			
			case 1 : 
			sprintf(str2, "%2d", MyRTC_Time[1]);     strcat(str_t, str2);      break;
			
			case 2 : 
			sprintf(str2, "%2d", MyRTC_Time[2]);     strcat(str_t, str2);      break;
			
			case 3 : 
			sprintf(str2, "%2d", MyRTC_Time[3]);     strcat(str_t, str2);      break;
			
			case 4 : 
			sprintf(str2, "%2d", MyRTC_Time[4]);     strcat(str_t, str2);      break;
			
			case 5 : 
			sprintf(str2, "%2d", MyRTC_Time[5]);     strcat(str_t, str2);      break;
			
			case 6 : 
		    sprintf(str2, "%.1f", data.Ligth);             strcat(str_t, str2);      break;
			
	        case 7 : 
			sprintf(str2, "%.1f", data.UVI);              strcat(str_t, str2);      break;
	
	        case 8 : 
			sprintf(str2, "%.1f", data.Temp);             strcat(str_t, str2);      break;
			
	        case 9 : 
			sprintf(str2, "%d", data.Humi);               strcat(str_t, str2);      break;
			
	        case 10 : 
			sprintf(str2, "%.2f", data.Wind_speed);         strcat(str_t, str2);      break;
			
            case 11 : 
			sprintf(str2, "%.2f", data.Gust_speed);         strcat(str_t, str2);      break;
			
	        case 12 : 
			sprintf(str2, "%d", data.Wind_direction);     strcat(str_t, str2);      break;
			
	        case 13 : 
			sprintf(str2, "%.1f", data.Rainfall_M);       strcat(str_t, str2);      break;
			
	        case 14 : 
			sprintf(str2, "%.1f", data.Rainfall_F);       strcat(str_t, str2);      break;
			
		    case 15 : 
			sprintf(str2, "%.2f", data.Pressure);       strcat(str_t, str2);      break;
			
			case 16 : 
			sprintf(str2, "%.2f", data.K_Pressure);       strcat(str_t, str2);      break;
			
			case 17 : 
			sprintf(str2, "%.1f", data.K_Ligth);             strcat(str_t, str2);      break;
			
			case 18 : 
			sprintf(str2, "%.1f", data.K_UVI);              strcat(str_t, str2);      break;
			
		}
		
    }	
	len = strlen(str_t);    
	str_t[len] = '\r';
	str_t[len+1] = '\n';	
}




void data_oled (void)
{
//	int i;
	switch(oled_flag)
		{
			case 1:
		        OLED_ShowString(1, 1,"Ligth:");
			
			    sprintf(strt, "%.1f", data.Ligth);
				OLED_ShowString(1, 7, strt);
			    
			    OLED_ShowString(2, 1, "UVI:");
			
			    sprintf(strt, "%.1f", data.UVI);
				OLED_ShowString(2, 7, strt);
			
//				OLED_SetCursor(1, 88);
//				for(i = 88; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}

//				OLED_SetCursor(2, strlen(strt)+6*8);
//				for(i = strlen(strt)+6*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}
			break;
			
			case 2:
	
			    OLED_ShowString(1, 1, "Temp:");
			    sprintf(strt, "%.1f", data.Temp);
                OLED_ShowString(1, 7, strt);
			    
			    OLED_ShowString(2, 1, "Humi:");
			    OLED_ShowNum(2,7,data.Humi,2);
			

//			    OLED_SetCursor(1, strlen(strt)+6*8);
//				for(i = strlen(strt)+6*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}

//				OLED_SetCursor(2, 88);
//				for(i = 88; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}
			
			break;
			
			
			case 3:
				
			    OLED_ShowString(1, 1, "Wind_s:");
			
			    sprintf(strt, "%.2f", data.Wind_speed);
                OLED_ShowString(1, 10, strt);
			
//			    OLED_ShowNum(1,13,data.Wind_speed,2);
			
			    OLED_ShowString(2, 1, "Gust_s:");
			
			    sprintf(strt, "%.2f", data.Gust_speed);
                OLED_ShowString(2, 10, strt);
			
//			    OLED_ShowNum(2,13,data.Gust_speed,2);
			

//			    OLED_SetCursor(1, 14*8);
//				for(i = 14*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}

//				OLED_SetCursor(2, 14*8);
//				for(i = 14*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}
			
			
			
			break;
			
			case 4:
				
			    OLED_ShowString(1, 1, "Wind_DIT:");
			    OLED_ShowNum(1,11,data.Wind_direction,3);
			
			    OLED_ShowString(2, 1, "Rain_M:");
			    sprintf(strt, "%.1f", data.Rainfall_M);
			
			    OLED_ShowString(2,9,strt);
			

//			    OLED_SetCursor(1, 12*8);
//				for(i = 12*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}

//				OLED_SetCursor(2, strlen(strt)+12*8);
//				for(i = strlen(strt)+12*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}
			
			
			
			
			break;
			
			case 5:
				         
			    OLED_ShowString(1, 1, "Rain_F:");
			    sprintf(strt, "%.1f", data.Rainfall_F);
			    OLED_ShowString(1,9,strt);
			
                OLED_ShowString(2, 1, "Pres:");
			    sprintf(strt, "%.2f", data.Pressure);
			
			    OLED_ShowString(2,6,strt);
			
//			    OLED_SetCursor(1, strlen(strt)+12*8);
//				for(i = strlen(strt)+12*8; i < 128; i++)
//				{
//					OLED_WriteData(0x00);
//				}
			
			break;
			
			case 6:
				         
			    OLED_ShowString(1, 1, "K_Pres:");
			    sprintf(strt, "%.2f", data.K_Pressure);
			    OLED_ShowString(1,9,strt);
			
                OLED_ShowString(2, 1, "K_Ligth:");
			    sprintf(strt, "%.1f", data.K_Ligth);
			
			    OLED_ShowString(2,9,strt);
				
			
			break;
			
			case 7:
				         
			    OLED_ShowString(1, 1, "K_UVI:");
			    sprintf(strt, "%.1f", data.K_UVI);
			    OLED_ShowString(1,9,strt);
			
//                OLED_ShowString(2, 1, "Pres:");
//			    sprintf(strt, "%.2f", data.Pressure);
//			
//			    OLED_ShowString(2,6,strt);
				
			
			break;
			
			default: break;
			
		}






}	
	

