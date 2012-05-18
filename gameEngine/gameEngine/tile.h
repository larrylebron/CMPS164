//A class to manage tiles

#ifndef TILE_H
#define TILE_H
#pragma once

#include "util.h"
#include "Wall.h"
#include "Drawable.h"
#include "GameObject.h"
class tile :
	public GameObject, Drawable
{
public:
	/*
		initialize with parameters
		pID: tile ID
		pVertices: the tile's vertices
		pNeighbors: the ids of the neighboring tiles, in edge order lower, right, upper, left
		pColor: the tile's color
	*/
	tile(int pID, vector<Vec3f> pVertices, vector<int> pNeighbors, Vec3f pColor);
	Vec3f getNormal();//return a copy of the normal vector
	
	vector<Vec3f> getVertices();//return a copy of the vector of vertices
	vector<int> getNeighbors(); //return a copy of the vector of neighbors

	int getNumEdges(); //return the number of vertices in the polygon
	bool containsPoint(Vec3f p); //returns true if the tile contains point p
	~tile();
	void draw(); //draw the tile
	//print info about this tile
	string toString();
	AUTO_SIZE;
private:

	Vec3f calcNormal(); //Sets the object's normal using Newell's method
	std::vector<Vec3f> vertices; //the vertices comprising this wall
	std::vector<int> neighbors; //ids of neighboring tiles, indexed by edge number
	std::vector< CMMPointer<Wall> > walls; //the walls surrounding the tile

	void buildWalls(); //build the tile's walls
	void initBounds();//initialize the simple bounds vectors
	Vec3f minBounds; // contains the minimum x, y, z vertex values
	Vec3f maxBounds; // contains the maximum x, y, z vertex values
	Logger* log;
};

#endif