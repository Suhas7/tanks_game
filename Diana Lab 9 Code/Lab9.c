// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/10/2017 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
#include "Systick.h"

//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm


struct Message{
	uint8_t length;
	char* string;
};typedef struct Message message_t;

const message_t msg[4] = {{1, "\nPlayer "}, {3, "\nScore: "}, {2, "\nLives: "}, {2, "\nAmmo: "}};

char toASCII(char in){
	return (in + 0x30);
}

// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX
int main(void){ 
  
 // TExaS_Init();       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
 // ADC_Init();    // initialize to sample ADC
	FiFo_Init();
  UART_Init();       // initialize UART
	ST7735_SetCursor(0,0);
  LCD_OutFix(0x30);
  EnableInterrupts();
	int x = 32;
	int frameCount;
	int barX = 0;
	int barY = 148;
  while(1){
		char Input;
		FiFo_Get(&Input);
		while(Input != 0x02){
			FiFo_Get(&Input);
		}
		
		FiFo_Get(&Input);
		frameCount = Input;
		for(int i = 0; i < 4; i++){
			Input = FiFo_Get(&Input);
			for(int j = 0; j < 4; j++){
				ST7735_OutString(msg[i].string);							//print category
				for(int k = 0; k < msg[j].length; k++){				//print number
					ST7735_OutChar(toASCII(Input));
				}
			}
			ST7735_OutChar(0x0A);
			ST7735_DrawFastHLine(x, 0, 128, ST7735_BLUE);
			x += 32;
		}
		ST7735_FillRect(barX, barY, 128, 8, ST7735_BLUE);
		ST7735_FillRect(barX, barY, frameCount, 8, ST7735_GREEN);
	}
}


