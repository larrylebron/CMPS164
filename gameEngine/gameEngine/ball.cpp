#include "ball.h"


//ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius, CMMPointer<level> pCurrLev)
ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius)
	: GameObject(pID), Drawable(pColor)
{
	position = pPosition;
	radius = pRadius;

	currTile = 0;
	lastTileId = 0;
	tileMap = 0;

	enteredNewTileTime = 0;
	delayTileReentry = false;
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
	currTileNormal = currTile->getNormal();
	currTile->toggleHighlight();
	lastTileId = currTile->getID();
}

void ball::draw() {
	//draw the ball -- add radius to position in normal direction so it isn't in the ground
	if (!inCup)renderManager::Instance()->drawSphere(radius, normal, position + normal*radius, color);
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
	if (velocity.magnitude() >= currTile->getFrictionMagnitude() ) {
		//ball has just been activated -- check for collisions this frame
		if (!active) {
			Vec3f endPos = pM->calcPosition(position, velocity, timeElapsed);//end position this frame
			checkFutureCollision(position, endPos);//check for a collision one frame ahead
		}
		active = true;
	} 
	//if velocity magnitude is under the friction threshold, don't do anything
	//reset the ball's parameters and make it inactive
	else if (active) {
		velocity = Vec3f(0,0,0);
		active = false;
		resolveCollision = false;
		//cout << currTile->toString();
		return;
	}
	
	//resolve a collision detected last frame
	if (resolveCollision) {
		velocity = postCollisionVelocity;
		resolveCollision = false;
	}

	//apply friction & gravity to the velocity
	velocity += calcFriction();
	velocity += currTile->getGravityDirection() * GRAVITY_MAGNITUDE;

	Vec3f endPos = pM->calcPosition(position, velocity, timeElapsed);//end position this frame
	Vec3f futurePos = pM->calcPosition(endPos, velocity, FRAME_TIME);//end position next frame
	checkFutureCollision(endPos, futurePos);//check for a collision one frame ahead
	
	//ensure ball sticks to tile
	Vec3f tileNorm = currTile->getNormal();
	endPos[1] = (-currTile->getDist() - endPos[0]*tileNorm[0] - endPos[2]*tileNorm[2])/tileNorm[1];

	//set position by current velocity
	position = endPos;

	//If this tile has a cup, check for collison with the cup
	if (currTile->hasCup()) checkCupCollision();
}

void ball::checkCupCollision() {
	Vec3f cupRay = cupPos - position;
	if (cupRay.magnitude() <= CUP_RADIUS) inCup = true;
}

void ball::checkFutureCollision(Vec3f startPos, Vec3f endPos) {

	vector<CMMPointer<Plane>> planes = currTile->getEdgePlanes();

	for (unsigned int i = 0; i < planes.size() && !resolveCollision; i++) {

		//Was calculating the intersect with an offset plane so it's the side of the ball -- but that might have been causing issues?
		//Vec3f* intPoint = pM->calcSpherePlaneIntersect(radius, startPos, endPos, planes[i]->getNormal(), planes[i]->getVertices() );
		Vec3f* intPoint = pM->calcPointPlaneIntersect(startPos, endPos, planes[i]->getNormal(), planes[i]->getDist() );
	
		//collides with plane i
		if (intPoint) {
			resolveCollision = true; //resolve this collision next frame
			vector<int> neighbors = currTile->getNeighbors();

			//wall collision
			if (neighbors[i] == 0) {
				postCollisionVelocity = pM->calcPlaneReflectionVelocity(velocity, planes[i]->getNormal());
				}
			//new tile entry
			else {
				//if trying to reenter last tile, check to see if time is up
				if (delayTileReentry && lastTileId == neighbors[i]) {
					if ( (timer.getElapsedTime() - enteredNewTileTime) > TILE_REENTRY_DELAY_TIME) 
						delayTileReentry = false; //delay time is up
				} else {
					resolveNewTileEntry(neighbors[i]); 
				}//resolve tile else
			}//new tile else
		}//collision detected 
		delete intPoint; //deallocate the memory
	}//checking for collisions against planes
}//doSimulation

void ball::resolveNewTileEntry(int newTileId) {

	//set reentry delay params
	enteredNewTileTime = timer.getElapsedTime();
	delayTileReentry = true;
	lastTileId = currTile->getID();

	currTile->toggleHighlight();//turn off tile's highlight
	currTile = (*tileMap)[newTileId]; //set the new tile as currTile
	currTile->toggleHighlight(); //turn on new tile's highlight

	Vec3f newNormal = currTile->getNormal();

	//compensate for change in normal by setting new velocity
	if (currTileNormal != newNormal) {
		
		Vec3f perp = velocity.cross(WORLD_UP_VECTOR);
		Vec3f newDirection = (perp.cross(newNormal)).normalize();
		postCollisionVelocity = -newDirection;
	} else {
		postCollisionVelocity = velocity;
	}
	currTileNormal = newNormal;
}

Vec3f ball::calcFriction() {
	if (velocity.magnitude() == 0) return Vec3f(0,0,0);//if ball is stationary
	return velocity.normalize() * -1 * currTile->getFrictionMagnitude();
}

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position[0] <<
		", " << position[1] << ", " << position[2] << endl;
	return ss.str();
}
