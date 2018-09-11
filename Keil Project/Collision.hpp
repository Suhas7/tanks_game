// Collision.hpp
// Runs on LM4F120/TM4C123
// Provide functions that detect and respond to object collisions
// Created: 04/28/2018 
// Student names: Suhas Raja & Diana Shao
// Last modification date: 04/28/18

#include <stdint.h>
#include "../tm4c123gh6pm.h"
#include "Controller.hpp"
#include "frameProcessing.hpp"
#include "Kentec.hpp"
#include "ADC.hpp"
#include <cmath>
#ifndef COLLISION_HPP
#define COLLISION_HPP

int checkUp(Controller& in);
int checkDown(Controller& in);
int checkLeft(Controller& in);
int checkRight(Controller& in);
void checkEdges(Controller& in);
bool playerCollision(Controller& a, Controller& b);
bool inSameSpace(Controller in);
int getCornerTile(Bullet in);
#endif
