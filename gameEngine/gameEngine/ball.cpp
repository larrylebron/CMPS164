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

void ball::printInfo() {
	cout << "ball id: " << id << endl;
	cout << "position: " << position << endl;
}
