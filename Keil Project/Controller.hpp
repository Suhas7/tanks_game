#include "Joystick.hpp"
#ifndef INCLUDED_CONTROLLER_HPP
#define INCLUDED_CONTROLLER_HPP
class Bullet{
	public:
	int ID;
	int x;
	int y;
	double angle;
	int vel;
	bool active;
};

class Controller{
	private:
	int ID;
	Joystick stick;
	int aBut;
	int bBut;
	public:
	int speed;
	int xPos;
	int yPos;
	int prevX;
	int prevY;
	int numBullets;
	int numMines;
	int score;
	int lives;
  public:
	int lastA;
	int lastB;
	int initX;
	int initY;
	Controller();
	bool active;
	bool hit;
	Controller(int n, int slope, int offset);
	uint8_t getID();
	void setXPos(int in){xPos=in;}
	void setYPos(int in){yPos=in;}
	int getXPos(){return xPos;}
	int getYPos(){return yPos;};
	void setPrevXPos(int in){prevX = in;};
	void setPrevYPos(int in){prevY = in;};
	int getPrevXPos(){return prevX;}
	int getPrevYPos(){return prevY;};
	int getLives(){return lives;};
	double getJAngle();
	int getJX();
	int getJY();
	bool getA();
	bool getB();
	void setID(int i);
	void update(uint32_t data[]);
	void move();
	void revMove();
	void shoot();
	void mine();
	bool checkHit(Bullet& in, int i);
	bool checkMine();
};

#endif
