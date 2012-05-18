#include "ball.h"


ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius)
	: GameObject(pID), Drawable(pColor)
{
	position = pPosition;
	radius = pRadius;

	velocity = Vec3f(0,0,0);
	lastFrameTime = 0;
}


ball::~ball(void)
{
}

Vec3f ball::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

void ball::setVelocity(Vec3f pVelocity) {
	velocity = pVelocity;
}

Vec3f ball::getVelocity() {
	Vec3f velCopy = velocity;
	return velCopy;
}

void ball::setCurrSurfaceNormal(Vec3f pNormal) {
	if (currSurfaceNormal[0] != pNormal[0] || 
		currSurfaceNormal[1] != pNormal[1] || 
		currSurfaceNormal[2] != pNormal[2] ) {
		//calcNewDirection();
		cout << "new normal: " << pNormal;
	}
	currSurfaceNormal = pNormal;
}

void ball::draw() {
	//draw the ball
	renderManager::Instance()->drawSphere(radius, normal, position, color);
}

void ball::doSimulation(float currTime) {
	//update timer
	float timeElapsed = currTime - lastFrameTime;
	lastFrameTime = currTime;

	position = PhysicsManager::Instance()->calcPosition(position, velocity, timeElapsed);
	
}

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position[0] <<
		", " << position[1] << ", " << position[2] << endl;
	return ss.str();
}
