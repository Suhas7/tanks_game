// ADC.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Student names: Diana Shao & Suhas Raja
// Last modification date: 04/24/18

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "ADC.hpp"

// ADC initialization function 
// Input: none
// Output: none
// measures from PE0-3, analog channel 3, 2, 1

void ADC_Init(){
	volatile int delay;
//	GPIO_PORTE_PCTL_R&=0xFF000000;
	GPIO_PORTD_PCTL_R&=0xFF000000;
	GPIO_PORTD_DIR_R &= ~0xCF;  //PD Dir Inputs
	GPIO_PORTE_DIR_R &= ~0x3F;  //PE Dir Inputs

	GPIO_PORTD_AFSEL_R = (GPIO_PORTD_AFSEL_R|0x03);  //AF
	GPIO_PORTE_AFSEL_R |= 0xCF;  //AF

	GPIO_PORTD_DEN_R = (GPIO_PORTD_DEN_R&(~0x03))|(0xCC);  //DEN
	GPIO_PORTE_DEN_R &= ~0xCF;  //DEN
	
	GPIO_PORTD_AMSEL_R |= 0x03;  //ANALOG ENABLE
	GPIO_PORTE_AMSEL_R |= 0xCF;  //ANALOG ENABLE

	SYSCTL_RCGCADC_R |= 0x01;  			// enable adc clock
	delay++;delay++;delay++;delay++;
	ADC0_PC_R &= ~0xF;              // clear max sample rate field
	ADC0_PC_R |= 0x1;               // configure for 125K samples/sec
	ADC0_SSPRI_R = 0x3210;          // Sequencer 3 is lowest priority
	ADC0_ACTSS_R &= ~0x0001;        // disable sample sequencer 0
	ADC0_EMUX_R &= ~0x0F;         	// seq0 is software trigger
	ADC0_SSMUX0_R =0x01892367;			// channel Ain
	ADC0_SSCTL0_R = 0x60000000;         		// no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
	ADC0_IM_R &= ~0x01;           	// disable SS0 interrupts
	ADC0_ACTSS_R |= 0x01;         	// enable sample sequencer 0
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC_In(uint32_t data[8]){
	ADC0_PSSI_R = 0x01;            	 // 1) initiate SS0
	while((ADC0_RIS_R&0x01)==0){};   // 2) wait for conversion done
	data[0] = ADC0_SSFIFO0_R&0xFFF;  // 3A) read result
  data[1] = ADC0_SSFIFO0_R&0xFFF;  // 3B) read result
  data[2] = ADC0_SSFIFO0_R&0xFFF;  // 3C) read result
  data[3] = ADC0_SSFIFO0_R&0xFFF;  // 3D) read result
	data[4] = ADC0_SSFIFO0_R&0xFFF;	 // 3E) read result
  data[5] = ADC0_SSFIFO0_R&0xFFF;  // 3F) read result
  data[6] = ADC0_SSFIFO0_R&0xFFF;  // 3G) read result
  data[7] = ADC0_SSFIFO0_R&0xFFF;  // 3H) read result
	ADC0_ISC_R = 0x01;             	 // 4) acknowledge completion		
}
