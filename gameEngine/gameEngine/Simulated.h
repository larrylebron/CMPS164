#pragma once

#include <queue>
#include "util.h"
#include "PhysicsManager.h"

class Simulated
{
public:
	Simulated();
	virtual ~Simulated(void){}
	virtual void doSimulation(bool AIProjection) = 0; //must implement a physics simulation
	Vec3f getVelocity(){return velocity;}
	void setVelocity(Vec3f pVelocity) {velocity = pVelocity;}
	bool isActive() {return active;} //return active status
	void applyForce(Vec3f force); //apply a force to the object
protected:
	Vec3f velocity;
	double lastFrameTime; //in seconds
	Timer timer;//timer for frame calculation
	bool resolveCollision; //flag that indicates a future collision has been detected
	PhysicsManager* pM;
	bool active; //true when the object is in motion
	std::queue<Vec3f> newForces;//the forces queued for this frame

};

