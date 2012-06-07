//A class to manage tiles

#ifndef TILE_H
#define TILE_H
#pragma once

#include <list>
#include "util.h"
#include "Wall.h"
#include "Drawable.h"
#include "GameObject.h"

class tile :
	public Plane, Drawable
{
public:
	/*
		initialize with parameters
		pID: tile ID
		pVertices: the tile's vertices
		pNeighbors: the ids of the neighboring tiles, in edge order lower, right, upper, left
		pColor: the tile's color
	*/
	tile(int pID, vector<Vec3f> pVertices, vector<int> pNeighbors, Vec3f pColor, float pFrictionMagnitude = DEFAULT_FRICTION_MAGNITUDE);
	~tile();
	void draw(); //draw the tile
	int getNumEdges(); //return the number of edges
	vector<int> getNeighbors();
	vector<CMMPointer<Plane>> getEdgePlanes();
	void toggleHighlight();//make the tile white for debugging
	float getFrictionMagnitude() {return frictionMagnitude;}
	bool hasCup();
	void setContainsCup(bool hasCup);

	 //add a ball to this tile
	void addBall(GameObject* b) {balls.push_back(b);}
	//remove the ball from this tile
	void removeBall(GameObject* b) {balls.remove(b);}
	//get the list of balls on this tile
	std::list< GameObject* > getBalls() {return balls;}
	
	//print info about this tile
	string toString();
	AUTO_SIZE;
private:
	vector<int> neighbors; //ids of neighboring tiles, indexed by edge number
	vector< CMMPointer<Plane> > edgePlanes; //the edge planes & walls used to test for ball collision
	std::list< GameObject* > balls; //vector of the balls on this tile
	void buildEdgePlanes(); //build the tile's walls and bounding planes
	bool highlighted; //tracks whether the tile is highlighted for debugging
	bool containsCup; //does the tile have a cup?
	float frictionMagnitude; //magnitude of this tile's friction
	Logger* log;
};

#endif