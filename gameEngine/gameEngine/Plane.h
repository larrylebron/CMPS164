#ifndef PLANE_H
#define PLANE_H

#pragma once
#include "util.h"
#include "GameObject.h"
#include "PhysicsManager.h"

class Plane :
	public GameObject
{
public:
	//Plane(vector<Vec3f> pVertices);
	Plane(vector<Vec3f> pVertices, int pId = ++planeIdCount );
	~Plane();
	//returns true if the p is within the plane's simple bounds
	bool inSimpleBounds(Vec3f p);

	//returns true if the plane contains point p
	bool preciseContainsPoint(Vec3f p);

	//return the vector of plane vertices
	vector<Vec3f> getVertices();
	float getDist();
	Vec3f getGravityDirection() {return gravityDirection;}
	bool isSloped() {return sloped;}
	string toString();
	AUTO_SIZE;
protected:
	static int planeIdCount;//counter for plane id
	vector<Vec3f> vertices; //the vertices comprising this plane
	float dist; //dist value from origin for any point on the plane according to Ax + By + Cz + d = 0
	Vec3f gravityDirection; //the direction that gravity will project an object down the plane if it's sloped
	bool sloped; //is the plane sloped?
private:
	void initBounds();//initialize the simple bounds vectors
	Vec3f minBounds; // contains the minimum x, y, z vertex values
	Vec3f maxBounds; // contains the maximum x, y, z vertex values

};

#endif