#ifndef BALL_H
#define BALL_H

#pragma once
#include <map>
#include <list>
#include <float.h>
#include "util.h"
#include "Drawable.h"
#include "GameObject.h"
#include "Simulated.h"
#include "tile.h"

class ball :
	public GameObject, public Drawable, public Simulated
{
public:
	ball::ball(int pID, Vec3f pPosition, Vec3f pColor, float pRadius);
	~ball();
	Vec3f getPosition();
	inline float getRadius() {return radius;}
	void setPosition(Vec3f pos);
	void draw();
	void doSimulation(); //handle ball movement at currTime in seconds
	void ball::setCurrTile(CMMPointer<tile> newTile); //set the ball's current tile
	void setTileMap(std::map<int, CMMPointer<tile>>*);//set the tile's map of level tiles for neighbor reference
	void setCupPos(Vec3f pCupPos); //set the ball's reference to the cup position for error checking
	bool isInCup(); //returns true if the ball's in the cup

	void setPlayerId(int id);
	int getPlayerId();

	/*
	if currTile doesn't match the tile detected by position, this function will step it back
	and retry the missed collision until it is resolved
	*/
	void checkMissedCollision(CMMPointer<tile> tileByPosition);
	string toString();	
	AUTO_SIZE;
private:
	Vec3f position;
	Vec3f lastGoodPosition; //last good position -- used if an error occurred
	CMMPointer<tile> lastGoodTile; //last good tile -- used if an error occurred
	float radius;
	CMMPointer<tile> currTile; //the ball's current tile
	Vec3f cupPos;//the position of the cup in the level
	bool inCup; //true if the ball's in the cup
	std::map<int, CMMPointer<tile>>* tileMap; //pointer to the map of tiles in the current level, referenced by tile ID
	int playerId;

	//set the new rolling velocity upon tile entry
	void resolveNewTileEntry(int newTileId);
	
	//returns true if the ball has collided with the cup
	void checkCupCollision();

	/*Returns true if there was a collision during timeElapsed
	If there is a collision during this time, updates velocity, tile and position
	*/
	bool checkEdgeCollisions(double timeElapsed);

	//handle a collision with a tile edge plane p
	void handleEdgePlaneCollision(int edgeIndex, CMMPointer<Plane> p);

	void handleBallCollisions();
};

#endif