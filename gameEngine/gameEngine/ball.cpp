#include "ball.h"


ball::ball(void)
{
}

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

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position << endl;
	return ss.str();
}
