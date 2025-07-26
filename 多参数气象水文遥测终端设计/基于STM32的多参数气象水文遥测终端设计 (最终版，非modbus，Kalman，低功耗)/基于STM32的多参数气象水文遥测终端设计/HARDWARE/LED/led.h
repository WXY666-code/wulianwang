#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

void LED_Init(void);//≥ı ºªØ
void BC26CTR_Init(void);

#define LED0 PAout(8)	// PA8
#define LED1 PBout(15)	// PB15	

#define PWRKEY  PCout(7)
//#define RESET   PAout(8)

#endif
