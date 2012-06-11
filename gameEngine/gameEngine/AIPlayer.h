#pragma once

#include <map>
#include "util.h"
#include "level.h"

struct Shot{
	Vec3f shotVec;//the shot vector
	int endScore; //the score yielded by the shot
	float dist; //the shot's distance from the palino
};

class AIPlayer :
	public IMMObject
{
public:
	AIPlayer(void);
	~AIPlayer(void);

	void setLevel(CMMPointer<level> pCurrLev) {currLev = pCurrLev;}

	//Take a turn with the current ball
	void takeTurn(int currBall);

	//for IMMobject tracking
	AUTO_SIZE;
private:
	//a reference to the current level for simulation
	CMMPointer<level> currLev;
	
};



