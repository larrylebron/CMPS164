#ifndef WALL_H
#define WALL_H

#pragma once
#include <vector>
//#include "renderManager.h" //this is causing compile errors for some reason
#include "externalLibs\mmanager.h"
#include "externalLibs\vec3f.h"

class Wall :
	public IMMObject
{
public:
	//initialize with start, end, height and color
	Wall::Wall(Vec3f pStart, Vec3f pEnd, float pHeight, Vec3f pColor);
	~Wall();
	void draw();//draw the wall
	string toString();
	AUTO_SIZE;
private:
	int id;
	static int numWalls;
	void initNormal();
	std::vector<Vec3f> vertices; //the vertices comprising this wall
	Vec3f normal; //the normal of the wall
	Vec3f color; //the color of the wall
	float height; //height of the wall
};

#endif