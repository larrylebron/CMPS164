/**
An interface for drawable gameObjects
*/
#pragma once
#include "renderManager.h"
#include "util.h"

class Drawable
{
public:

	Drawable(Vec3f pColor) {color = pColor;} //initializes default color & normal
	virtual ~Drawable() {} //virtual destructor
	virtual void draw() = 0; //draw the object

protected:
	Vec3f color; //color of the drawable object
};

