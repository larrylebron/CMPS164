#ifndef BALL_H
#define BALL_H

#pragma once
#include "util.h"
#include "Drawable.h"
#include "GameObject.h"
#include "Simulated.h"
#include "tile.h"

//time to delay after a collision to avoid double-triggering
static const double TILE_REENTRY_DELAY_TIME = .2;

class ball :
	public GameObject, public Drawable, public Simulated
{
public:
	ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius);
	~ball();
	Vec3f getPosition();
	void draw();
	void doSimulation(); //handle ball movement at currTime in seconds
	void ball::setCurrTile(CMMPointer<tile> newTile); //set the ball's current tile
	void setTileMap(std::map<int, CMMPointer<tile>>*);//set the tile's map of level tiles for neighbor reference
	void setCupPos(Vec3f pCupPos); //set the ball's reference to the cup position for error checking
	bool isInCup(); //returns true if the ball's in the cup
	string toString();	
	AUTO_SIZE;
private:
	Vec3f position;
	float radius;
	Vec3f currTileNormal; //the normal for the ball's current surface
	CMMPointer<tile> currTile; //the ball's current tile
	int lastTileId; //id of the last tile the ball was on
	Vec3f cupPos;//the position of the cup in the level
	bool inCup; //true if the ball's in the cup
	std::map<int, CMMPointer<tile>>* tileMap; //pointer to the map of tiles in the current level, referenced by tile ID
	
	void resolveNewTileEntry(int newTileId);//set the new rolling velocity upon tile entry
	
	//calculate the current friction being applied against the ball
	Vec3f calcFriction();

	//returns true if the ball has collided with the cup
	void checkCupCollision();

	//check for collision between these two positions
	void checkFutureCollision(Vec3f startPos, Vec3f endPos);

	//the velocity to use when resolving a future collision
	Vec3f postCollisionVelocity;

	//time the last tile was entered
	double enteredNewTileTime; 

	//flag to ignore collisons that would put the ball back on its last tile
	bool delayTileReentry;
};

#endif