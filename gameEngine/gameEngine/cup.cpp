#include "cup.h"


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

string cup::toString() {
	std::stringstream ss;
	ss << "cup id: " << id << endl << "position: " << position << endl;
	return ss.str();
}
