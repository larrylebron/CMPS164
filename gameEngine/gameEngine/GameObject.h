#pragma once
#include "util.h"

class GameObject : public IMMObject
{
public:
	GameObject(void);
	~GameObject(void);
private:
	int id;
};

