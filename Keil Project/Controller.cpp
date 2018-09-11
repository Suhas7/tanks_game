// Controller.cpp
// Runs on LM4F120/TM4C123
// A class representing a Controller object that includes a joystick and two buttons
// Created: 4/22/2016
// Student names: Diana Shao & Suhas Raja
// Last modification date: 04/24/18

//#include <cmath>
#include <math.h> 
#include "Controller.hpp"
#include "Kentec.hpp"
#include "../tm4c123gh6pm.h"
#include "Images.h"
#include "Collision.hpp"
#define PI 3.1415926535897932384626433832795028841971
#define cJ 1 //standardize joystick output
#define BULLVEL 8
#define PRAD 18

static int i = 0;
extern Bullet bullets[100];
extern int bcount;
extern uint8_t stage[];
extern const unsigned char* Tiles[3];
extern Controller Controllers[4];
extern const unsigned char* Tanks[4];
extern int currSound;

void controllerInit(){
	volatile int x = 0;
	SYSCTL_RCGCGPIO_R |= 0x3F;	//activate clock for port D/E
	x++;x++;x++;x++;	
	GPIO_PORTA_DIR_R &= ~0xC;  //PA Dir Inputs
	GPIO_PORTD_DIR_R &= ~0xCF;  //PD Dir Inputs
	GPIO_PORTE_DIR_R &= ~0x3F;  //PE Dir Inputs
	GPIO_PORTD_AFSEL_R = (GPIO_PORTD_AFSEL_R|0x03)&!(0xCC);  //AF
	GPIO_PORTE_AFSEL_R = (GPIO_PORTE_AFSEL_R|0xCF);  //AF
	GPIO_PORTA_DEN_R |= 0xC;
	GPIO_PORTD_DEN_R = (GPIO_PORTD_DEN_R&(~0x03))|(0xCC);  //DEN
	GPIO_PORTE_DEN_R = (GPIO_PORTE_DEN_R&(~0xCF));  //DEN
	GPIO_PORTD_AMSEL_R |= 0xCC;  //ANALOG ENABLE
	GPIO_PORTE_AMSEL_R |= 0xCF;  //ANALOG ENABLE
}

Controller::Controller(){
	this->ID=i;i++;
	this->active = false;
	this->score=0;
	this->numBullets=4;
	this->numMines=3;
	this->lives=3;
	this->speed=800;
	this->hit=false;
}

Controller::Controller(int n, int slope, int offset){
	this->ID=n;
	this->stick =Joystick(slope, offset);
}

uint8_t Controller::getID(){return this->ID;}

double Controller::getJAngle(){
	double x = this->stick.DistanceX();
	double y = this->stick.DistanceY();
	return atan2(y,x)*180/PI;
}

int Controller::getJX(){return this->stick.DistanceX();}
int Controller::getJY(){return this->stick.DistanceY();}
bool Controller::getA(){return this->aBut;}
bool Controller::getB(){return this->bBut;}
void Controller::update(uint32_t data[]){
	stick.Save(data[2*this->ID], data[2*this->ID+1], this->initX, this->initY);
	this->prevX = this->stick.DistanceX();
	this->prevY = this->stick.DistanceY();
	switch(this->ID){
		case 0:
			this->aBut = (GPIO_PORTD_DATA_R & 0x04)/0x4;
			this->bBut = (GPIO_PORTD_DATA_R & 0x08)/0x8;
			break;                                
		case 1:                                 
			this->aBut = (GPIO_PORTC_DATA_R & 0x40)/0x40;
			this->bBut = (GPIO_PORTC_DATA_R & 0x80)/0x80;
			break;                                
		case 2:                                 
			this->aBut = (GPIO_PORTD_DATA_R & 0x40)/0x40;
			this->bBut = (GPIO_PORTD_DATA_R & 0x80)/0x80;
			break;                                
		case 3:                                 
			this->aBut = (GPIO_PORTA_DATA_R & 0x08)/0x8;
			this->bBut = (GPIO_PORTA_DATA_R & 0x04)/0x4;
			break;
	}
}

void Controller::revMove(){
	this->xPos +=this->getJX()/this->speed;
	this->yPos +=this->getJY()/this->speed;
}

void Controller::move(){
	this->xPos -=this->getJX()/this->speed;
	this->yPos -=this->getJY()/this->speed;
}

void Controller::shoot(){
	Bullet bullet;
	if(numBullets<=0){return;}
	currSound = 1;
	bullet.ID=this->ID;
	bullet.angle=this->getJAngle();
	bullet.vel = BULLVEL;
	bullet.x = this->getXPos()+14; //figure out bullet position relative to tank
	bullet.y = this->getYPos()+14;
	bullets[bcount] = bullet;
	bullets[bcount].active=true;
	bcount++;
	this->numBullets--;
	LCD_DrawBMP(Tanks[i], xPos,yPos);
}

void mineShift(Controller* in){
	int xTile = in->getXPos()/10;
	int yTile = in->getYPos()/10;
	while(stage[32*yTile+xTile]==3){
		in->xPos++;
		in->yPos++;
		xTile = in->getXPos()/10;
		yTile = in->getYPos()/10;
	}
}

void Controller::mine(){
	if(this->numMines<=0){return;}
	currSound = 3;
	int xTile = this->xPos/10;
	int yTile = this->yPos/10;
	stage[32*yTile+xTile]=3;
	LCD_DrawBMP(mineSprite, 10*xTile, 10*yTile);
	mineShift(this);
	this->numMines--;
}

bool Controller::checkHit(Bullet& in, int i){
	if(!in.active) return false;
	int xDist = in.x-this->xPos-14;
	int yDist = in.y-this->yPos-14;
	int dist = xDist*xDist+yDist*yDist;
	if(in.ID==i){
		LCD_DrawBMP(Tanks[this->ID], xPos,yPos);
		return false;
	}
	if(dist<PRAD*PRAD){
		in.active=false;
		this->lives--;
		Controllers[in.ID].score++;
		//Controllers[i].hit=true;
		return true;
	}else{
		int xTile = in.x/10;
		int yTile = in.y/10;
		for(int i = 0; i<2;i++){
			for(int j = 0; j<2; j++){
				LCD_DrawBMP(Tiles[stage[32*(yTile+i)+xTile+j]], xTile*10, yTile*10);
			}
		}
		if(this->lives>0){
			LCD_DrawBMP(Tanks[this->ID], xPos,yPos);
		}
		return false;
	}
}

bool Controller::checkMine(){
	int xTile=this->xPos/10;
	int yTile = this->yPos/10;
	for(int i = 0;i<3;i++){
		for(int j = 0; j<3;j++){
			if(stage[32*(yTile+j)+xTile+i]==3){
				this->lives-=2;
				stage[32*(yTile+j)+xTile+i]=0;
			}else if(stage[32*(yTile+j)+xTile+i]==4){
				this->speed-=70;
				stage[32*(yTile+j)+xTile+i]=0;
			}
		}
	}
	return false;
}

