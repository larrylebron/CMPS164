#include "AIPlayer.h"


AIPlayer::AIPlayer(void)
{
}


AIPlayer::~AIPlayer(void)
{
}

void AIPlayer::takeTurn(int currBall) {
		
	currLev->saveState(); //save the level's state
	
	int numSims = 200;
	float minDist = 1000000; //dummy value for a high distance
	Vec3f finalShot;

	for (int i = 0; i < numSims; i++) {
		//random test shot
		float angle =  (float)rand()/(float)RAND_MAX * (2 * PI);
		float magnitude = (float)rand()/(float)RAND_MAX * MAX_POWER;
		Vec3f testShot(angle, magnitude);

		//run the simulation on this shot
		float newDist = currLev->runAIBallSimulation(currBall, testShot);
			
		//track max score achieved by a test shot
		if (newDist < minDist) {
			minDist = newDist;
			finalShot = testShot;
		}

		currLev->restoreState();
	}

	currLev->getBall(currBall)->applyForce(finalShot);
	
}
