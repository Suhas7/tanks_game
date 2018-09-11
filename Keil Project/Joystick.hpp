// Joystick.hpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 and use a slide pot to measure distance
// Created: 3/28/2018 
// Student names: Suhas Raja & Diana Shao
// Last modification date: 04/28/18

#ifndef Joystick_HPP
#define Joystick_HPP
#include <stdint.h>

//int** portSettings = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
class Joystick{ private:
        int32_t dataX;     // raw ADC value
        int32_t flagX;      // 0 if data is not valid, 1 if valid
        int32_t distanceX; // distance in 0.001cm
				int32_t dataY;     // raw ADC value
        int32_t flagY;      // 0 if data is not valid, 1 if valid
        int32_t distanceY; // distance in 0.001cm
// distance = (slope*data+offset)/4096
        uint32_t slope;    // calibration coeffients
        uint32_t offset;
    public:
				Joystick();
        Joystick(uint32_t m, uint32_t b); // initialize slide pot
        void Save(uint32_t nX, uint32_t nY, int initX, int initY);        // save ADC, set semaphore
        void Sync(void);              // wait for semaphore
        int32_t Convert(uint32_t n); // convert ADC to raw sample
        int32_t ADCsampleX(void);     // return last ADC sample value (0 to 4095)
        int32_t DistanceX(void);      // return last distance value (0 to 2000), 0.001cm
				int32_t ADCsampleY(void);     // return last ADC sample value (0 to 4095)
        int32_t DistanceY(void);      // return last distance value (0 to 2000), 0.001cm
				void setX(uint32_t x);
				void setY(uint32_t y);
    };

#endif
