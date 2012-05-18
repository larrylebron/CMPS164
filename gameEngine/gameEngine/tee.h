#ifndef TEE_H
#define TEE_H

#pragma once
#include "util.h"
#include "GameObject.h"
class tee :
	public GameObject
{
public:
	tee(int pID, Vec3f pPosition);
	~tee();
	Vec3f getPosition();
	string toString();
	AUTO_SIZE;
private:
	Vec3f position;
};

#endif