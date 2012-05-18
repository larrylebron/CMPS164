#pragma once
#include "util.h"

class GameObject 
	: public IMMObject
{
public:
	GameObject(int pID, Vec3f normal=DEFAULT_NORMAL);
	~GameObject(void){}
	int getID(); //return the object id
	AUTO_SIZE;
protected:
	Vec3f normal;
	int id;
};

