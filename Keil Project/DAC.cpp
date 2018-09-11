// DAC.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize the DAC
// Last Modified: 4/10/2016
// Student names: Diana Shao & Suhas Raja
// Last modification date: 04/24/18

#include <stdint.h>
#include "../tm4c123gh6pm.h"

#include "DAC.hpp"

void DAC_Init(){
	volatile int a = 0;
	SYSCTL_RCGCGPIO_R |=0x20;
	a++;a++;a++;a++;
	GPIO_PORTF_DEN_R |= 0x1F;
	GPIO_PORTF_DIR_R = 0x1F;
	GPIO_PORTF_AMSEL_R &= ~0x1F;      
	GPIO_PORTF_PCTL_R &= ~0x00000FFF;
}

void DAC_Out(int v){GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x1F)+v;}
