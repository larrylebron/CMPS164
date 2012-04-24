#pragma once
#include "util.h"
#include "level.h"
class renderManager : public IMMObject
{
public:
	renderManager();
	~renderManager();

	//draw the level
	void drawLevel(CMMPointer<level> lev);

	AUTO_SIZE;
};

