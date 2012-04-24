#include "cup.h"


cup::cup()
{
}

cup::cup(int pID, Vec3f pPosition)
{
	id = pID;
	position = pPosition;
}


cup::~cup()
{
}

Vec3f cup::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

void cup::printInfo() {
	cout << "cup id: " << id << endl;
	cout << "position: " << position << endl;
}
