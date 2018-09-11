// main.cpp
#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "Controller.hpp"
#include "ADC.hpp"
#include "DAC.hpp"
#include "Sound.hpp"
#include "Kentec.hpp"
#include "frameProcessing.hpp"
#include "UART.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Images.h"
#define PF2        (*((volatile uint32_t *)0x40025010))

uint32_t data[8];

uint8_t flute[64] = {12,14,18,18,20,22,24,26,26,28,
											30,30,30,30,30,28,26,26,24,22,
											20,18,16,14,14,12,12,10,10,10,
											10, 8, 8, 8, 8, 8, 8, 6, 6, 6,
											6, 6, 4, 4, 2, 2, 2, 2, 0, 0,
											0, 0, 0, 0, 2, 2, 2, 4, 4, 6, 
											6, 8, 8, 10};															

uint8_t SinWave[64] = {16, 17, 19, 20, 22, 23, 25, 26, 27, 28, 				// 0-9
												29, 30, 31, 31, 31, 31, 31, 31, 31, 31,				// 10-19
												31, 30, 29, 28, 27, 26, 25, 23, 22, 20,				// 20-29
												19, 17, 16, 14, 13, 11, 10, 8, 7, 6, 			// 30-39
												 4,	 3,  2,  2,  1,  1,  1,  0,  0,  0,       // 40-49
												 1,  1,  1,  2,  2,  3,  4, 6, 7, 8,       // 50-59
												10, 11, 13, 14};															// 60-63

uint32_t Wave_Count = 0;

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

Controller Controllers[4];

extern Controller Controllers[4];
extern uint32_t data[8];
extern int frameNum;
extern int currSound;
volatile uint32_t flag;
uint8_t msg[8];

void butInit(void){
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R |=0xCF;
	GPIO_PORTA_DIR_R &= ~0xC;   	//PC Dir Inputs
	GPIO_PORTC_DIR_R &= ~0xC0;  	//PD Dir Inputs
	GPIO_PORTD_DIR_R &= ~0xCF;  	//PD Dir Inputs
	GPIO_PORTA_DEN_R |= 0xC;    	//PC DEN Inputs
	GPIO_PORTC_DEN_R |= 0xC0;   	//PD DEN Inputs
	GPIO_PORTD_DEN_R |= 0xCC;   	//PD DEN Inputs
	GPIO_PORTA_AFSEL_R &= ~0xC;   //PC ASEL Inputs
	GPIO_PORTC_AFSEL_R &= ~0xC0;  //PD ASEL Inputs
	GPIO_PORTD_AFSEL_R &= ~0xCC;  //PD ASEL Inputs
	GPIO_PORTA_AMSEL_R &= ~0xC;   //PC ASEL Inputs
	GPIO_PORTC_AMSEL_R &= ~0xC0;  //PD ASEL Inputs
	GPIO_PORTD_AMSEL_R &= ~0xCC;  //PD ASEL Inputs
	GPIO_PORTA_PDR_R |= 0xC;   		//PC PDR Inputs
	GPIO_PORTC_PDR_R |= 0xC0;  		//PD PDR Inputs
	GPIO_PORTD_PDR_R |= 0xCC;  		//PD PDR Inputs
}
void Init(){
	PLL_Init(Bus80MHz);										// 80 MHz
	SYSCTL_RCGCGPIO_R |=0x3F;
	SYSCTL_RCGC2_R |=0x3F;
	int x;x++;x++;x++;x++;x++;
  ADC_Init();                   				// ADC channels
	butInit();
	Sound_Init();													// Initialize DAC
  LCD_Init();
	UART_Init();
	ADC_In(data); 												//get initial joystick states
	for(int i = 0;i<4;i++){
		Controllers[i].update(data);
		Controllers[i].initX=Controllers[i].getJX();
		Controllers[i].initY=Controllers[i].getJY();
	}
	Timer1_Init(&frameHandler, 1333333); 	//60 fps
}

int main(void){	 //Controller Testing Main
	Init();
	frameNum = 0;
  while(1){
				// UART ISR
	/*msg[0] = 0x02;
	msg[1] = frameNum%200;
	uint16_t index = 2;
	int i;
	for(i = 0; i < 4; i++){
		if(Controllers[i].active){
			msg[index] = Controllers[i].getID();
			index++;
			msg[index] = Controllers[i].score;
			index++;
			msg[index] = Controllers[i].getLives();
			index++;
			msg[index] = Controllers[i].numBullets;
			index++;
		}
	}
	for(i = 0; msg[i] != 0x03; i++){
		UART_OutChar(msg[i]);
	}
	UART_OutChar(msg[i]);*/
	}
}

void SysTick_Handler(void){
	if(currSound == 0){
		DAC_Out(flute[Wave_Count]);
	}
	else{
		DAC_Out(SinWave[Wave_Count]);
	}
		Wave_Count++;
	if(Wave_Count > 64){      // Reset Array
			Wave_Count = 0;
	}
}
