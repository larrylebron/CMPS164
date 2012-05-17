#ifndef BALL_H
#define BALL_H

#pragma once
#include "util.h"
#include <vector>
#include "externalLibs\vec3f.h"
#include "tile.h"
class ball :
	public IMMObject
{
public:
	ball::ball(int pID, Vec3f pPosition);
	~ball();
	Vec3f getPosition();
	Vec3f getVelocity();
	string toString();
	void draw();
	AUTO_SIZE;
private:
	int id;
	Vec3f position;
	Vec3f velocity;
};

#endif