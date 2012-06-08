#include "level.h"


level::level()
{
	numTiles = 0;
	numBalls = 0;
	numTees = 0;
	numCups = 0;
	complete = false;
	active = false;
	par = 1;
	strokes = 1;
}

level::~level()
{

}

void level::addTile(int pId, CMMPointer<tile>* pTile) 
{
	numTiles++;
	tiles[pId] = *pTile;

}

void level::addBall(int pId, CMMPointer<ball>* pBall, bool newInit) {
	numBalls++;
	balls[pId] = *pBall;

	if (newInit)
	{
		//initialize the ball's current tile
		Vec3f ballPos = balls[pId]->getPosition();
		CMMPointer<tile> ballTile = getTileContainingPoint(ballPos);
		if (!ballTile.isValid()) Logger::Instance()->err("ball not on any tile");
		balls[pId]->setCurrTile(ballTile); 

		//set the tile reference map
		balls[pId]->setTileMap(&tiles);
		balls[pId]->setCupPos(getCupPos());
	}
}

void level::addTee(int pId, CMMPointer<tee>* pTee) {
	numTees++;
	tees[pId] = *pTee;
}

void level::addCup(int pId, CMMPointer<cup>* pCup) {
	numCups++;
	cups[pId] = *pCup;
}

CMMPointer<ball> level::getBall(int pID) {
	return balls[pID];
}

Vec3f level::getBallPos(int pId) {
	return balls[pId]->getPosition();
}

Vec3f level::getTeePos() {
	return (*tees.begin()).second->getPosition();
}

bool level::isActive() {
	return active;
}

Vec3f level::getCupPos() {
	if (cups.size() > 0)
		return (*cups.begin()).second->getPosition();
	else
		return balls[0]->getPosition();
}

bool level::isComplete() {
	return complete;
}

CMMPointer<tile> level::getTileContainingPoint(Vec3f point) {

	//check through the tiles to see if they contain the ball's position
	std::map<int, CMMPointer<tile>>::iterator it;
	for ( it=tiles.begin() ; it != tiles.end(); it++ ) {
		CMMPointer<tile> checkTile = (*it).second;
		if ( checkTile->inSimpleBounds(point) ) return checkTile; //return pointer to current tile
	}
	return CMMPointer<tile>();
}

bool level::checkLevel() {
	//check the tiles to ensure all neighbors exist
	std::map<int, CMMPointer<tile>>::iterator it;
	CMMPointer<tile> t;
	for ( it=tiles.begin(); it != tiles.end(); it++ ) {
		
		t = it->second;
		int n = t->getNumEdges();
		vector<int> neighbors = t->getNeighbors();
		
		for(int i = 0; i < n; i++) {
			if (neighbors[i] != 0 && tiles.find(neighbors[i]) == tiles.end() ) {
				std::stringstream msg;
				msg << "Neighbor " << i << " in tile id " << t->getID() << " is invalid";
				Logger::Instance()->err(msg.str());
				return false;
			}
		}//end neighbor search
	} //end all tile search

	if (numBalls < 1) {
		Logger::Instance()->err("No balls in the level");
		return false;
	} else if (numCups < 1) {
		Logger::Instance()->err("No cups in the level");
		return false;
	} else if (numTees < 1) {
		Logger::Instance()->err("No tees in the level");
		return false;
	}

	//Level is valid, so it's safe to update the ball & cup tile with relevant info
	setComponentParams();
	return true;
}

void level::setComponentParams() {
	//assumes one ball
	CMMPointer<ball> ball = (*balls.begin()).second;

	//initialize the ball's current tile
	Vec3f ballPos = ball->getPosition();
	CMMPointer<tile> ballTile = getTileContainingPoint(ballPos);
	if (!ballTile.isValid()) Logger::Instance()->err("ball not on any tile");
	ball->setCurrTile(ballTile); 

	//set the tile reference map
	ball->setTileMap(&tiles);

	//flag the tile containing the cup -- assumes only one cup
	Vec3f cupPos = getCupPos();
	CMMPointer<tile> cupTile = getTileContainingPoint(cupPos);
	if (!cupTile) Logger::Instance()->err("cup not on any tile");
	cupTile->setContainsCup(true);

	//set the ball's cup position for collision checking
	ball->setCupPos(cupPos);
}

void level::update(bool isBocce, int numTotalBalls) {
	//check to see if the ball's in the cup -- if so, mark the level complete
	if ( ((*balls.begin()).second->isInCup() && !isBocce) || (balls.size() > numTotalBalls && isBocce)) {
		complete = true;
	}

	// remove cup and tileHasCup boolean
	if (isBocce && cups.size() != 0) {
		Vec3f cupPos = getCupPos();
		CMMPointer<tile> cupTile = getTileContainingPoint(cupPos);
		cupTile->setContainsCup(false);
		cups.clear();
	}

	//Run the simulation for all balls in the level
	active = runBallSimulation();

	std::map<int, CMMPointer<tile>>::iterator it;
	for ( it=tiles.begin(); it != tiles.end(); it++ ) {
		(*it).second->draw();
	}
	
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin(); itB != balls.end(); itB++ ) {
		(*itB).second->draw();
	}
	
	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin(); itC != cups.end(); itC++ ) {
		(*itC).second->draw();
	}
}

bool level::runBallSimulation() {
	bool currentlyActive = false;

	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin(); itB != balls.end(); itB++ ) {
		CMMPointer<ball> b = (*itB).second;
		b->doSimulation(); //run the ball's simulation

		//If ball misses a collision, handle it
		Vec3f ballPos = b->getPosition();
		CMMPointer<tile> ballTile = getTileContainingPoint(ballPos);
		b->checkMissedCollision(ballTile);
		
		currentlyActive = currentlyActive || b->isActive(); //testing to see if level is active
	}

	return currentlyActive;
}


// reset ball positions & all level parameters
void level::resetLevel()
{
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin(); itB != balls.end(); itB++ ) {
		(*itB).second = 0;   
	}
	balls.clear();

	std::map<int, CMMPointer<tile>>::iterator itT;
	for ( itT=tiles.begin(); itT != tiles.end(); itT++ ) {
		(*itT).second->clearBalls();  
	}
	
	strokes = 0;
	numBalls = 0;
	int id = 0;
	CMMPointer<ball>* newBall = new CMMPointer<ball>(new ball(id, getTeePos(), BALL_COLOR, BALL_RADIUS));
	addBall(id, newBall, true);
	complete = false;
}

void level::setLevelName(string name)
{
	levelName = name;
}

string level::getLevelName()
{
	return levelName;
}

void level::setPar(int toPar)
{
	par = toPar;
}

int level::getPar()
{
	return par;
}

int level::getStrokes() {
	return strokes;
}

void level::addStroke(int num) {
	strokes += num;
}

int level::getScore(bool isBocce, int playerId)
{
	if (!isBocce)
		return par - strokes;
	else
		return 0;
}

int level::getPlayerTurn(int current)
{
	// assume 2 players
	float p1Dist = 10000;
	float p2Dist = 10000;

	int p1Balls = 0;
	int p2Balls = 0;

	// if only first red ball has been thrown, that player gets to go again
	if (balls.size() <= 1) {
		cout << "no hand over from " << current << endl;
		return current;
	}

	Vec3f mainBall = balls[0]->getPosition();

	for (int i = 1; i < balls.size(); i++) {
		Vec3f current = balls[i]->getPosition();
		float dist = (mainBall - current).magnitude();
		
		// get the ID of the current ball
		int player = balls[i]->getPlayerId();

		// if this ball the closest ball to the main ball, save it for that player
		if (player == 0) {
			p1Balls++;
			if (dist < p1Dist) {
				p1Dist = dist;
			}
		} else if (player == 1) {
			p2Balls++;
			if (dist < p2Dist) {
				p2Dist = dist;
			}
		}
	}

	if (p1Balls == 4) {
		return 1;
	} else if (p2Balls == 4) {
		return 0;
	}

	cout << current << " " << p1Dist << " " << p2Dist << endl;

	int result = 0;

	// if the distance the same, hand over turn
	if (p1Dist == p2Dist)
	{
			result = (current == 0) ? 1 : 0;
	// if p1 is closer, p2 goes
	} else if (p1Dist > p2Dist) {
		cout << "boo" << endl;
		result = 0;
	} else {
		result = 1;
	}

	cout << result << endl;
	return result;

}

string level::toString() {
	return "level info";
	/*
	cout << "Level Info: \n";
	cout << "Number of tiles: " << numTiles << endl;
	std::map<int, CMMPointer<tile>>::iterator it;
	for ( it=tiles.begin(); it != tiles.end(); it++ ) {
		//print info for the tile pointed at by the CMMPointer
		(*it).second->printInfo();
	}
	
	cout << "Number of balls: " << numBalls << endl;
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin(); itB != balls.end(); itB++ ) {
		(*itB).second->printInfo();
	}
	
	cout << "Number of tees: " << numTees << endl;
	std::map<int, CMMPointer<tee>>::iterator itTee;
	for ( itTee=tees.begin(); itTee != tees.end(); itTee++ ) {
		(*itTee).second->printInfo();
	}

	cout << "Number of cups: " << numCups << endl;
	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin(); itC != cups.end(); itC++ ) {
		(*itC).second->printInfo();
	}
	*/
}
