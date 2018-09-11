// main.c
// Runs on LM4F120 or TM4C123
// Student names: put your names here
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/11/2018 

// Analog Input connected to PD2=ADC5
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// EE319K Lab 9, use U1Rx connected to PC4 interrupt
// EE319K Lab 9, use U1Tx connected to PC5 busy wait
// EE319K Lab 9 hardware
// System 1        System 2
//   PC4 ----<<----- PC5
//   PC5 ---->>----- PC4
//   Gnd ----------- Gnd

// main1 Understand UART interrupts
// main2 Implement and test the FIFO class on the receiver end 
//   import ST7735 code from Lab 7,8
// main3 convert UART0 to UART1, implement busy-wait on transmission
// final main for Lab 9
//   Import SlidePot and ADC code from Lab8. 
//   Figure out what to do in UART1_Handler ISR (receive message)
//   Figure out what to do in SysTickHandler (sample, convert, transmit message)
//   Figure out what to do in main (LCD output)

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "PLL.h"
#include "SlidePot.h"
#include "print.h"
#include "UART.h"
#include "FIFO.h"

SlidePot Sensor(2115, 254);
char arr[8];
int TxCounter = 0;

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);
extern Queue RxFifo;

// PF1 should be toggled in UART ISR (receive data)
// PF2 should be toggled in SysTick ISR (60 Hz sampling)
// PF3 should be toggled in main program
#define PF1  (*((volatile uint32_t *)0x40025008))
#define PF2  (*((volatile uint32_t *)0x40025010))
#define PF3  (*((volatile uint32_t *)0x40025020))
#define PF4  (*((volatile uint32_t *)0x40025040))

struct Message{
	uint8_t length;
	char* string;
};typedef struct Message message_t;

const message_t msg[4] = {{1, "\nPlayer "}, {1, "\nScore: "}, {1, "\nLives: "}, {1, "\nAmmo: "}};

char toASCII(char in){
	return (in + 0x30);
}

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = period;  					// 60 Hz
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
  NVIC_ST_CTRL_R = 7;  									// enable SysTick with core clock
}

// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x20;      // activate port F
  while((SYSCTL_PRGPIO_R&0x20) != 0x20){};
  GPIO_PORTF_DIR_R |=  0x0E;   // output on PF3,2,1 (built-in LED)
  GPIO_PORTF_PUR_R |= 0x10;
  GPIO_PORTF_DEN_R |=  0x1E;   // enable digital I/O on PF
}


// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX interrupt
int main(void){   
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  UART_Init();    // initialize UART
  // you write this
	
	SysTick_Init(1333333);
	EnableInterrupts();
	
	int y = 40;
	int frameCount;
	int barX = 0;
	int barY = 148;
	ST7735_FillRect(barX, barY, 128, 8, ST7735_BLUE);

	
  while(1){ // runs every 16,67 ms
		char pt;
		if(RxFifo.Get(&pt) && (pt == 0x02)){
			while(!RxFifo.Get(&pt));
 			frameCount = pt;
				for(int i = 0; i < 4; i++){
					while(!RxFifo.Get(&pt));
					pt = RxFifo.Get(&pt);
					for(int j = 0; j < 4; j++){
						ST7735_OutString(msg[j].string);							//print category
						for(int k = 0; k < msg[j].length; k++){				//print number
							ST7735_OutChar(toASCII(pt));
						}
					}
					ST7735_OutChar(0x0A);
					ST7735_DrawFastHLine(0, y, 128, ST7735_BLUE);
					y += 32;
				}
		ST7735_FillRect(barX, barY, frameCount, 8, ST7735_GREEN);
		ST7735_SetCursor(0, 0);

		}				
	}
}

void chop(){
	int distance = Sensor.Distance();	
	arr[0] = (0x02);
	arr[1] = (distance/1000 + '0');
	distance %= 1000;
	arr[2] = (0x2E);
	arr[3] = (distance/100 + '0');
	distance %= 100;
	arr[4] = (distance/10 + '0');
	distance %= 10;
	arr[5] = (distance + '0');
	arr[6] = (0x0D);
	arr[7] = (0x03);
}

void SysTick_Handler(void){ // every 16.67 ms
 //Similar to Lab8 except rather than grab sample,
// form a message, transmit 
  PF2 ^= 0x04;  // Heartbeat
  PF2 ^= 0x04;  // Heartbeat
	Sensor.Save(ADC_In());
	chop();
	for(int i = 0; i < 8; i++){
		UART_OutChar(arr[i]);
	}
	TxCounter++;
  PF2 ^= 0x04;  // Heartbeat
}

