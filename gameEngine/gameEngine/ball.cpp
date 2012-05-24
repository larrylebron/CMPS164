#include "ball.h"


//ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius, CMMPointer<level> pCurrLev)
ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius)
	: GameObject(pID), Drawable(pColor)
{
	velocity = Vec3f(.6, 0,-.05);
	position = pPosition;
	radius = pRadius;

	currTile = 0;
	lastTileId = 0;
	tileMap = 0;

	enteredNewTileTime = 0;
	delayTileReentry = false;
}


ball::~ball(void)
{
}

Vec3f ball::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

void ball::setTileMap(std::map<int, CMMPointer<tile>>* newTileMap) {
	tileMap = newTileMap;
}


void ball::setCurrTile(CMMPointer<tile> newTile) {
	currTile = newTile;
	currTileNormal = currTile->getNormal();
	currTile->toggleHighlight();
	lastTileId = currTile->getID();
}

void ball::draw() {
	//draw the ball -- add radius to position in normal direction so it isn't in the ground
	renderManager::Instance()->drawSphere(radius, normal, position + normal*radius, color);
}

void ball::doSimulation() {
	
	//update timer
	double currTime = timer.getElapsedTimeInSec();
	double timeElapsed = currTime - lastFrameTime;
	lastFrameTime = currTime;

	//resolve a collision detected last frame
	if (resolveCollision) {
		cout << "resolve collision";
		velocity = postCollisionVelocity;
		resolveCollision = false;
	}

	Vec3f endPos = pM->calcPosition(position, velocity, timeElapsed);//end position this frame
	Vec3f futurePos = pM->calcPosition(endPos, velocity, FRAME_TIME);//end position next frame
	checkFutureCollision(endPos, futurePos);//check for a collision one frame ahead
	
	//set position by current velocity
	position = endPos;
}

void ball::checkFutureCollision(Vec3f startPos, Vec3f endPos) {

	vector<CMMPointer<Plane>> planes = currTile->getEdgePlanes();

	for (int i = 0, collision = false; i < planes.size() && !resolveCollision; i++) {

		Vec3f* intPoint = pM->calcSpherePlaneIntersect(radius, startPos, endPos, planes[i]->getNormal(), planes[i]->getVertices() );
	
		//collides with plane i
		if (intPoint) {
			resolveCollision = true; //resolve this collision next frame
			vector<int> neighbors = currTile->getNeighbors();

			//wall collision
			if (neighbors[i] == 0) {
				postCollisionVelocity = pM->calcPlaneReflectionVelocity(velocity, planes[i]->getNormal());
				cout << "velocity now: " << velocity << "\npost coll" << postCollisionVelocity << endl;
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

string ball::toString() {
	std::stringstream ss;
	ss << "ball id: " << id << endl << "position: " << position[0] <<
		", " << position[1] << ", " << position[2] << endl;
	return ss.str();
}
