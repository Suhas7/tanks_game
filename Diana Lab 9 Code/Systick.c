

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADC.h"

uint32_t ADCStatus = 0x10;
uint32_t ADCMail;

void Systick_Init(void){
	NVIC_ST_CTRL_R &= ~(0x01);
	NVIC_ST_RELOAD_R = 1333333; 			// 60 Hertz
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x07;
	ADCStatus = 0x10;
}

