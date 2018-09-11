// Collision.cpp
// Runs on LM4F120/TM4C123
// Provide functions that detect and respond to object collisions
// Created: 04/28/2018 
// Student names: Suhas Raja & Diana Shao
// Last modification date: 04/28/18

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "Controller.hpp"
#include "Collision.hpp"
#include "Kentec.hpp"
#include "frameProcessing.hpp"
#include "images.h"
#include "ADC.hpp"
#include <cmath>

#define PRAD 18
extern const uint8_t stage[];
extern uint32_t data[8];
int checkUp(Controller& in){
	int xTile = in.getXPos()/10;
	int yTile = in.getYPos()/10;
	for(int i = 0;i<3;i++){
		if(stage[i+32*yTile+xTile]){
		return stage[i+32*yTile+xTile];}
	}
	return 0;
}
int checkDown(Controller& in){
	int xTile = 1+in.getXPos()/10;
	int yTile = 3+in.getYPos()/10;
	for(int i = 0;i<3;i++){
		if(stage[i+32*yTile+xTile]){
		return stage[i+32*yTile+xTile];}
	}
	return 0;
}
int checkLeft(Controller& in){
	int xTile = in.getXPos()/10;
	int yTile = 1+in.getYPos()/10;
	for(int i = 0;i<2;i++){
		if(stage[32*(i+yTile)+xTile]){
		return stage[32*(yTile+i)+xTile];}
	}
	return 0;
}
int checkRight(Controller& in){
	int xTile = 3+in.getXPos()/10;
	int yTile = 1+in.getYPos()/10;
	for(int i = 0;i<2;i++){
		if(stage[32*(i+yTile)+xTile]){
		return stage[32*(yTile+i)+xTile];}
	}
	return 0;
}

void checkEdges(Controller& in){
	while(checkLeft(in)==1){in.setXPos((in.getXPos()+1)%320);}
	while(checkRight(in)==1){in.setXPos((in.getXPos()-1)%320);}
	while(checkUp(in)==1){in.setYPos((in.getYPos()+1)%240);}
	while(checkDown(in)==1){in.setYPos((in.getYPos()-1)%240);}
}
int absVal(int in){
	if(in<0){return -in;}
	return in;
}
bool playerCollision(Controller& a, Controller& b){
	int x1 = a.getXPos();
	int y1 = a.getYPos();
	int x2 = b.getXPos();
	int y2 = b.getYPos();
	int dist = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
	if(dist<((PRAD*2)*(PRAD*2))){a.revMove();b.revMove();return true;}
	return false;
}

bool inSameSpace(Controller in){
	int xDiff = in.getXPos()-in.getPrevXPos();
	if(xDiff<0)xDiff=-xDiff;
	int yDiff = in.getYPos()-in.getPrevYPos();
	if(yDiff<0)yDiff=-yDiff;
	return ((xDiff>1)||(yDiff >1));
}

int getCornerTile(Bullet in){return stage[in.x/10+32*(in.y/10)];}
