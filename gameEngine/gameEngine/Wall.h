#ifndef WALL_H
#define WALL_H

#pragma once
#include "util.h"
#include "Drawable.h"
#include "Plane.h"

class Wall :
	public Plane, Drawable
{
public:
	//initialize with start, end, height and color
	Wall(vector<Vec3f> pVertices, Vec3f pColor);
	~Wall();
	void draw();//draw the wall
	string toString();
	AUTO_SIZE;
private:
};

#endif