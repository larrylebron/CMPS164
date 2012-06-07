#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#pragma once

#include "util.h"

class PhysicsManager{
public:
   static PhysicsManager* Instance();//returns an instance of the manager

   //calculate new position given initial position, velocity and time elapsed
   Vec3f calcPosition(Vec3f position, Vec3f velocity, float timeElapsed);

   //Return the normal of the plane comprised of at least 3 vertices
   Vec3f calcPlaneNormal(vector<Vec3f> vertices);

   //Return the normalized direction vector that gravity will project onto a plane
   Vec3f calcPlaneGravityDirection(Vec3f planeNormal);

   //Return the friction vector based on the magnitude and object velocity
   Vec3f calcFrictionVector(float frictionMagnitude, Vec3f velocity);

   //Returns the time that the point intersects the plane
   float calcPointPlaneIntersectTime(Vec3f startPos, Vec3f velocity, Vec3f planeNormal, float planeDist);

   //returns true intersection point if the point collides with the plane between startPos and endPos, else returns NULL
   bool PhysicsManager::pointPlaneIntersect(Vec3f startPos, Vec3f velocity, float duration, Vec3f planeNormal, float planeDist);

   //returns true if the sphere collides with the plane starting at startPos, traveling at velocity, for duration time
   float calcSpherePlaneIntersectTime(float radius, Vec3f startPos, Vec3f velocity, Vec3f planeNormal, vector<Vec3f> planeVertices );

   //Returns the reflection velocity, given the velocity of the object hitting the plane, and the plane normal
   Vec3f calcPlaneReflectionVelocity(Vec3f velocity, Vec3f planeNormal);

   //returns true if the spheres at these positions are colliding
   bool spheresCollide(Vec3f positionA, float radiusA, Vec3f positionB, float radiusB);

private:
	PhysicsManager(); //private constructor for singleton
	static PhysicsManager* m_pInstance; //static instance of the manager

};

#endif