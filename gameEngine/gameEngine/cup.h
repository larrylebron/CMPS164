#pragma once
#include "util.h"
#include "externalLibs\vec3f.h"
class cup : public IMMObject
{
public:
	cup();
	cup(int pID, Vec3f pPosition);
	~cup();
	Vec3f getPosition();
	string toString();
	AUTO_SIZE;
private:
	int id;
	Vec3f position;
};
