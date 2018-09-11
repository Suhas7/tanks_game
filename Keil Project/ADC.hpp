// ADC.hpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0

// Student names: Diana Shao
// Last modification date: 04/22/18

#include <stdint.h>
#ifndef INCLUDED_ADC_HPP
#define INCLUDED_ADC_HPP

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init();

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC_In(uint32_t data[2]);

#endif
