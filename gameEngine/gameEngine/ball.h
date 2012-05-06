#pragma once
#include "util.h"
#include "externalLibs\vec3f.h"
class ball :
	public IMMObject
{
public:
	ball();
	ball::ball(int pID, Vec3f pPosition);
	~ball();
	Vec3f getPosition();
	string toString();
	AUTO_SIZE;
private:
	int id;
	Vec3f position;
};