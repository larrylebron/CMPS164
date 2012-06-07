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
	void addBall(int pId, CMMPointer<ball>* pBall, bool newInit = false);
	void addTee(int pId, CMMPointer<tee>* pTee);
	void addCup(int pId, CMMPointer<cup>* pCup);

	CMMPointer<ball> getBall(int pId = 0);

	//get the positions -- returns first ball by defualt
	Vec3f getBallPos(int pId = 0);
	void resetBallPos();
	Vec3f getCupPos();
	Vec3f getTeePos();

	 //returns true if the level is actively simulating
	bool isActive();
	
	//Check the level to confirm it's valid
	bool checkLevel();

	//returns a pointer to the tile containing this point, or a void pointer
	CMMPointer<tile> getTileContainingPoint(Vec3f point);

	//returns true if the level is complete
	bool isComplete(); 

	//update the level and draw
	void update(bool isBocce = false, int numTotalBalls = 9); 

	// reset ball positions to tee position
	void resetLevel();

	void setLevelName(string name);
	string getLevelName();

	void setPar(int toPar);
	int getPar();
	int getStrokes();
	void addStroke(int num = 1);
	int getScore(bool isBocce = false, int playerId = 0);
	int getPlayerTurn();
	//print info about the tiles in the level
	string toString(); 

	AUTO_SIZE;
private:
	void setComponentParams();//update the ball with tile info & any tiles containing cups with that info
	
	//simulate the ball movements -- returns true while at least one ball is active
	bool runBallSimulation();

	//The tiles comprising the level, indexed by their ids
	std::map<int, CMMPointer<tile>> tiles;
	std::map<int, CMMPointer<tee>> tees; //the tee
	std::map<int, CMMPointer<cup>> cups; //the cups
	std::map<int, CMMPointer<ball>> balls; //the balls
	int numTiles;
	int numBalls;
	int numTees;
	int numCups;
	int par;
	int strokes;
	bool active; //true when level contains moving objects
	bool complete; //true when the level is complete
	string levelName;
};

#endif
