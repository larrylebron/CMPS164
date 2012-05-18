#ifndef BALL_H
#define BALL_H

#pragma once
#include "util.h"
#include "Drawable.h"
#include "GameObject.h"
#include "Simulated.h"
class ball :
	public GameObject, Drawable, Simulated
{
public:
	ball::ball(int pID, Vec3f pPosition, Vec3f color, float pRadius);
	~ball();
	Vec3f getPosition();
	void setVelocity(Vec3f pVelocity);
	Vec3f getVelocity();
	void draw();
	void doSimulation(float currTime); //handle ball movement at currTime in seconds
	void setCurrSurfaceNormal(Vec3f pNormal); //set the ball's current surface normal
	string toString();	
	AUTO_SIZE;
private:
	Vec3f position;
	float radius;
	Vec3f currSurfaceNormal; //the normal for the ball's current surface
};

#endif