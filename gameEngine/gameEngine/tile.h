//A class to manage tiles

#pragma once

#include "util.h"
#include "externalLibs/vec3f.h"
class tile :
	public IMMObject
{
public:
	tile();

	/*
		initialize with parameters
		pID: tile ID
		pNumVerts: number of vertices
		pNumEdges: number of edges for tracking neighbors
		pVertices: the tile's vertices
		pNeighbors: the ids of the neighboring tiles, in edge order lower, right, upper, left
		pNormal: the face normal of the tile
	*/
	tile(int pID, int pNumVerts, int pNumEdges, Vec3f* pVertices, int* pNeighbors, Vec3f pNormal);
	Vec3f getNormal();//return a copy of the normal vector
	Vec3f* getVertices();//return a copy of the array of vertices
	int getNumVertices(); //return a copy of numVertices
	int getNumEdges();
	int* getNeighbors(); //return a copy of the array of neighbors
	int getId();
	~tile();

	//print info about this tile
	void printInfo();
	AUTO_SIZE;
private:
	Vec3f* vertices;
	int* neighbors;
	int id;
	int numVertices;
	int numEdges;
	Vec3f normal;
};

