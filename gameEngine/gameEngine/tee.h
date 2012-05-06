#pragma once
#include "util.h"
#include "externalLibs\vec3f.h"
class tee :
	public IMMObject
{
public:
	tee();
	tee(int pID, Vec3f pPosition);
	~tee();
	Vec3f getPosition();
	string toString();
	AUTO_SIZE;
private:
	int id;
	Vec3f position;
};

