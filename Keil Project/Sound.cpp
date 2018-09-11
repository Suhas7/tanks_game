// Sound.cpp
// Runs on LM4F120/TM4C123
// Provide functions that play different sounds using the DAC
// Last Modified: 4/10/2016
// Student names: Diana Shao & Suhas Raja
// Last modification date:

#include <stdint.h>
#include "Sound.hpp"
#include "DAC.hpp"
#include "Timer0.h"
#include "Systick.h"
#include "../tm4c123gh6pm.h"

//#include "assetsAudio.hpp"
//#include "audio.hpp" 

#define C1   597.227567651786   // 2093 Hz
#define B1   632.740566861036   // 1975.5 Hz
#define BF1   670.365278893248   // 1864.7 Hz
#define A1   710.227272727272   // 1760 Hz
#define AF1   752.459584061999   // 1661.2 Hz
#define G1   797.203159310634   // 1568 Hz
#define GF1   844.60732599625   // 1480 Hz
#define F1   894.830291118517   // 1396.9 Hz
#define E1   948.039669154854   // 1318.5 Hz
#define EF1   1004.41304145816   // 1244.5 Hz
#define D1   1064.1385489181   // 1174.7 Hz
#define DF1   1127.41551986378   // 1108.7 Hz
#define C   1194.45513530357   // 1046.5 Hz
#define B   1265.48113372207   // 987.8 Hz
#define BF   1340.7305577865   // 932.3 Hz
#define A   1420.45454545455   // 880 Hz
#define AF   1504.919168124   // 830.6 Hz
#define G   1594.40631862127   // 784 Hz
#define GF   1689.2146519925   // 740 Hz
#define F   1789.66058223704   // 698.5 Hz
#define E   1896.07933830971   // 659.3 Hz
#define EF   2008.82608291633   // 622.3 Hz
#define D   2128.2770978362   // 587.3 Hz
#define DF   2254.83103972756   // 554.4 Hz
#define C0   2388.91027060714   // 523.3 Hz
#define B0   2530.96226744415   // 493.9 Hz
#define BF0   2681.46111557299   // 466.2 Hz
#define A0   2840.90909090909   // 440 Hz
#define AF0   3009.838336248   // 415.3 Hz
#define G0   3188.81263724254   // 392 Hz
#define GF0   3378.42930398501   // 370 Hz
#define F0   3579.32116447407   // 349.2 Hz
#define E0   3792.15867661942   // 329.6 Hz
#define EF0   4017.65216583266   // 311.1 Hz
#define D0   4256.55419567239   // 293.7 Hz
#define DF0   4509.66207945512   // 277.2 Hz
#define C7   4777.82054121429   // 261.6 Hz
#define B7   5061.9245348883   // 246.9 Hz
#define BF7   5362.92223114599   // 233.1 Hz
#define A7   5681.81818181819   // 220 Hz
#define AF7   6019.676672496   // 207.7 Hz
#define G7   6377.62527448508   // 196 Hz
#define GF7   6756.85860797002   // 185 Hz
#define F7   7158.64232894815   // 174.6 Hz
#define E7   7584.31735323884   // 164.8 Hz
#define EF7   8035.30433166532   // 155.6 Hz
#define D7   8513.10839134479   // 146.8 Hz
#define DF7   9019.32415891024   // 138.6 Hz
#define C6   9555.64108242859   // 130.8 Hz
#define	B6		10128.6697									// 123.4 Hz
#define Rest 0x00FFFFFF					// No sound

#define ShortestNote 4934210				// 32nd note ||  about 114 bpm
#define Whole				32
#define Half				16
#define Dotted_Quarter  12
#define Quarter 		8
#define Eighth 			4
#define Sixteenth 	2

												
//const soundByte* currSound = &music;
//soundByte music(tfmusic, 1038336);
//const soundByte* currSound = &music;
//const soundByte* prevSound = currSound;
enum Sound{SONG, SHOOT, KILL, MINE, DEATH};
int currIndex;
uint8_t currSound;
uint8_t prevSound;
const int Size[5] = {44, 44, 44, 44, 16};
const int soundFreq[5] = {0x00FFFFFF, C7, 11128, C1, 0x00FFFFFF};

void Sound_Init(void){
	DAC_Init();
	/*currSound = &music;
	prevSound = &music;*/
	currIndex = 0;
	currSound = SONG;
	prevSound = SONG;
	Timer0_Init(&Timer0_Handler,4000000);
	SysTick_Init();
};

void SysTick_Init(void){
// you write this as part of Lab 5
	NVIC_ST_CTRL_R &= ~(0x01);
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x07;
	
	
}

//void Sound_Play(const uint8_t *pt, uint32_t count){};

struct SongStruct{ 	uint32_t Note;
										uint32_t On;					// Total # of 32nd notes
										uint32_t Off;					// Spacing between Notes
												};
typedef const struct SongStruct Song_t;
												
Song_t music[44] = {{G0, Sixteenth, 0}, {A0, Sixteenth, 0}, {C0, Sixteenth, 0}, {B0, Sixteenth, 0}, {C0, Eighth, 1}, {F0, Eighth, 1}, {G0, Sixteenth, 0}, {A0, Sixteenth, 0},
										{C0, Sixteenth, 0}, {B0, Sixteenth, 0}, {C0, Eighth, 1}, {F0, Eighth, 1}, {G0, Sixteenth, 0}, {A0, Sixteenth, 0}, {C0, Sixteenth, 0}, 
										{B0, Sixteenth, 0}, {C0, Eighth, 1}, {G0, Eighth, 1}, {C0, Eighth, 1}, {C7, Sixteenth, 0}, {B7, Sixteenth, 0}, {C7, Eighth, 1}, {G0, Eighth, 1},
										{D0, Eighth, 1}, {A7, Eighth, 1}, {G7, Sixteenth, 0}, {B7, Sixteenth, 0}, {GF7, Eighth, 1}, {A7, Eighth, 1}, {G7, Eighth, 1}, {F7, Sixteenth, 0},
										{G7, Sixteenth, 0}, {E7, Eighth, 1}, {G7, Eighth, 1}, {GF7, Eighth, 1}, {E7, Sixteenth, 0}, {G7, Sixteenth, 0}, {EF7, Eighth, 1},
										{B6, Eighth, 1}, {E7, Eighth, 1}, {C7, Sixteenth, 0}, {D0, Sixteenth, 0}, {E0, Sixteenth, 0}, {F0, Sixteenth, 0}};						// Measure 8
									
Song_t death[16] = {{A0, Sixteenth, 0}, {G0, Sixteenth, 0}, {F0, Sixteenth, 0}, {E0, Sixteenth, 0}, {G0, Sixteenth, 0}, {F0, Sixteenth, 0}, {E0, Sixteenth, 0}, {D0, Sixteenth, 0}, 
										{F0, Sixteenth, 0}, {E0, Sixteenth, 0}, {D0, Sixteenth, 0}, {C7, Sixteenth, 0}, {E0, Sixteenth, 0}, {D0, Sixteenth, 0}, {C7, Sixteenth, 0}, {B7, Sixteenth, 0}};
																						
int On_Off;		
int Counter = 0;
int Next = 1;												

void Timer0_Handler(void){
	//if(prevSound!=currSound){currIndex=0;}
	if(currIndex>=Size[currSound]){
		currIndex=0;currSound=SONG;}
	//DAC_Out(currSound->samples[currIndex]);
	//DAC_Out(tfmusic[currIndex]);

	//currIndex++;
	if(Next == 1){
		Next = 0;
		currIndex ++;
		if(currIndex >= Size[currSound]){
			currIndex = 0;
		}
	}
	currIndex%=1038336;
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout  
	if(currSound == SONG){
		if(On_Off == 0){ 
			NVIC_ST_CTRL_R &= ~(0x01);
			NVIC_ST_RELOAD_R = music[currIndex].Note;		// Set Reload value to Frequency
			NVIC_ST_CURRENT_R = 0;
			NVIC_ST_CTRL_R |= 0x01;
			if(Counter <= music[currIndex].On - music[currIndex].Off){			// Runs number of 32nd notes in Requested note
				Counter += 1;
			}	
			if(Counter > music[currIndex].On - music[currIndex].Off){
				On_Off = 1;									// Switch to Off
				Counter = 0;
			}
		}
		if(On_Off  == 1){
			NVIC_ST_CTRL_R &= ~(0x01);
			NVIC_ST_RELOAD_R = 0x00FFFFFF;		// Set Reload value to Frequency
			NVIC_ST_CURRENT_R = 0;
			NVIC_ST_CTRL_R |= 0x01;
			if(Counter <= music[currIndex].Off){			// Runs number of 32nd notes in Requested note
				Counter += 1;
			}
			if(Counter > music[currIndex].Off){
				On_Off = 0;												// Switch to Off
				Counter = 0;
				Next = 1;
			}
		}
	}
	else if(currSound == DEATH){
		if(On_Off == 0){ 
			NVIC_ST_CTRL_R &= ~(0x01);
			NVIC_ST_RELOAD_R = death[currIndex].Note;		// Set Reload value to Frequency
			NVIC_ST_CURRENT_R = 0;
			NVIC_ST_CTRL_R |= 0x01;
			if(Counter <= death[currIndex].On - death[currIndex].Off){			// Runs number of 32nd notes in Requested note
				Counter += 1;
			}	
			if(Counter > death[currIndex].On - death[currIndex].Off){
				On_Off = 1;									// Switch to Off
				Counter = 0;
			}
		}
		if(On_Off  == 1){
			NVIC_ST_CTRL_R &= ~(0x01);
			NVIC_ST_RELOAD_R = 0x00FFFFFF;		// Set Reload value to Frequency
			NVIC_ST_CURRENT_R = 0;
			NVIC_ST_CTRL_R |= 0x01;
			if(Counter <= death[currIndex].Off){			// Runs number of 32nd notes in Requested note
				Counter += 1;
			}
			if(Counter > death[currIndex].Off){
				On_Off = 0;												// Switch to Off
				Counter = 0;
				Next = 1;
				currSound = SONG;
			}
		}
	}
	else{
		NVIC_ST_CTRL_R &= ~(0x01);
		NVIC_ST_RELOAD_R = soundFreq[currSound];		// Set Reload value to Frequency
		NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R |= 0x01;
		On_Off = 0;												// Switch to Off
		Counter = 0;
		currSound = SONG;
	}
}


//void Sound_Music(void){currSound=&music;};
//void Sound_Shoot(void){currSound=&shoot;};
//void Sound_Select(void){currSound=&zing;};
//void Sound_Kill(void){currSound=&zow;};
