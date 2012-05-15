#include "ball.h"


ball::ball(int pID, Vec3f pPosition)
{
	id = pID;
	position = pPosition;
}


ball::~ball(void)
{
}

Vec3f ball::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

Vec3f ball::getVelocity() {
	Vec3f velCopy = velocity;
	return velCopy;
}

void ball::draw() {
	//draw the ball
	//renderManager::Instance()->drawSphere(float size, Vec3f position, Vec3f color);
}

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position[0] <<
		", " << position[1] << ", " << position[2] << endl;
	return ss.str();
}
