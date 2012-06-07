#include "ball.h"

//ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius, CMMPointer<level> pCurrLev)
ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius)
	: GameObject(pID), Drawable(pColor)
{
	position = pPosition;
	radius = pRadius;

	currTile = 0;
	tileMap = 0;

	inCup = false;
}


ball::~ball(void)
{
}

bool ball::isInCup() {
	return inCup;
}

Vec3f ball::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

void ball::setPosition(Vec3f pos) {
	position = pos;
}

void ball::setTileMap(std::map<int, CMMPointer<tile>>* newTileMap) {
	tileMap = newTileMap;
}

void ball::setCupPos(Vec3f pCupPos) {
	cupPos = pCupPos;
}

void ball::setCurrTile(CMMPointer<tile> newTile) {
	currTile = newTile;
}

void ball::draw() {
	//draw the ball -- add radius to position in normal direction so it isn't in the ground
	if (!inCup)
		renderManager::Instance()->drawSphere(radius, normal, position + normal*radius, color);
}


void ball::doSimulation() {

	if (inCup) return;

	//update timer
	double currTime = timer.getElapsedTimeInSec();
	double timeElapsed = currTime - lastFrameTime;
	lastFrameTime = currTime;
	
	//update velocity with any new forces
	while ( !newForces.empty() ) {
		velocity += newForces.front();
		newForces.pop();
	}

	//check to see if the ball's velocity is greater than the friction
	if (velocity.magnitude() >= currTile->getFrictionMagnitude() || currTile->isSloped()) {
		active = true;
	} 
	//if velocity magnitude is under the friction threshold, don't do anything
	//reset the ball's parameters and make it inactive
	else {
		velocity = Vec3f(0,0,0);
		active = false;
		return;
	}
	
	//apply friction & gravity to the velocity
	velocity += currTile->getGravityDirection() * GRAVITY_MAGNITUDE;
	velocity += pM->calcFrictionVector(currTile->getFrictionMagnitude(), velocity);

	//handle any collisions this step
	if ( !handleCollisions(timeElapsed) ) {
		//end position this frame if there wasn't a collision
		position = pM->calcPosition(position, velocity, timeElapsed);
	}
	
	//ensure ball sticks to tile
	Vec3f tileNorm = currTile->getNormal();
	position[1] = (-currTile->getDist() - position[0]*tileNorm[0] - position[2]*tileNorm[2])/tileNorm[1];

	//If this tile has a cup, check for collison with the cup
	if (currTile->hasCup()) checkCupCollision();
}

bool ball::handleCollisions(double timeElapsed) {

	//would eventually update this to check against all simulated objects on the tile
	vector<CMMPointer<Plane>> planes = currTile->getEdgePlanes();
	bool collisionHandled = false;

	for (unsigned int i = 0; i < planes.size() && !collisionHandled; i++) {

		//get the time until the ball will collide with this plane
		float collTime = pM->calcPointPlaneIntersectTime(position, velocity, planes[i]->getNormal(), planes[i]->getDist());
		
		if (collTime >= 0 && collTime <= timeElapsed) {
			Vec3f collPos = pM->calcPosition(position, velocity, collTime);

			//set the ball's position to the point of collision -- not sure if this is desirable
			position = collPos;

			//updates velocity after collision
			handleEdgePlaneCollision(i, planes[i]);

			//handle the rest of the ball's movement this step
			double timeRemaining = timeElapsed - collTime; 
			position = pM->calcPosition(position, velocity, timeRemaining);

			//break out of the for loop
			collisionHandled = true;
		}//collTime is >= 0
	}//potential collision objects for loop

	//Was a collision handled this step?
	return collisionHandled;

}//end handleCollisions

void ball::handleEdgePlaneCollision(int edgeIndex, CMMPointer<Plane> p) {
	
	vector<int> neighbors = currTile->getNeighbors();

	//wall collision
	if (neighbors[edgeIndex] == 0) {
		velocity = pM->calcPlaneReflectionVelocity(velocity, p->getNormal());
	}
	//new tile entry
	else resolveNewTileEntry(neighbors[edgeIndex]); 
}

//if ball leaves the level, move it back a step and recheck collision
void ball::checkMissedCollision(CMMPointer<tile> tileByPosition) {

	//if the tile is invalid or ball isn't on this tile
	if (!tileByPosition.isValid() || tileByPosition!= currTile){
		//move position back a frame
		position = lastGoodPosition;
		currTile = lastGoodTile;
		active = false;
	}
	else {
		lastGoodPosition = position;
		lastGoodTile = tileByPosition;
	}
}
		

void ball::resolveNewTileEntry(int newTileId) {

	//the newTile to be entered
	CMMPointer<tile> newTile = (*tileMap)[newTileId]; 

	//compensate for change in normal by setting new velocity, otherwise velocity remains constant
	if ( currTile->getNormal() != newTile->getNormal() ) {
		
		//from formula in CMPS164 assignment description
		Vec3f currDirection = velocity.normalize();
		Vec3f perpPlane = currDirection.cross(WORLD_UP_VECTOR);
		//needed to invert this for some reason
		Vec3f newDirection = - perpPlane.cross( newTile->getNormal() ).normalize();
		velocity = newDirection * velocity.magnitude();
	} 
	//update currTile
	currTile = newTile;
}


void ball::checkCupCollision() {
	Vec3f cupRay = cupPos - position;
	if (cupRay.magnitude() <= CUP_RADIUS) inCup = true;
}

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position[0] <<
		", " << position[1] << ", " << position[2] << endl;
	return ss.str();
}

void ball::setPlayerId(int id){
	ball::playerId = id;
}

int ball::getPlayerId(){
	return ball::playerId;
}
