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

void cup::printInfo() {
	cout << "cup id: " << id << endl;
	cout << "position: " << position << endl;
}
