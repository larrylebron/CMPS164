#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include <Windows.h>
#include <map>
#include <sstream>
#include "tile.h"
#include "tee.h"
#include "ball.h"
#include "cup.h"
#include "Wall.h"
#include "Logger.h"
#include "util.h"

class level : public IMMObject
{
public:
	level();
	~level();
	void addTile(int pId, CMMPointer<tile>* pTile);
	void addBall(int pId, CMMPointer<ball>* pBall);
	void addTee(int pId, CMMPointer<tee>* pTee);
	void addCup(int pId, CMMPointer<cup>* pCup);

	//get the maps
	std::map<int, CMMPointer<tile>> getTiles();
	std::map<int, CMMPointer<ball>> getBalls();
	std::map<int, CMMPointer<tee>> getTees();
	std::map<int, CMMPointer<cup>> getCups();

	//Check the level to confirm it's valid
	bool checkLevel();

	string toString(); //print info about the tiles in the level
	AUTO_SIZE;
private:
	//The tiles comprising the level, indexed by their ids
	std::map<int, CMMPointer<tile>> tiles;
	std::map<int, CMMPointer<tee>> tees; //the tee
	std::map<int, CMMPointer<cup>> cups; //the cups
	std::map<int, CMMPointer<ball>> balls; //the balls
	int numTiles;
	int numBalls;
	int numTees;
	int numCups;

};

#endif
