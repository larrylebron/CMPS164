#pragma once
#include "util.h"
#include "PhysicsManager.h"

class GameObject 
	: public IMMObject
{
public:
	GameObject(int pID, Vec3f normal=WORLD_UP_VECTOR);
	~GameObject(void){}
	int getID(); //return the object id
	Vec3f getNormal();
	AUTO_SIZE;
protected:
	Vec3f normal;
	int id;
};

