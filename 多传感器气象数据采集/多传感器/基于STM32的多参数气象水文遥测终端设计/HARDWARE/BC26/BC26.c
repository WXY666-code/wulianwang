#include "BC26.h"
#include "string.h"
///AT+NSOCL=0
#include "BC26.h"
#include "string.h"
#include "data.h"
#include "OLED.h"

char *strx,*extstrx;
char atstr[BUFLEN];
int err;    //全局变量
char atbuf[BUFLEN];
char objtnum[BUFLEN];//观察号
char distnum[BUFLEN];//观察号

BC26 BC26_Status;

extern DATA data;

uint16_t temp1;
float temp2;
char sendData[BUFLEN];

//第二设备
////////////////////////只要修改三要素/////////////////////////////////
#define PRODUCEKEY "aj13oLDgTAG0UeDi"        //修改产品秘钥
#define DEVICENAME "54245d8abaea"           //修改设备名称
#define DEVICESECRET "66d1d733fe873156c60d62fbdddfa493" //修改设备秘钥
///////////////////////////////////////////////////////////////////////


void Clear_Buffer(void)//清空串口3缓存
{
 //   printf(buf_uart3.buf);  //清空前打印信息
    delay_ms(300);
    buf_uart3.index=0;
    memset(buf_uart3.buf,0,BUFLEN);
}

int BC26_Init(void)
{
    int errcount = 0;
    err = 0;    //判断模块卡是否就绪最重要
//    printf("start init BC26\r\n");
		Clear_Buffer();	
    Uart3_SendStr("ATi\r\n");
    delay_ms(3000);
//    printf(buf_uart3.buf);      //打印收到的串口信息
//    printf("get back BC26\r\n");
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返回OK
    
    while(strx==NULL)
    {
  //      printf("\r\n单片机正在连接到模块...\r\n");
        Clear_Buffer();	
        Uart3_SendStr("ATi\r\n");
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返回OK
    }
    
    Uart3_SendStr("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"ERROR");//只要卡不错误 基本就成功
    if(strx==NULL)
    {
 //       printf("我的卡号是 : %s \r\n",&buf_uart3.buf[8]);
        Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
//        printf("卡错误 : %s \r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart3_SendStr("AT+CGSN=1\r\n");//激活网络，PDP
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返OK
    Clear_Buffer();	
    if(strx)
    {
        Clear_Buffer();	
//        printf("GET IMEI OK\r\n");
        delay_ms(300);
    }
    Uart3_SendStr("AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//返1 表明激活成功 获取到IP地址了
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();	
        Uart3_SendStr("AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//返回1,表明注网成功
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart3_SendStr("AT+QBAND?\r\n"); //允许错误值
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返回OK
    if(strx)
    {
 //       printf("========BAND========= \r\n %s \r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart3_SendStr("AT+CSQ\r\n");//查看获取CSQ值
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CSQ");//返回CSQ
    if(strx)
    {
 //       printf("信号质量:%s\r\n",buf_uart3.buf);
        Clear_Buffer();
        delay_ms(300);
    }
    Uart3_SendStr("AT+CEREG?\r\n");
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 0,1");//返回注册状态
    extstrx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 1,1");//返回注册状态
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//两个返回值都没有
    {
        errcount++;
        Clear_Buffer();
        Uart3_SendStr("AT+CEREG?\r\n");//判断运营商
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 0,1");//返回注册状态
        extstrx=strstr((const char*)buf_uart3.buf,(const char*)"+CEREG: 1,1");//返回注册状态
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC26_PDPACT(void)//激活场景，为连接服务器做准备
{
    int errcount = 0;
    Uart3_SendStr("AT+CGPADDR=1\r\n");//激活场景
    delay_ms(300);
    Clear_Buffer();
    Uart3_SendStr("AT+CGSN=1\r\n");//激活场景
    delay_ms(300);
    Clear_Buffer();
    Uart3_SendStr("AT+CGATT?\r\n");//激活场景
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//注册上网络信息
    Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart3_SendStr("AT+CGATT?\r\n");//激活场景
        delay_ms(300);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGATT: 1");//一定要终端入网
        if(errcount>100)     //防止死循环
        {
            errcount = 0;
            break;
        }
    }
    Clear_Buffer();
		OLED_ShowString(0,0,"Fatfs Error!");
}

//#define SUBSCRIBE "/a1NqBFdnqec/NBLX/user/get"      //订阅入口
//#define ISSUE "/sys/a1NqBFdnqec/NBLX/thing/event/property/post"
void BC26_RegALIYUNIOT(void)//平台注册
{
    u8  BC20_IMEI[20],i;//IMEI值
		int errcount = 0;
    Uart3_SendStr("AT+QMTDISC=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
    Clear_Buffer();

    Uart3_SendStr("AT+QMTCLOSE=0\r\n");//删除句柄
    delay_ms(300);
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//发送阿里云配置参数
    delay_ms(300);  //等待300ms反馈OK
    strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返OK
	OLED_ShowString(4,1,"55555");
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"OK");//返OK
    }
    Clear_Buffer();
		
OLED_ShowString(0,0,"77777777");
    Uart3_SendStr("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");//登录阿里云平台
    delay_ms(300);
		OLED_ShowString(4,1,"22222");
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTOPEN: 0,0");//返+QMTOPEN: 0,0
		OLED_ShowString(4,1,"Fatfs");
    while(strx==NULL)
    {
				errcount++;
				delay_ms(30);
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTOPEN: 0,0");//返回OK
        if(errcount>100)     //防止死循环
        {
             GPIO_SetBits(GPIOC,GPIO_Pin_3);		//模块重启
             delay_ms(500);
             GPIO_ResetBits(GPIOC,GPIO_Pin_3);
             delay_ms(300);
             NVIC_SystemReset();	//没有创建TCP SOCKET就重启系统等到服务器就绪
        }
    }
    Clear_Buffer();
OLED_ShowString(4,1,"1111111!");
    Uart3_SendStr("AT+CGSN=1\r\n");//获取模块的IMEI号
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGSN:");//返+CGSN:
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+CGSN:");//返+CGSN:
    }
    for(i=0;i<15;i++)
        BC20_IMEI[i]=strx[i+7];
    BC20_IMEI[15]=0;
    Clear_Buffer();

//    printf("我的模块IMEI是:%s\r\n\r\n\r\n",BC20_IMEI);

    memset(atstr,0,BUFLEN);
		OLED_ShowString(4,1,"Fatfs Error3!");
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//发送链接到阿里云
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
		OLED_ShowString(4,1,"555555!");
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
    }
    Clear_Buffer();
		OLED_ShowString(4,1,"666666!");
		
		


	  /*
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//发送链接到阿里云
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    }
    Clear_Buffer();
		*/
}

//void BC26_ALYIOTSenddata(u8 *len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
//{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{CurrentTemperature:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    }
//    Clear_Buffer();
//}

////void BC26_ALYIOTSenddata1(u8 *len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
////{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{RelativeHumidity:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    }
//    Clear_Buffer();
//}


void BC26_ALYIOTSenddata_Light(u8 *len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Light:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_UVI(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{UVI:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Temp(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Temp:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}


void BC26_ALYIOTSenddata_Humi(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Humi:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Wind_speed(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Wind_speed:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Gust_speed(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Gust_speed:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Wind_direction(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Wind_direction:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Rainfall_M(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Rainfall_M:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Rainfall_F(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Rainfall_F:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
  //  printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_Pressure(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{Pressure:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_Pressure(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_Pressure:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_Ligth(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_Ligth:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC26_ALYIOTSenddata_K_UVI(u8 *len,u8 *data)
{
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"/sys/%s/%s/thing/event/property/post\",\"{params:{K_UVI:%s}}\"\r\n",PRODUCEKEY,DEVICENAME,data);
//    printf("atstr = %s \r\n",atstr);
    Uart3_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
    delay_ms(300);
    strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart3.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
    }
    Clear_Buffer();
}





//数据处理函数

void BC26_DATA(void)
{
	
	
	
	sprintf(sendData, "%.1f", data.Ligth);
	
	BC26_ALYIOTSenddata_Light("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	
	sprintf(sendData, "%.1f", data.UVI);
	BC26_ALYIOTSenddata_UVI("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Temp);
    BC26_ALYIOTSenddata_Temp("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);


    sprintf(sendData, "%d", data.Humi);
    BC26_ALYIOTSenddata_Humi("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);


    sprintf(sendData, "%.2f", data.Wind_speed);
	BC26_ALYIOTSenddata_Wind_speed("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	
	sprintf(sendData, "%.2f", data.Gust_speed);
	BC26_ALYIOTSenddata_Gust_speed("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	
	sprintf(sendData, "%d", data.Wind_direction);
	BC26_ALYIOTSenddata_Wind_direction("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Rainfall_M);
    BC26_ALYIOTSenddata_Rainfall_M("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.Rainfall_F);
	BC26_ALYIOTSenddata_Rainfall_F("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);

    sprintf(sendData, "%.2f", data.Pressure);
	BC26_ALYIOTSenddata_Pressure("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);

    sprintf(sendData, "%.2f", data.K_Pressure);
	BC26_ALYIOTSenddata_K_Pressure("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);

    sprintf(sendData, "%.1f", data.K_Ligth);
	BC26_ALYIOTSenddata_K_Ligth("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);
	
	sprintf(sendData, "%.1f", data.K_UVI);
	BC26_ALYIOTSenddata_K_UVI("8",(u8 *)sendData);//发数据到平台端
//    delay_ms(500);

 }
void ITO_GetCmdData_Deal(void)
{
	strx =strstr((char *)buf_uart3.buf,(char *)"params");

////从云端获取数据 并更改数据
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

