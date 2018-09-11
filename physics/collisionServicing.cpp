#include "Actors.hpp"
#include <cmath>
//GENERAL PHYSICS
//MOVEMENT PROCESSING
bool checkCollision(Object& a, ActiveObject& b){
	double xDist = (b.xPos-a.xPos);
	double yDist = (b.yPos-a.yPos);
	bool overlap = false;
	if(xDist<0) xDist*=-1;
	if(yDist<0) yDist*=-1;
	if(xDist<=(a.colliderWidth+b.colliderWidth)) overlap = true;
	if(yDist<=(a.colliderHeight+b.colliderHeight)) overlap = true;
	if(!overlap)
}
bool checkCollision(Object& a, Object& b){ //check if colliders intersect
	double xDist = (b.xPos-a.xPos);
	double yDist = (b.yPos-a.yPos);
	if(xDist<0) xDist*=-1;
	if(yDist<0) yDist*=-1;
	if(xDist<=(a.colliderWidth+b.colliderWidth)) return true;
	if(yDist<=(a.colliderHeight+b.colliderHeight)) return true;
	return false;
}
bool boop(double angle){return(((angle>60) && (angle<120)) || ((angle>240) && (angle<300)));}
//finds angle of collision
double collisionAngle(Object& a, Object& b){return atan((a.yPos-b.yPos)*(a.yPos-b.yPos)/((a.xPos-b.xPos)*(a.xPos-b.xPos)))*180/PI;}
void serviceCollision(Archer& a,Archer& b){
    //if not upper quad: halts along axis of collision
    //else: kill player bc booperoni
	double angle = collisionAngle(a, b);
	if(boop(angle)){
		if(a.yPos<b.yPos){a.kill();}
		else{b.kill();}
	}else{
		if(a.xPos<b.xPos){a.xVel=-.2;b.xVel=.2;
		}else{b.xVel=-.2;a.xVel=.2;}
	}
	//poof noise
}
void serviceCollision(Archer& a,Arrow& b){
	if(b.grounded || a.dodged){a.catchArrow(b);}
	else if((!a.dodged) && (b.speed()>=fatalSpeed)){a.kill();b.halt();}
	//according sound FX, zing or blooge
}
void serviceCollision(Archer& a,Wall& b){ //if they're jumping laterally into wall, halt their y and bump them away a lil
	int dir;
	if((b.x1<a.xPos) && (b.x2<a.xPos)){dir=0;}
	else if((b.x1>a.xPos) && (b.x2>a.xPos)){
		dir=1;
	}
	else if((b.y1>a.yPos) && (b.y2>a.yPos)){
		dir=2;
	}
	else if((b.y1<a.yPos) && (b.y2<a.yPos)){
		dir=3;
	}
	switch(dir){
		case 0:a.halt();a.xPos+=.5;
		case 1:a.halt();a.xPos-=.5;
		case 2:a.haltY();a.yAcc=GRAVMAG;
		case 3:a.haltY();a.yAcc=0;
	}
}

void serviceCollision(Arrow& a,Archer& b){serviceCollision(b, a);};
void serviceCollision(Arrow& a,Arrow& b){
	a.halt();b.halt(); //halt both
	a.yVel=.2;b.yVel=.2; //bump up a lil up
	if(a.xPos<b.xPos){a.xVel=-.1;b.xVel=.1; //bump up a lil side/side
	}else{a.xVel=.1;b.xVel=-.1;}
}
void serviceCollision(Arrow& a,Wall& b){
	a.halt();
	a.grounded=true;
}
void serviceCollision(Wall& a,Archer& b) {serviceCollision(b, a);};
void serviceCollision(Wall& a,Arrow& b)  {serviceCollision(b, a);};
void serviceCollision(Wall& a,Wall& b);//wtf
	//phase thru?