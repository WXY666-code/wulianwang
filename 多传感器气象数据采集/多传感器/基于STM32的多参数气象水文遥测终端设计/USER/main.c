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
#include "wdg.h"

#include "malloc.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"

#include "sd.h"
#include "usart3.h"
#include "Serial.h"
#include "HCSR04.h"
#include <kalman.h>


/*

//??1????01???
//??2????02???
//??3????03???
//??4??????????(?????????0x02)

//?????????n??????
//Host_Read03_slave(0x02,0x0000,0x0006);//??1????,??2????,??3??????


*/

//????????????????
int slave=0;//?? id

int host_slave_flag=0;//0-???????????,1-??????????uint8_t key_value=0;//???????? 1-4
uint8_t key_flag=0;//key_flag ?? 0 ??????????(???????? 1 ???)-----



int jx ;

//typedef struct {


//	double x;

//double P;	//  ?????

//	double Q;	// ????(?? 0.01-0.1)

//} KalmanFilter; 
typedef struct {
  double x;	//    ????
  double P;	// ???????
	double Q;		// ???????
	double R;	// ???????
	double K;	// ?????
} KalmanFilter;

KalmanFilter kf1; // ???????
 

KalmanFilter kf2;
KalmanFilter kf3;

//  ? main ?????????

//void	kalman_init(KalmanFilter*	kf,	double	init_dist,	double	init_uncertainty,	double process_noise);
//void kalman_update(KalmanFilter* kf, double d1, double d2, double d3); 
double fused_distance(double d1, double d2, double d3);
void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R); 
double KalmanFilter_Update(KalmanFilter *kf, double z);
//???? 3 ?? STP-23L ????,???? 1 ?????????,oled ????????:
//STP-23L ??	?????




//P5V 		5V GND 			GND TX 	B11
//RX 	??

//***************************************/

double fuse_distances(double d1, double d2, double d3); 
void OLED_SHOW(void);
u16 receive_cnt;//???????????
u8 confidence;
u16 distance,noise,reftof;
 

u32 peak,intg; 
u8 dis;
extern LidarPointTypedef Pack_sum;		/* ?????? */ 
volatile uint8_t page_flag = 0;	// ????
volatile uint32_t timer_counter = 0; // ?????

uint8_t current_page = 0;	// ????(0-2)

// ??????(?? TIM4,1 ???)

void PageTimer_Init(void) {

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
// ?????(1 ???)

TIM_TimeBaseStructure.TIM_Period = 10000 - 1;		// ??????
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;	// ???
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// ????

NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 

NVIC_Init(&NVIC_InitStructure); 
TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
TIM_Cmd(TIM4, ENABLE);
}

// TIM4 ??????

void TIM4_IRQHandler(void) {

  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) { 
	    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
			if(++timer_counter >= 5) { // 5 ???
					page_flag = 1;

					current_page = (current_page + 1) % 3; // ??????

					timer_counter = 0;

       }

  }

}

double init_x1 = 0.0;	// ??????

double init_P1 = 1.0;	// ?????????
double Q1 = 0.001;			// ???????
double R1 = 0.1;		// ???????
double init_x2 = 0.0;	//   ??????
double init_P2 = 1.0;	// ?????????
double Q2 = 0.001;	// ???????
 

double R2 = 0.1;		// ???????
double init_x3 = 0.0;	//  ? ? ? ? ? ? 
double init_P3= 1.0;	// ?????????
double Q3 = 0.001;			// ???????
double R3 = 0.1;		// ???????
int main(void)

{

HCSR04_DisTance HCSR04_distance = {0};

NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //?????????,????????
//NVIC_PriorityGroup_2,?????????? 2,???[0, 1, 2, 3]????????????? 2,???[0, 1, 2, 3]?????
delay_init();//?????
uart_init(115200);//?? 1 ???,??????????????

/* ? ? JTAG ? ? */ 
//JTAG_Set(JTAG_SWD_DISABLE);
/*?? SWD ?? ??????? SWD ????*/ 
//JTAG_Set(SWD_ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
OLED_Init();

uart3_init(230400);//??????
HCSR04_Init();
 

HCSR04_Start();

NVIC_Configuration();	//?? NVIC ???? 2:2 ??????,2 ??????

//uart_init(9600);	//?? 1 ???? 9600(?????????)



LED_Init();	//LED ?????

//KEY_Init();	//?????????????Modbus_uart2_init(9600);//??? modbus ?? 2 ? 485 ????
Modbus_TIME3_Init(7200-1,10-1);//???????? 1 ?????,?? 2 ?????

//1ms ????

Modbus_Init();//MODBUS ???--???????????,??????????

PageTimer_Init();	// ????????

//KalmanFilter kf1;KalmanFilter kf2;KalmanFilter kf3;

//	kalman_init(&kf1, 0.0, 1.0, 0.05); kalman_init(&kf2, 0.0, 1.0, 0.05); kalman_init(&kf3, 0.0,1.0, 0.05);

// ??????????

KalmanFilter_Init(&kf1, init_x1, init_P1, Q1, R1);

KalmanFilter_Init(&kf2, init_x2, init_P2, Q2, R2);
KalmanFilter_Init(&kf3, init_x3, init_P3, Q3, R3); 
double fused_result = 0; // ???????
char display_buf[16];	// ?????

double d1 ;	// ???????? 1 ??
double d2 ;	// ???????? 2 ??
double d3 ;	// ???????? 3 ??
 

page_flag =1; 
double ls_result;
double var1, var2, var3; 
while(1)
{

d1=HCSR04_distance.DistanceInt; 
	d2=(distance/10);
d3=jx	;

double z1 = d1;

double x_est1 = KalmanFilter_Update(&kf1, z1); 
	double z2 = d2;
double x_est2 = KalmanFilter_Update(&kf2, z2); 
	double z3 = d3;
double x_est3 = KalmanFilter_Update(&kf3, z3); 
	fused_result = fused_distance(x_est1, x_est2, x_est3);
ls_result = fuse_distances(x_est1, x_est2, x_est3);

//	key_Send();//????

// ????:??????

HCSR04_Start();



delay_ms(50);
 

if (HCSR04_Status & HCSR04_GETDATASUCCESSFUL)

{

HCSR04_GetDistance(&HCSR04_distance); 
	delay_ms(100);
HCSR04_Start();

}

Host_Read03_slave(0x01, 0x0001, 0x0001); 
if (modbus.Host_send_flag)
{

modbus.Host_Sendtime = 0;

modbus.Host_time_flag = 0;

modbus.Host_send_flag = 0;




HOST_ModbusRX(); // ????



}

delay_ms(1000); 
OLED_Clear(); 
switch(current_page)
{

case 0: // ?????
 

OLED_Clear(); 
OLED_ShowString(1, 0, " Distance:"); 
OLED_ShowString(2, 0, " 000.000cm"); 
OLED_ShowString(3, 0, " 000.000cm"); 
OLED_ShowString(4, 0, " 000.000cm");
OLED_ShowNum(2, 0, HCSR04_distance.DistanceInt, 4);

OLED_ShowNum(2, 5, HCSR04_distance.DistanceFloat, 3);




OLED_SHOW();

OLED_ShowNum(4, 1, jx, 4); break;


case 2: // ?????

OLED_Clear();

sprintf(display_buf, "Fused:%.3f", fused_result); 
OLED_ShowString(1, 1, display_buf);
sprintf(display_buf, "LS_IRLS:%.3f", ls_result);	// ????????

OLED_ShowString(2, 1, display_buf);

// ???????

break;




case 1: // ?????
 

OLED_ShowString(1, 1, "Kalman:"); 
sprintf(display_buf, "H1:%.3f", x_est1);
OLED_ShowString(2, 1, display_buf);

sprintf(display_buf, "H2:%.3f", x_est2); 
OLED_ShowString(3, 1, display_buf);
sprintf(display_buf, "H3:%.3f", x_est3); 
OLED_ShowString(4, 1, display_buf);


break;

    }
  }
}
/*	oled ??	*/ 
void OLED_SHOW(void)
{

OLED_ShowNum(3,1,distance/10,3); 
	OLED_ShowNum(3,5,distance*100,3);
}

double fused_distance(double d1, double d2, double d3) { 
	double diff1 = fabs(d1-d2) + fabs(d1-d3);
double diff2 = fabs(d2-d1) + fabs(d2-d3); 
	double diff3 = fabs(d3-d1) + fabs(d3-d2);
if(diff1+diff2+diff3 < 1e-9) return (d1+d2+d3)/3;
 

double w1 = 1/(diff1 + 1e-6); 
	double w2 = 1/(diff2 + 1e-6); 
	double w3 = 1/(diff3 + 1e-6); 
	double total = w1 + w2 + w3;
return (d1*w1 + d2*w2 + d3*w3)/total;

}

void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R) { 
	kf->x = init_x;
kf->P = init_P; 
	kf->Q = Q;
kf->R = R; 
	kf->K = 0.0;
}

double KalmanFilter_Update(KalmanFilter *kf, double z) {

// ????

double x_prior = kf->x;

double P_prior = kf->P + kf->Q;




// ????

kf->K = P_prior / (P_prior + kf->R);
kf->x = x_prior + kf->K * (z - x_prior); 
	kf->P = (1 - kf->K) * P_prior;
 




return kf->x;

}

double fuse_distances(double d1, double d2, double d3) {

double estimate = (d1 + d2 + d3) / 3.0;	// ????(????)

double prev_estimate;

int max_iter = 20;	// ??????

double epsilon = 1e-8;	// ????

for (int i = 0; i < max_iter; i++) { 
	prev_estimate = estimate;
// ??????(??????)

double r1 = d1 - estimate; 
	double r2 = d2 - estimate; 
	double r3 = d3 - estimate;
// ????(????)

double w1 = fabs(r1) > 1e-6 ? 1.0/(r1*r1 + 1e-6) : 1e6; 
	double w2 = fabs(r2) > 1e-6 ? 1.0/(r2*r2 + 1e-6) : 1e6; 
	double w3 = fabs(r3) > 1e-6 ? 1.0/(r3*r3 + 1e-6) : 1e6;
// ?????

estimate = (d1*w1 + d2*w2 + d3*w3) / (w1 + w2 + w3);




// ????
 

if (fabs(estimate - prev_estimate) < epsilon) { 
	break;
}

}

return estimate;

}


