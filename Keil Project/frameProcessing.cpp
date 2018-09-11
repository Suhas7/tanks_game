// frameProcessing.cpp
// Runs on LM4F120/TM4C123
// Provide functions that process graphics and gameplay actions
// Created: 04/26/18
// Student names: Suhas Raja & Diana Shao
// Last modification date: 04/28/18

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "Controller.hpp"
#include "Kentec.hpp"
#include "frameProcessing.hpp"
#include "Collision.hpp"
#include "images.h"
#include "ADC.hpp"
#include "Sound.hpp"
#include <cmath>
#include <math.h>

#define STARTSCREEN 0
#define PLAYERSELECT 1
#define ROUND 2
#define ENDGAME 3
#define WALL 1
#define DIRT 0
#define MUD 2
#define MINE 3
#define speedUp 4
#define BULLVEL 4

#define PI 3.141592653589279323846264

const unsigned char* Tanks[4] = {greenTank, redTank, blueTank, goldTank};
const unsigned char* Tiles[5] = {dirt16, wall16,water16, mineSprite, powerUp1};
Bullet bullets[100];
extern Bullet bullets[100];
extern uint32_t data[8];
int bcount = 0;
extern int bcount;
static int state = 0;
extern const unsigned char* Tanks[4];
static bool newStartScreen = true;
static bool newSelectScreen = true;
static bool newRound = true;
static int rounds = 0;
static bool newEndGame = true;
int frameNum;
extern int frameNum;
int loopcount=0;
extern int currSound;
extern void DAC_Out(int x);

uint8_t stage1[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,1,1,1,1,1,0,4,0,0,1,0,0,0,0,0,4,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t stage2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,4,0,0,1,0,0,0,0,0,4,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t stage3[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
									 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,4,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,0,1,0,4,0,0,1,0,0,0,0,0,4,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
									 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,4,0,0,0,0,1,
									 1,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
									 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t stage[32*24];
int stageID = 0;
extern uint8_t stage[];
extern Controller Controllers[4];
//UART 
//output frame number, then controller ID, score, lives, ammo, repeat, then ETX
void ghostRemoval(const uint8_t stage[], Controller& in){
	int x = ((in.getPrevXPos())/10) %32;
	int y = (in.getPrevYPos())/10%24;
	for(int i = -1;i<4;i++){  //write over last location
		for(int j=-1;j<4;j++){
			if((x+i>-1) && (x+i<33) && (y+j>-1) && (y+j<25)){
				LCD_DrawBMP(Tiles[stage[((x+i)+32*(y+j)) % (32*24)]], 10*(x+i),10*(y+j));
			}
		}
	}
}
void ghostRemoval2(const uint8_t stage[], Controller& in){
	int x = ((in.getXPos())/10) %32;
	int y = (in.getYPos())/10%24;
	for(int i = 0;i<3;i++){  //write over last location
		for(int j=0;j<3;j++){
			if((x+i>-1) && (x+i<33) && (y+j>-1) && (y+j<25)){
				LCD_DrawBMP(Tiles[stage[((x+i)+32*(y+j)) % (32*24)]], 10*(x+i),10*(y+j));
			}
		}
	}
	if(in.getJX()<0){
		int xTile = in.getXPos()/10;
		int yTile = in.getYPos()/10;
		for(int i = 0;i<3;i++){
			LCD_DrawBMP(Tiles[stage[32*(i+yTile)+xTile]], xTile*10,yTile*10);
		}
	}else{
		int xTile = 4+in.getXPos()/10;
		int yTile = in.getYPos()/10;
		for(int i = 0;i<3;i++){
			LCD_DrawBMP(Tiles[stage[32*(i+yTile)+xTile]], xTile*10,yTile*10);
		}
	}
	if(in.getJY()>0){
		int xTile = in.getXPos()/10;
		int yTile = 4+in.getYPos()/10;
		for(int i = 0;i<3;i++){
			LCD_DrawBMP(Tiles[stage[i+32*yTile+xTile]], xTile*10,yTile*10);
		}
	}else{
		int xTile = in.getXPos()/10;
		int yTile = in.getYPos()/10;
		for(int i = 0;i<3;i++){
			LCD_DrawBMP(Tiles[stage[i+32*yTile+xTile]], xTile*10,yTile*10);
		}
	}
}

void frameHandler(){
	TIMER1_CTL_R = 0x0;
	ADC_In((uint32_t*) data);
	for(int i = 0; i<4;i++){Controllers[i].update((uint32_t*) data);}
	switch(state){ //process each state
		case STARTSCREEN:
			processStart();
			break;
		case PLAYERSELECT:
			processPlayerSelect();
			break;
		case ROUND:
			processRound();
			break;
		case ENDGAME:
			processEndGame();
			break;
	}
	TIMER1_CTL_R = 0x00000001;
	frameNum++;
}

void processStart(){
	//DRAW TITLE SCREEN
	//LCD_DrawBMP(startScreen, 0, 0);
	//START MUSIC
	//Output welcome message on ST7735
	if(newStartScreen){
		LCD_ColorFill(0); 											// black background
		LCD_DrawBMP(blueTank, 70, 70);
		LCD_DrawBMP(greenTank, 120, 70);
		LCD_DrawBMP(goldTank, 170, 70);
		LCD_DrawBMP(redTank, 220, 70);
		LCD_SetCursor(130, 200);
		LCD_SetTextColor(0xFF,0xFF,0xFF);
		char string[] = "Suhas Raja && Diana Shao";
		LCD_PrintString(string);
		LCD_SetCursor(140, 130);
		char string2[] = "TANKS";
		LCD_PrintString(string2);
//		LCD_DrawBMP(startTitle,75,10);	//title screen
//		LCD_DrawBMP(startSubTitle,75,10);	//title screen
		newStartScreen =false;
	}
	bool tmp = false;
	for(int i = 0; i<4; i++){tmp|=Controllers[i].getA();}
	if(tmp){state++;}
}
void processPlayerSelect(){
	//Write a black screen with grey boxes
	if(newSelectScreen){
		LCD_ColorFill(0); 											// black background
		LCD_DrawBMP(playerSelectTitle,75,10);	//title screen
		newSelectScreen=false;
	}
	//if any A buttons have been hit, activate controller
	static bool lastState[4] = {false,false,false,false};
	int tmp = 1;
	for(int i = 0; i<4; i++){
		if((Controllers[i].getA() && !(lastState[i]))){ //draw characters onto their boxes to indicate activity
			Controllers[i].active=true;
			lastState[i] = true;
			switch(i){			
				case 0: LCD_DrawBMP(greenTank,  70,  50);currSound = 3;break; 	//player select boxes
				case 1: LCD_DrawBMP(redTank, 225, 50);currSound = 3;break;
				case 2: LCD_DrawBMP(blueTank,  70, 145);currSound = 3;break;
				case 3: LCD_DrawBMP(goldTank, 225,145);currSound = 3;break;
			}
		}
	}
	//check if active players are ready
	for(int i = 0; i<4; i++){
		if(Controllers[i].active){tmp &=Controllers[i].getB();}
	}
	int playerCount=0;
	for(int i = 0;i<4;i++){if(Controllers[i].active>0) playerCount++;}

	//if they are ready, start game
	if(tmp &&(playerCount>1))state++;
}
void bullGhost(Bullet& in){
	int xTile = in.x/10;
	int yTile = in.y/10;
	for(int i = -1; i<2; i++){
		for(int j = -1; j<2; j++){
			if((xTile+i)<32 && (yTile+j)<24)
			LCD_DrawBMP(Tiles[stage[(xTile+i)+32*(yTile+j)]], 10*(xTile+i), 10*(yTile+j));
		}
	}
}
void processRound(){
	int xStart[2] = {50, 200};
	int yStart[2] = {50, 150};
	if(newRound){
		for(int i = 0;i<32;i++){
			for(int j = 0; j<24;j++){
				switch(stageID%3){
					case 0:
						stage[i+32*j]=stage1[i+32*j];
						break;
					case 1:
						stage[i+32*j]=stage2[i+32*j];
						break;
					case 2:
						stage[i+32*j]=stage3[i+32*j];
				}
			}
		}
		stageID++;
		LCD_ColorFill(0);
		LCD_SetCursor(145, 120);
		char roundString[10] = "ROUND    ";
		if(rounds/10)roundString[6]=0x30+(1+rounds-loopcount)/10;
		roundString[7]=0x30+(rounds+1-loopcount)%10;
		LCD_PrintString(roundString);
		bool tmp = false;
		while(!tmp){
			DAC_Out(0);
			for(int i = 0; i<4; i++){
				if(Controllers[i].active){
					tmp|=Controllers[i].getA();
					Controllers[i].update(data);
				}
			}
		}
			if(tmp){
			for(int i = 0; i<(24*32);i++){
				LCD_DrawBMP(Tiles[stage[i]], 10*(i%32),10*(i/32));
			}
			for(int i = 0; i<4; i++){
				if(Controllers[i].active){
					Controllers[i].setXPos(xStart[i/2]);
					Controllers[i].setYPos(yStart[i%2]);
					Controllers[i].numBullets=5;
					Controllers[i].numMines=3;
					Controllers[i].speed=800;
					LCD_DrawBMP(Tanks[Controllers[i].getID()], Controllers[i].getXPos(),Controllers[i].getYPos());
				}
			}
			newRound=false;
		}
	}
	for(int i = 0; i<4; i++){
		if(Controllers[i].active && Controllers[i].lives>0){
			Controllers[i].setPrevXPos(Controllers[i].getXPos());
			Controllers[i].setPrevYPos(Controllers[i].getYPos());
			Controllers[i].move();
			Controllers[i].checkMine();
			checkEdges(Controllers[i]);
			for(int j = 0;j<4;j++){
				if(Controllers[j].lives>0 && (j!=i)){
					if(playerCollision(Controllers[i], Controllers[j])){
						LCD_DrawBMP(Tanks[Controllers[i].getID()], Controllers[i].getXPos(),Controllers[i].getYPos());
						LCD_DrawBMP(Tanks[Controllers[j].getID()], Controllers[j].getXPos(),Controllers[j].getYPos());			
					}
				}
			}
			for(int k = 0; k<bcount;k++){ //per each active bullet, check if it hit anyone
				if(Controllers[i].checkHit(bullets[k], i)){
					currSound = 2;
					for(int i = bcount+1;i<99;i++){{bullets[i]=bullets[i+1];}}
				}
				if(bullets[k].active){
					int ok = true;
					for(int i = 0;i<2;i++){
						for(int j = 0; j<2;j++){
							if(stage[bullets[k].x/10+i+32*(bullets[k].y/10+j)]==1){
								bullets[k].active=false;
								for(int i = bcount+1;i<99;i++){{bullets[i]=bullets[i+1];}}
								ok = false;
								break;
							}
						}
					}
					bullGhost(bullets[k]);	
					if(ok){
						bullets[k].x-=BULLVEL*cos(bullets[k].angle*PI/180);
						bullets[k].y-=BULLVEL*sin(bullets[k].angle*PI/180);
						LCD_DrawBMP(bulletSprite, bullets[k].x, bullets[k].y);
					}
					
				}
			}
			if(!Controllers[i].lastA && Controllers[i].getA()){
				Controllers[i].shoot();
			}
			if(!Controllers[i].lastB && Controllers[i].getB()){
				Controllers[i].mine();
			}
			
			if(inSameSpace(Controllers[i])){
				ghostRemoval(stage, Controllers[i]);
				LCD_DrawBMP(Tanks[Controllers[i].getID()], Controllers[i].getXPos(),Controllers[i].getYPos());
			}
			if(Controllers[i].lives<=0){
				ghostRemoval(stage, Controllers[i]);
			}
			Controllers[i].lastA=Controllers[i].getA();
			Controllers[i].lastB=Controllers[i].getB();
			if(!(frameNum%300)){
				Controllers[i].numBullets++;
				if(Controllers[i].numBullets>6)Controllers[i].numBullets=6;
			}
		}
	}
	int playerCount=0;
	for(int i = 0;i<4;i++){if((Controllers[i].lives>0) && (Controllers[i].active>0)) playerCount++;}
	if(playerCount==1){
		for(int i = 0;i<4;i++){
		Controllers[i].lives=3;}
		newRound=true;
		rounds++;
	}
	if(!((1+rounds)%4)){state++;rounds++;loopcount++;}
}

void processEndGame(){
	//write end game
	//write score to main screen
	//write winner to HUD
	int tmp = 1;
	currSound = 4;
	if(newEndGame){
		LCD_ColorFill(0);
		int x = 30;
		int y = 60;
		int maxI=0;
		char scoreString[18] = "PLAYER  :    HITS";
		for(int i = 0;i<4;i++){
			if(Controllers[i].active){
				if(Controllers[i].score>Controllers[maxI].score) maxI=i;
				LCD_SetCursor(x, y);
				scoreString[7]=0x31+i;
				if(Controllers[i].score/10){
					scoreString[10]=0x30+Controllers[i].score/10;
					scoreString[11]=0x30+Controllers[i].score%10;
				}else{
					scoreString[10]=0x30+Controllers[i].score;
					scoreString[11]=' ';
				}
				LCD_PrintString(scoreString);
				y+=20;
			}
		}
		LCD_SetCursor(160, 40);
		char winString[15] = "PLAYER   WINS!";
		winString[7]=0x31+maxI;
		LCD_PrintString(winString);
		newEndGame=false;
	}
	tmp = 1;
	for(int i = 0; i<4; i++){//check if everyone's ready to restart
		if(Controllers[i].active){
			tmp &= Controllers[i].getB();
		}
	}
	if(tmp){
		state=2;
		newRound=true;
		newSelectScreen = true;
		newStartScreen=true;
		newEndGame=true;
	}
}
