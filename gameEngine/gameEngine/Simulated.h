#pragma once

#include "util.h"
#include "PhysicsManager.h"
class Simulated
{
public:
	Simulated();
	virtual ~Simulated(void){}
	virtual void doSimulation() = 0; //must implement a physics simulation
	Vec3f getVelocity(){return velocity;}
	void setVelocity(Vec3f pVelocity) {velocity = pVelocity;}
protected:
	Vec3f velocity;
	double lastFrameTime; //in seconds
	Timer timer;//timer for frame calculation
	bool resolveCollision; //flag that indicates a future collision has been detected
	PhysicsManager* pM;
};

