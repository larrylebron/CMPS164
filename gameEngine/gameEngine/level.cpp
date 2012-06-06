#include "level.h"


level::level()
{
	numTiles = 0;
	numBalls = 0;
	numTees = 0;
	numCups = 0;
	complete = false;
	par = 1;
	strokes = 0;
}

level::~level()
{
	/*
	std::map<int, CMMPointer<tile>>::iterator itT;
	for ( itT=tiles.begin() ; itT != tiles.end(); itT++ ) {
		(*itT).second = 0;
	}
	
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin() ; itB!= balls.end(); itB++ ) {
		(*itB).second = 0;
	}

	std::map<int, CMMPointer<tee>>::iterator itTee;
	for ( itTee=tees.begin() ; itTee != tees.end(); itTee++ ) {
		(*itTee).second = 0;
	}

	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin() ; itC != cups.end(); itC++ ) {
		(*itC).second = 0;
	}*/
}

void level::addTile(int pId, CMMPointer<tile>* pTile) 
{
	numTiles++;
	tiles[pId] = *pTile;

}

void level::addBall(int pId, CMMPointer<ball>* pBall) {
	numBalls++;
	balls[pId] = *pBall;
}

void level::addTee(int pId, CMMPointer<tee>* pTee) {
	numTees++;
	tees[pId] = *pTee;
}

void level::addCup(int pId, CMMPointer<cup>* pCup) {
	numCups++;
	cups[pId] = *pCup;
}

CMMPointer<ball> level::getBall() {
	return (*balls.begin()).second;
}

Vec3f level::getBallPos() {
	return (*balls.begin()).second->getPosition();
}

void level::resetBallPos() {
	Vec3f teePos = (*tees.begin()).second->getPosition();
	(*balls.begin()).second->setPosition(teePos);
}

Vec3f level::getCupPos() {
	return (*cups.begin()).second->getPosition();
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

void level::update() {

	//check to see if the ball's in the cup -- if so, mark the level complete
	if ( (*balls.begin()).second->isInCup() ) {
		complete = true;
	}

	//If ball goes out of bounds handle it
	CMMPointer<ball> b = (*balls.begin()).second;
	Vec3f ballPos = b->getPosition();
	CMMPointer<tile> ballTile = getTileContainingPoint(ballPos);
	b->checkMissedCollision(ballTile);

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


// reset ball positions to tee position
void level::resetLevel()
{
	resetBallPos();
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

int level::getScore()
{
	return par - strokes;
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
