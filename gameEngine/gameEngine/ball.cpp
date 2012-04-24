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

void ball::printInfo() {
	cout << "ball id: " << id << endl;
	cout << "position: " << position << endl;
}
