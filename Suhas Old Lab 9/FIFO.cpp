// FIFO.cpp
// Runs on any microcontroller
// Student names: put your names here
// Last modification date: change this to the last modification date or look very silly
// Last Modified: 4/11/2018 

#include <stdint.h>

#include "FIFO.h"
#include "ST7735.h"
#include "print.h"

// A class named Queue that defines a FIFO
Queue::Queue(){
  // Constructor - make FIFO initially empty
  this->PutI=0;this->GetI=0;this->race=0;
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){return this->race==0;}

  // To check whether Queue is full or not
bool Queue::IsFull(void){return this->race>=31;}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
	if(this->IsFull()) return false;
	this->Buf[this->PutI]=x;PutI++; PutI%=32;
	this->race++;
  return true;
}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
  if(PutI==GetI){return false;}
	*pt=this->Buf[this->GetI];this->GetI++;GetI%=32;
	this->race--;
	return true;
}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){for(int i=GetI;i<PutI;i++)ST7735_OutChar(this->Buf[i]);}


