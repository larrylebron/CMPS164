#include "tee.h"


tee::tee()
{
}

tee::tee(int pID, Vec3f pPosition)
{
	id = pID;
	position = pPosition;
}


tee::~tee()
{
}

Vec3f tee::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

void tee::printInfo() {
	cout << "tee id: " << id << endl;
	cout << "position: " << position << endl;
}
