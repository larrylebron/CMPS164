#include "level.h"


level::level()
{
	numTiles = 0;
	numBalls = 0;
	numTees = 0;
	numCups = 0;
}

level::~level()
{
	std::map<int, CMMPointer<tile>>::iterator itT;
	for ( itT=tiles.begin() ; itT != tiles.end(); itT++ ) {
		delete (*itT).second;
	}
	
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin() ; itB!= balls.end(); itB++ ) {
		delete (*itB).second;
	}

	std::map<int, CMMPointer<tee>>::iterator itTee;
	for ( itTee=tees.begin() ; itTee != tees.end(); itTee++ ) {
		delete (*itTee).second;
	}

	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin() ; itC != cups.end(); itC++ ) {
		delete (*itC).second;
	}
}

void level::addTile(int pId, CMMPointer<tile>* pTile) 
{
	numTiles++;
	tiles[pId] = *pTile;

}

void level::addBall(int pId, CMMPointer<ball>* pBall) {
	numBalls++;
	balls[pId] = *pBall;
	balls[pId]->setCurrTile(getCurrTile()); //initialize the ball's current tile
	//set the tile reference map
	balls[pId]->setTileMap(&tiles);
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

Vec3f level::getCupPos() {
	return (*cups.begin()).second->getPosition();
}


CMMPointer<tile> level::getCurrTile() {
	//get the ball's position -- assumes only one ball
	Vec3f ballPos = getBallPos();

	//check through the tiles to see if they contain the ball's position
	std::map<int, CMMPointer<tile>>::iterator it;
	for ( it=tiles.begin() ; it != tiles.end(); it++ ) {
		CMMPointer<tile> checkTile = (*it).second;
		if ( checkTile->containsPoint(ballPos) ) return checkTile; //return pointer to current tile
	}

	Logger::Instance()->err("Ball not on any tile");
	return CMMPointer<tile>(); //return a void pointer
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
	return true;
}

void level::update() {
	std::map<int, CMMPointer<tile>>::iterator it;
	for ( it=tiles.begin(); it != tiles.end(); it++ ) {
		(*it).second->draw();
	}
	
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin(); itB != balls.end(); itB++ ) {
		(*itB).second->draw();
		//(*itB).second->setCurrTile(getCurrTile());//update the ball's current tile -- only needed at init?

	}
	
	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin(); itC != cups.end(); itC++ ) {
		(*itC).second->draw();
	}
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
