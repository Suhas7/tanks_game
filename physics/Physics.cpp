//KEEP AN EYE OUT FOR GETTING STUCK ON WALLS
#include "Actors.hpp"

//#include <cmath>
//GENERAL PHYSICS
	//ISINSTANCE FUNCTIONS
		template<typename T, typename K>
		inline bool isType(const K &k) {
    		return typeid(T).hash_code() == typeid(k).hash_code();
		}
//        bool isType<Wall>(Object& in){return dynamic_cast<Wall*>(in) != nullptr; }
//        bool isType<Archer>(Object& in){return (dynamic_cast<Archer*>(in))!=nullptr;}
//        bool isType<Arrow>(Object& in){return (dynamic_cast<Arrow*>(in))!=nullptr;}
	//MOVEMENT PROCESSING
		bool checkCollision(Object& a, Object& b){ //check if colliders intersect
			double xDist = (b.xPos-a.xPos);
			double yDist = (b.yPos-a.yPos);
			if(xDist<0) xDist*=-1;
			if(yDist<0) yDist*=-1;
			if(xDist<=(a.colliderWidth+b.colliderWidth)) return true;
			if(yDist<=(a.colliderHeight+b.colliderHeight)) return true;
			return false;
			/* RADIAL IMPLEMENTATION
				double xDist = (a->xPos-b->xPos)*(a->xPos-b->xPos);
				double yDist = (a->yPos-b->yPos)*(a->yPos-b->yPos);
				double sDist = (a->colliderRadius+b->colliderRadius);
				double threshold = sDist*sDist;
				return threshold>=(xDist+yDist) ? true:false;*/
		}

		void serviceCollision(Archer& a,Archer& b);
		void serviceCollision(Archer& a,Arrow& b);
		void serviceCollision(Archer& a,Wall& b);
		void serviceCollision(Arrow& a,Archer& b){serviceCollision(b, a)};
		void serviceCollision(Arrow& a,Arrow& b);
		void serviceCollision(Arrow& a,Wall& b);
		void serviceCollision(Wall& a,Archer& b) {serviceCollision(b, a)};
		void serviceCollision(Wall& a,Arrow& b)  {serviceCollision(b, a)};
		void serviceCollision(Wall& a,Wall& b);

        void processWallWall(Wall& a, Object& b){return;}
        	//phase thru?
        void processWallArcher(Wall& a, Archer& b){}
        //set angle var to angle of collision how do??
        //wall/player:
        //if angle is not down quadrant:
        //player halts both- key to wall jump, can wall hang
        //else:
        //halt the players Y velocity
        //scale down X velocity(?)
        void processWallArrow(Object& a, Arrow& b){b.halt();b.grounded=true;}
        bool boop(double angle){return(((angle>60) && (angle<120)) || ((angle>240) && (angle<300)))}
        void processArcherArcher(Archer& a,Archer& b){
        	double angle = collisionAngle(a, b);
        	if(boop(angle)){
        		Archer* whichDies;
        		Archer* whichLives;
        		if(a.yPos<b.yPos){whichDies= a;whichLives=b;}
        		else{whichDies=b;whichLives=a;}
        		whichDies->kill();
        	}else{
        		Archer* whichLeft;
        		Archer* whichRight;
        		if(a.xPos<b.xPos){whichLeft=a;whichRight=b
        		}else{whichLeft=b;whichRight=a}
        		whichLeft.xVel=-.2;
        		whichRight.xVel=.2;
        	}
        	//poof noise
        }
        //if not upper quad: halts along axis of collision
        //else: kill player bc booperoni
        void processArcherArrow(Archer& a, Arrow& b){
        	if(b.grounded || a.dodged){a.catchArrow(b)}
        	else if((!a.dodged) && arrowspeed>=fatal){a.kill();b.halt()}
        	//according sound FX
        }
        void processArrowArrow(Arrow& a,Arrow& b){
        	a.halt();b.halt(); //halt both
        	a.yVel=.2;b.yVel=.2; //bump up a lil up
        	Arrow* whichLeft;
    		Arrow* whichRight;
    		if(a.xPos<b.xPos){whichLeft=a;whichRight=b //bump up a lil side/side
    		}else{whichLeft=b;whichRight=a}
    		whichLeft.xVel=-.1;
    		whichRight.xVel=.1;
        }
        double collisionAngle(Object& a, Object& b){ //finds angle of collision
            double yDist = (a.yPos-b.yPos)*(a.yPos-b.yPos);
            double xDist = (a.xPos-b.xPos)*(a.xPos-b.xPos);
            return atan(yDist/xDist)*180/PI;
        }

	//MOVEMENT APPLICATION
        void ActiveObject::ActiveObject();
		void ActiveObject::processPhys(){
			this->applyAcc();
			this->applyVel();
			//iff arrow:
			//	this->angle=atan(this->yVel/this->xVel); //only for arrows?
		}
		void ActiveObject::applyVel(){ //increment pos by scaled Vel
			this->xPos+=this->xVel/60;
			while(this->xPos>xSIZE) this->xPos -= xSIZE;
			this->yPos+=this->yVel/60;
            while(this->yPos>ySIZE) this->yPos -= ySIZE;
		}
		void ActiveObject::applyAcc(){ //increment velocity by scaled Acc
			this->xVel+=this->xAcc/60;
			if(this->xVel>this->terminalX)this->xVel = this->terminalX ;
			this->yVel+=this->yAcc/60;
            if(this->yVel>this->terminalY)this->yVel = this->terminalY;
        }
		void ActiveObject::applyForce(double x, double y){
			this->xAcc+=x;
			this->yAcc+=y;
		}
	//MOVEMENT ERASURE
		void ActiveObject::haltX(){this->xVel=0;}
		void ActiveObject::haltY(){this->yVel=0;}
		//void ActiveObject::haltR(){this->rVel=0;}
		void ActiveObject::halt(){ this->haltX();this->haltY();}
//ARCHER METHODS
	//ARCHER STRUCTORS
		void Archer::Archer(){
			static int i=0;
			this->playerID=i;
            i++;
            ctrlr = new Controller(playerID);
			this->arrowInventory=new int[8]; //is this right lol
			for(this->inventorySize=0;this->inventorySize<3;this->inventorySize++){ //default arrows
				this->arrowInventory[this->inventorySize]=normal;
			}
			for(int in = 3; in<8; in++){this->arrowInventory[in]=sentinel;}
			this->dodged=0;
			this->grounded=true;
			this->aiming=false;
		}
		void Archer::~Archer(){
			delete this->ctrlr;
			delete[] this->arrowInventory;
		}
	//ARCHER BEHAVIOUR
		void Archer::catchArrow(Arrow& caughtArrow){
			if(this->inventorySize==4){caughtArrow.halt();return;} //if inventory full, halt arrow and return
			this->arrowInventory[inventorySize]=caughtArrow.arrowType; //else, add to inventory and destroy
			delete caughtArrow;
			//playSound(sByte[4]);
		}
		void Archer::jump(){
			if(this->grounded) this->yVel=JUMPMAG;
		}
		void Archer::aim(double angle){
			static int last = false;
			if(this->ctrlr->getB()){
				this->aiming=true;
				this->aimAngle = this->ctrlr->getJAngle();
				last=true;
			}else{
				if(last){shoot(this->ctrlr->getJAngle());}
				this->aiming=false;
			}
		}
		void Archer::shoot(double angle){
			Arrow* shotArrow = new Arrow(this->xPos, this->yPos, this->arrowInventory[0], angle);
			for(int i =1; i<this->inventorySize; i++){this->arrowInventory[i-1]=arrowInventory[i];}
			inventorySize--;
			shotArrow->angle=angle;
		}
		void Archer::dodge(double angle){
			this->xVel = DODGESPEED * cos(angle*PI/180);
			this->yVel = DODGESPEED * sin(angle*PI/180);
			this->dodged=15;
		}
        void Archer::refresh(){
            if(this->dodged){
            	this->dodged--;
            	if(!this->dodged) this->halt();
            }        //update dodge status, if nonzero
            if(grounded){yAcc=0;yVel=0;   //set yAcc in accordance w grounding
            }else{yAcc=GRAVMAG;}
            this->aim(this->ctrlr->getJAngle());     //update aim parameter
        }
//ARROW METHODS
    void Arrow::Arrow();
	void Arrow::Arrow(double xPos,double yPos,int type,double angle){
		this->xPos=xPos;
		this->yPos=yPos;
		this->xVel=(cos(angle*PI/180))*ARROWSPEED;
		this->yVel=(sin(angle*PI/180))*ARROWSPEED;
		this->yAcc=GRAVMAG;
		this->xAcc=0;
		this->angle =angle;
		terminalX=5;
	 	terminalY=7;
	}
	void Arrow::refresh(){
		this->angle =atan((this->yVel/this->xVel))*180/PI;
	}
