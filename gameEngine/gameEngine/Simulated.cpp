#include "Simulated.h"

Simulated::Simulated() {
	pM = PhysicsManager::Instance();
	velocity = Vec3f(0,0,0); 
	timer.start();//start the timer
	lastFrameTime = timer.getElapsedTimeInSec();
	resolveCollision = false;
}