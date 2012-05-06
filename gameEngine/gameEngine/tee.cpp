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

string tee::toString() {
	std::stringstream ss;
	ss << "tee id: " << id << endl << "position: " << position << endl;
	return ss.str();
}
