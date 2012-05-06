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
}

void level::addTee(int pId, CMMPointer<tee>* pTee) {
	numTees++;
	tees[pId] = *pTee;
}

void level::addCup(int pId, CMMPointer<cup>* pCup) {
	numCups++;
	cups[pId] = *pCup;
}

std::map<int, CMMPointer<tile>> level::getTiles() {
	std::map<int, CMMPointer<tile>>tilesCopy = tiles;
	return tilesCopy;
}

std::map<int, CMMPointer<tee>> level::getTees() {
	std::map<int, CMMPointer<tee>>teesCopy = tees;
	return teesCopy;
}

std::map<int, CMMPointer<ball>> level::getBalls() {
	std::map<int, CMMPointer<ball>>ballsCopy = balls;
	return ballsCopy;
}

std::map<int, CMMPointer<cup>> level::getCups() {
	std::map<int, CMMPointer<cup>>cupsCopy = cups;
	return cupsCopy;
}

bool level::checkLevel() {
	//check the tiles to ensure all neighbors exist
	std::map<int, CMMPointer<tile>>::iterator it;
	CMMPointer<tile> t;
	for ( it=tiles.begin(); it != tiles.end(); it++ ) {
		
		t = it->second;
		int n = t->getNumEdges();
		int* neighbors = t->getNeighbors();
		
		for(int i = 0; i < n; i++) {
			if (neighbors[i] != 0 && tiles.find(neighbors[i]) == tiles.end() ) {
				std::stringstream msg;
				msg << "Neighbor " << i << " in tile id" << t->getId() << " is invalid";
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
void level::printInfo() {
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
}
