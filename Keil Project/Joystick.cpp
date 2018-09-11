// Joystick.cpp
// Runs on LM4F120/TM4C123
// Provide functions that use two slide pots to measure distance
// Created: 3/28/2018 
// Student names: Diana Shao & Suhas Raja
// Last modification date: 04/24/18

#include <stdint.h>
#include "Joystick.hpp"
#include "../tm4c123gh6pm.h"

Joystick::Joystick(){
//*** students write this ******
// initialize all private variables
	this->dataX = 0; 
	this->distanceX = 0;
	this->flagX = 0;
	this->dataY = 0;
	this->distanceY = 0;
	this->flagY = 0;
	this->slope = 1500;
	this->offset = 0;	
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
Joystick::Joystick(uint32_t m, uint32_t b){
//*** students write this ******
// initialize all private variables
	this->dataX = 0;
	this->distanceX = 0;
	this->flagX = 0;
	this->dataY = 0;
	this->distanceY = 0;
	this->flagY = 0;
// make slope equal to m and offset equal to b
	this->slope = m;
	this->offset = b;	
}

void Joystick::Save(uint32_t nX, uint32_t nY, int initX, int initY){
// 1) save ADC sample into private variable
	this->dataX = nX;
	this->dataY = nY;
// 2) calculate distance from ADC, save into private variable
	// distance = (slope*data+offset)/4096
	this->distanceX = nX-2032; //-initX;
	this->distanceY = nY-2032;//-initY;
// 3) set semaphore flag = 1	
	this->flagX = 1;
	this->flagY = 1;
	
}

int32_t Joystick::Convert(uint32_t n){
  // use calibration data to convert ADC sample to distance
	return (this->slope*n + this->offset)/4096;
}

void Joystick::Sync(void){
// 1) wait for semaphore flag to be nonzero
	while(this->flagX == 0) {};
// 2) set semaphore flag to 0
	this->flagX = 0;
}

int32_t Joystick::ADCsampleX(void){ // return ADC sample value (0 to 4095)
  //*** students write this ******
  // return last calculated ADC sample
  return this->dataX;
}

int32_t Joystick::ADCsampleY(void){ // return ADC sample value (0 to 4095)
  //*** students write this ******
  // return last calculated ADC sample
  return this->dataY;
}

int32_t Joystick::DistanceX(void){  // return distance value (0 to 2000), 0.001cm
  //*** students write this ******
  // return last calculated distance in 0.001cm
  return this->distanceX;
}

int32_t Joystick::DistanceY(void){  // return distance value (0 to 2000), 0.001cm
  //*** students write this ******
  // return last calculated distance in 0.001cm
  return this->distanceY;
}

