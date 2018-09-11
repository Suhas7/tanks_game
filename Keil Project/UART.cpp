// UART1.cpp
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// November 17, 2014
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "FiFo.h"
#include "UART.h"
#include "../tm4c123gh6pm.h"

uint32_t DataLost; 
uint32_t RxCounter;
// Initialize UART1
// Baud rate is 115200 bits/sec
// Make sure to turn ON UART1 Receiver Interrupt (Interrupt 6 in NVIC)
// Write UART1_Handler
void UART_Init(void){
   // --UUU-- complete with your code
	SYSCTL_RCGCUART_R |= 0x02;
	SYSCTL_RCGCGPIO_R |= 0x04;
	UART1_CTL_R &= ~0x01;		//disable during initialization
	UART1_IBRD_R = 43;		//integer
  UART1_FBRD_R = 26;	//fractional
  UART1_CTL_R |= 0x00000300;	 //enable transmitter and receiver for UART1
  UART1_CTL_R |= 0x00000001;    //enable UART1
	GPIO_PORTC_AFSEL_R |= 0x30;		//enable alt function
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF) + 0x00110000;
	GPIO_PORTC_DEN_R |= 0x30;		//digital I/O on PC5-4
	GPIO_PORTC_AMSEL_R &= ~0x30;	//no analog on PC5-4
	UART1_LCRH_R = 0x0070;  // 8-bit length, enable FIFO
	
	UART1_IM_R |= 0x10;			//receiver init
	UART1_IFLS_R = (UART1_IFLS_R & ~0x38) + 0x10; //1/2 full
	NVIC_PRI1_R |= 0x00E00000;  // bits 21-23
	NVIC_EN0_R |= 0x40; // enable interrupt 6 in NVIC
	RxCounter = 0;
	DataLost = 0;
}

// input ASCII character from UART
// spin if RxFifo is empty
char UART_InChar(void){
  while((UART1_FR_R&0x0010) != 0); // wait until RXFE is 0
  return((uint8_t)(UART1_DR_R&0xFF));
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
 while((UART1_FR_R&0x0020) != 0);  // wait until TXFF is 0
  UART1_DR_R = data;
}

// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR
/*void UART1_Handler(void){
  uint8_t i;
	for(i = 0; i < dataSize[i]; i++){
		if(FiFo_Put(UART_InChar()) == 0){		//put received data in FIFO
			DataLost++;
			break;
		}
	}
	
	UART1_ICR_R = 0x10;
	RxCounter++;
}*/
