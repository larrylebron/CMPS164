#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#pragma once

#include "util.h"

class PhysicsManager{
public:
   static PhysicsManager* Instance();//returns an instance of the manager
   
   //Pass in a list of forces, and return the result
   Vec3f calcForces();

   //calculate new position given initial position, velocity and time elapsed
   Vec3f calcPosition(Vec3f position, Vec3f velocity, float timeElapsed);

   //Return the normal of the plane comprised of at least 3 vertices
   Vec3f calcPlaneNormal(vector<Vec3f> vertices);

   //Return the normalized direction vector that gravity will project onto a plane
   Vec3f calcPlaneGravityDirection(Vec3f planeNormal);

   //Returns the time that the point intersects the plane
   float calcPointPlaneIntersectTime(Vec3f startPos, Vec3f endPos, Vec3f planeNormal, float planeDist);

   //returns the intersection point if the point collides with the plane between startPos and endPos, else returns NULL
   Vec3f* PhysicsManager::calcPointPlaneIntersect(Vec3f startPos, Vec3f endPos, Vec3f planeNormal, float planeDist);

   //returns tthe intersection point if the sphere collides with the plane between these twosphere-center positions, else returns NULL
   Vec3f* calcSpherePlaneIntersect(float radius, Vec3f startPos, Vec3f endPos, Vec3f planeNormal, vector<Vec3f> planeVertices );

   //Returns the reflection velocity, given the velocity of the object hitting the plane, and the plane normal
   Vec3f calcPlaneReflectionVelocity(Vec3f velocity, Vec3f planeNormal);
private:
	PhysicsManager(); //private constructor for singleton
	static PhysicsManager* m_pInstance; //static instance of the manager

};

#endif