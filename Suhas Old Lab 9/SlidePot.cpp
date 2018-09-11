// SlidePot.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 and use a slide pot to measure distance
// Created: 3/28/2018 
// Student names: Sanika Phanse & Suhas Raja
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "SlidePot.h"
#include "../tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){
	volatile int x = 0;
	
	SYSCTL_RCGCGPIO_R |= 0x08;	//activate clock for port D
	x++;
	x++;
	x++;
	x++;
	GPIO_PORTD_DIR_R &= ~0x04;  //pd2 -> input
	GPIO_PORTD_AFSEL_R |= 0x04;  //enable AF
	GPIO_PORTD_DEN_R &= ~0x04;  //disable digital function
	GPIO_PORTD_AMSEL_R |= 0x04;  //enable analog function
	SYSCTL_RCGCADC_R |= 0x01;  //enable adc clock
	x++;
	x++;
	x++;
	x++;
	ADC0_PC_R |=0x01;	//max sampling rate = 125 kHz
	ADC0_SSPRI_R = 0x0123;	//highest priority -> seq 3
	ADC0_ACTSS_R &= ~0x0008;	//disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;	//seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;	//clear SS3 field
  ADC0_SSMUX3_R += 5;	//channel Ain5 (PD2)
  ADC0_SSCTL3_R = 0x0006;	//no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0008;   //disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008; //enable sample sequencer 3
		
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){
  int conversion;
	
	//ADC_Init();
	ADC0_PSSI_R = 0x08;	//SS3 init
	while((ADC0_RIS_R & 0x08) == 0) {};	//wait for status flag to be set
	conversion = ADC0_SSFIFO3_R & 0xFFF;	//read data
	ADC0_ISC_R = 0x0008;	//clear completion flag
	return conversion;		
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
//*** students write this ******
// initialize all private variables
	this->data = 0;
	this->distance = 0;
	this->flag = 0;
// make slope equal to m and offset equal to b
	this->slope = m;
	this->offset = b;	
}

void SlidePot::Save(uint32_t n){
//*** students write this ******
// 1) save ADC sample into private variable
	this->data = n;
	
// 2) calculate distance from ADC, save into private variable
	// distance = (slope*data+offset)/4096
	this->distance = Convert(n);
	
// 3) set semaphore flag = 1	
	this->flag = 1;
	
}
uint32_t SlidePot::Convert(uint32_t n){
  //*** students write this ******
  // use calibration data to convert ADC sample to distance

	return (this->slope*n + this->offset)/4096;
}

void SlidePot::Sync(void){
// 1) wait for semaphore flag to be nonzero
	while(this->flag == 0) {};
// 2) set semaphore flag to 0
	this->flag = 0;
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
  //*** students write this ******
  // return last calculated ADC sample
  return this->data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
  //*** students write this ******
  // return last calculated distance in 0.001cm
  return this->distance;
}


