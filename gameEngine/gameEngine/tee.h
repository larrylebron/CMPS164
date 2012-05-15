#ifndef TEE_H
#define TEE_H

#pragma once
#include "util.h"
#include "externalLibs\vec3f.h"
class tee :
	public IMMObject
{
public:
	tee(int pID, Vec3f pPosition);
	~tee();
	Vec3f getPosition();
	string toString();
	AUTO_SIZE;
private:
	int id;
	Vec3f position;
};

#endif