#ifndef WALL_H
#define WALL_H

#pragma once
#include "util.h"
#include "Drawable.h"
#include "GameObject.h"

class Wall :
	public GameObject, Drawable
{
public:
	//initialize with start, end, height and color
	Wall::Wall(Vec3f pStart, Vec3f pEnd, float pHeight, Vec3f pColor);
	~Wall();
	void draw();//draw the wall
	string toString();
	AUTO_SIZE;
private:
	static int wallIdCount;//counter for wall ids
	Vec3f calcNormal();
	std::vector<Vec3f> vertices; //the vertices comprising this wall
	float height; //height of the wall
};

#endif