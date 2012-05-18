#pragma once

#include "util.h"
#include "PhysicsManager.h"
class Simulated
{
public:
	virtual ~Simulated(void){}
	virtual void doSimulation(float currTime) = 0; //must implement a physics simulation
protected:
	Vec3f velocity;
	float lastFrameTime; //in seconds
};

