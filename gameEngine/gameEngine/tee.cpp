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

void tee::printInfo() {
	cout << "tee id: " << id << endl;
	cout << "position: " << position << endl;
}
