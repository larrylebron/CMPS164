//A class to manage tiles

#ifndef TILE_H
#define TILE_H
#pragma once
#include  <vector>
#include "util.h"
#include "Logger.h"
#include "Wall.h"
#include "externalLibs/vec3f.h"
class tile :
	public IMMObject
{
public:
	/*
		initialize with parameters
		pID: tile ID
		pNumVerts: number of vertices
		pNumEdges: number of edges for tracking neighbors
		pVertices: the tile's vertices
		pNeighbors: the ids of the neighboring tiles, in edge order lower, right, upper, left
		pNormal: the face normal of the tile
	*/
	tile(int pID, int pNumVerts, int pNumEdges, Vec3f* pVertices, int* pNeighbors);
	Vec3f getNormal();//return a copy of the normal vector
	Vec3f* getVertices();//return a copy of the array of vertices
	int getNumVertices(); //return a copy of numVertices
	int getNumEdges();
	int* getNeighbors(); //return a copy of the array of neighbors
	int getId();
	bool containsPoint(Vec3f p); //returns true if the tile contains point p
	~tile();

	//print info about this tile
	string toString();
	AUTO_SIZE;
private:
	void initNormal();//Sets the object's normal using Newell's method
	Vec3f* vertices;
	int* neighbors;
	int id;
	int numVertices;
	int numEdges;
	std::vector< CMMPointer<Wall> > walls; //the walls surrounding the tile
	Vec3f normal;

	void initBounds();//initialize the simple bounds vectors
	Vec3f minBounds; // contains the minimum x, y, z vertex values
	Vec3f maxBounds; // contains the maximum x, y, z vertex values
	Logger* log;
};

#endif