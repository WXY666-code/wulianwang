#include "BC26.h"
#include "string.h"
///AT+NSOCL=0
#include "BC26.h"
#include "string.h"
#include "data.h"
#include "OLED.h"

char *strx,*extstrx;
char atstr[BUFLEN];
int err;    //ȫ�ֱ���
char atbuf[BUFLEN];
char objtnum[BUFLEN];//�۲��
char distnum[BUFLEN];//�۲��

BC26 BC26_Status;

extern DATA data;

uint16_t temp1;
float temp2;
char sendData[BUFLEN];

//�ڶ��豸
////////////////////////ֻҪ�޸���Ҫ��/////////////////////////////////
#define PRODUCEKEY "aj13oLDgTAG0UeDi"        //�޸Ĳ�Ʒ��Կ
#define DEVICENAME "54245d8abaea"           //�޸��豸����
#define DEVICESECRET "66d1d733fe873156c60d62fbdddfa493" //�޸��豸��Կ
///////////////////////////////////////////////////////////////////////


void Clear_Buffer(void)//��մ���3����
{
 //   printf(buf_uart3.buf);  //���ǰ��ӡ��Ϣ
    delay_ms(300);
    buf_uart3.index=0;
    memset(buf_uart3.buf,0,BUFLEN);
}

int BC26_Init(void)
{
    int errcount = 0;
    err = 0;    //�ж�ģ�鿨�Ƿ��������Ҫ
//    printf("start init BC26\r\n");
		Clear_Buffer();	
    Uart3_SendStr("ATi\r\n");
    delay_ms(3000);
//    printf(buf_uart3.buf);      //��ӡ�յ��Ĵ�����Ϣ
//    printf("get back BC26\r\n");
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//����OK
    
    while(strx==NULL)
    {
  //      printf("\r\n��Ƭ���������ӵ�ģ��...\r\n");
        Clear_Buffer();	
        Uart3_SendStr("ATi\r\n");
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//����OK
    }
    
    Uart3_SendStr("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"ERROR");//ֻҪ�������� �����ͳɹ�
    if(strx==NULL)
    {
 //       printf("�ҵĿ����� : %s \r\n",&buf_uart3.buf[8]);
        Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
//        printf("������ : %s \r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart3_SendStr("AT+CGSN=1\r\n");//�������磬PDP
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//��OK
    Clear_Buffer();	
    if(strx)
    {
        Clear_Buffer();	
//        printf("GET IMEI OK\r\n");
        delay_ms(300);
    }
    Uart3_SendStr("AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//��1 ��������ɹ� ��ȡ��IP��ַ��
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();	
        Uart3_SendStr("AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart3_SendStr("AT+QBAND?\r\n"); //�������ֵ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//����OK
    if(strx)
    {
 //       printf("========BAND========= \r\n %s \r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart3_SendStr("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CSQ");//����CSQ
    if(strx)
    {
 //       printf("�ź�����:%s\r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }
    Uart3_SendStr("AT+CEREG?\r\n");
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 0,1");//����ע��״̬
    extstrx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 1,1");//����ע��״̬
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//��������ֵ��û��
    {
        errcount++;
        Clear_Buffer();
        Uart3_SendStr("AT+CEREG?\r\n");//�ж���Ӫ��
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 0,1");//����ע��״̬
        extstrx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 1,1");//����ע��״̬
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC26_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
    int errcount = 0;
    Uart3_SendStr("AT+CGPADDR=1\r\n");//�����
    delay_ms(300);
    Clear_Buffer();
    Uart3_SendStr("AT+CGSN=1\r\n");//�����
    delay_ms(300);
    Clear_Buffer();
    Uart3_SendStr("AT+CGATT?\r\n");//�����
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//ע����������Ϣ
    Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart3_SendStr("AT+CGATT?\r\n");//�����
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//һ��Ҫ�ն�����
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    Clear_Buffer();
		OLED_ShowString(0,0,"Fatfs Error!");
}

//#define SUBSCRIBE "/a1NqBFdnqec/NBLX/user/get"      //�������
//#define ISSUE "/sys/a1NqBFdnqec/NBLX/thing/event/property/post"
void BC26_RegALIYUNIOT(void)//ƽ̨ע��
{
    u8  BC20_IMEI[20],i;//IMEIֵ
		int errcount = 0;
    Uart3_SendStr("AT+QMTDISC=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
    Clear_Buffer();

    Uart3_SendStr("AT+QMTCLOSE=0\r\n");//ɾ�����
    delay_ms(300);
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//���Ͱ��������ò���
    delay_ms(300);  //�ȴ�300ms����OK
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//��OK
	OLED_ShowString(4,1,"55555");
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//��OK
    }
    Clear_Buffer();
		
OLED_ShowString(0,0,"77777777");
    Uart3_SendStr("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");//��¼������ƽ̨
    delay_ms(300);
		OLED_ShowString(4,1,"22222");
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTOPEN: 0,0");//��+QMTOPEN: 0,0
		OLED_ShowString(4,1,"Fatfs");
    while(strx==NULL)
    {
				errcount++;
				delay_ms(30);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTOPEN: 0,0");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
             GPIO_SetBits(GPIOC,GPIO_Pin_3);		//ģ������
             delay_ms(500);
             GPIO_ResetBits(GPIOC,GPIO_Pin_3);
             delay_ms(300);
             NVIC_SystemReset();	//û�д���TCP SOCKET������ϵͳ�ȵ�����������
        }
    }
    Clear_Buffer();
OLED_ShowString(4,1,"1111111!");
    Uart3_SendStr("AT+CGSN=1\r\n");//��ȡģ���IMEI��
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGSN:");//��+CGSN:
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGSN:");//��+CGSN:
    }
    for(i=0;i<15;i++)
        BC20_IMEI[i]=strx[i+7];
    BC20_IMEI[15]=0;
    Clear_Buffer();

//    printf("�ҵ�ģ��IMEI��:%s\r\n\r\n\r\n",BC20_IMEI);

    memset(atstr,0,BUFLEN);
		OLED_ShowString(4,1,"Fatfs Error3!");
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//�������ӵ�������
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
		OLED_ShowString(4,1,"555555!");
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
    }
    Clear_Buffer();
		OLED_ShowString(4,1,"666666!");
		
		


	  /*
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//�������ӵ�������
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    }
    Clear_Buffer();
		*/
}

//void BC26_ALYIOTSenddata(u8 *len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
//{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{CurrentTemperature:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    }
//    Clear_Buffer();
//}

////void BC26_ALYIOTSenddata1(u8 *len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
////{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{RelativeHumidity:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    }
//    Clear_Buffer();
//}


void BC26_ALYIOTSenddata_Light(u8 *len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Light:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_UVI(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{UVI:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Temp(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Temp:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}


void BC26_ALYIOTSenddata_Humi(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Humi:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Wind_speed(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Wind_speed:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Gust_speed(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Gust_speed:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Wind_direction(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Wind_direction:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Rainfall_M(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Rainfall_M:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Rainfall_F(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Rainfall_F:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
  //  printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Pressure(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Pressure:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_Pressure(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_Pressure:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_Ligth(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_Ligth:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_UVI(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_UVI:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
    }
    Clear_Buffer();
}





//���ݴ�����

void BC26_DATA(void)
{
	
	
	
	sprintf(sendData, "%.1f", data.Ligth);
	
	BC26_ALYIOTSenddata_Light("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	
	sprintf(sendData, "%.1f", data.UVI);
	BC26_ALYIOTSenddata_UVI("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Temp);
    BC26_ALYIOTSenddata_Temp("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);


    sprintf(sendData, "%d", data.Humi);
    BC26_ALYIOTSenddata_Humi("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);


    sprintf(sendData, "%.2f", data.Wind_speed);
	BC26_ALYIOTSenddata_Wind_speed("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	
	sprintf(sendData, "%.2f", data.Gust_speed);
	BC26_ALYIOTSenddata_Gust_speed("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	
	sprintf(sendData, "%d", data.Wind_direction);
	BC26_ALYIOTSenddata_Wind_direction("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Rainfall_M);
    BC26_ALYIOTSenddata_Rainfall_M("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Rainfall_F);
	BC26_ALYIOTSenddata_Rainfall_F("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);

    sprintf(sendData, "%.2f", data.Pressure);
	BC26_ALYIOTSenddata_Pressure("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);

    sprintf(sendData, "%.2f", data.K_Pressure);
	BC26_ALYIOTSenddata_K_Pressure("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);

    sprintf(sendData, "%.1f", data.K_Ligth);
	BC26_ALYIOTSenddata_K_Ligth("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.K_UVI);
	BC26_ALYIOTSenddata_K_UVI("8",(u8 *)sendData);//�����ݵ�ƽ̨��
//    delay_ms(500);

 }
void ITO_GetCmdData_Deal(void)
{
	strx =strstr((char *)buf_uart3.buf,(char *)"params");

////���ƶ˻�ȡ���� ����������
//     if(strx == strstr((char *)buf_uart3.buf + 2 ,(char *)"params")) 
//	  {(star_sp = strstr((char *)MQTT_CMDOutPtr + 2,

//		end_sp = strstr(star_sp + 10,

//		memcpy (identifier _temp, star_sp + 10,
//	  }	
	
	if(strstr((char *)buf_uart3.buf,"\"params\":{\"relay\":1}"))
    {
		
		
//		Relay_0n();

//		Switch_Sta("relay"):
    }
	else if(strstr((char *)buf_uart3.buf,"\"params\":{\"relay\":0}"))
    {
		
//		Relay_Off();

//		Switch_Sta("relay");
	}
    Clear_Buffer();
	
}

