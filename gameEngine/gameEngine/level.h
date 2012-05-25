#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include <Windows.h>
#include <map>
#include <sstream>
#include "Drawable.h"
#include "GameObject.h"
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

	CMMPointer<ball> getBall();

	//get the positions -- currently assumes only a single ball & cup
	Vec3f getBallPos();
	Vec3f getCupPos();
	
	//Check the level to confirm it's valid
	bool checkLevel();

	//returns a pointer to the tile containing this point, or a void pointer
	CMMPointer<tile> getTileContainingPoint(Vec3f point);

	//returns true if the level is complete
	bool isComplete(); 

	//update the level and draw
	void update(); 

	//print info about the tiles in the level
	string toString(); 

	AUTO_SIZE;
private:
	void setComponentParams();//update the ball with tile info & any tiles containing cups with that info
	//The tiles comprising the level, indexed by their ids
	std::map<int, CMMPointer<tile>> tiles;
	std::map<int, CMMPointer<tee>> tees; //the tee
	std::map<int, CMMPointer<cup>> cups; //the cups
	std::map<int, CMMPointer<ball>> balls; //the balls
	int numTiles;
	int numBalls;
	int numTees;
	int numCups;
	bool complete; //true when the level is complete
};

#endif
