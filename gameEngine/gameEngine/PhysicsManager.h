#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#pragma once

#include <vector>
#include <map>
#include "Logger.h"
#include "ball.h"

using std::vector;
using std::map;


class PhysicsManager{
public:
   static PhysicsManager* Instance();//returns an instance of the manager
   
   //Pass in a list of forces, and return the result
   Vec3f calcForces();

   //calculate new position given initial position, velocity and time elapsed
   Vec3f calcPosition(Vec3f position, Vec3f velocity, float timeElapsed);

private:
	PhysicsManager(); //private constructor for singleton
	static PhysicsManager* m_pInstance; //static instance of the manager
	
	Logger* log; //for logging

};

#endif