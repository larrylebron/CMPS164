#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::m_pInstance = 0;  

PhysicsManager::PhysicsManager() {
}

/** This function is called to create an instance of the class. 
    Calling the constructor publicly is not allowed. The constructor 
    is private and is only called by this Instance function.
*/  
PhysicsManager* PhysicsManager::Instance()
{
   if (!m_pInstance) {   // Only allow one instance of class to be generated.
      m_pInstance = new PhysicsManager;
   }

   return m_pInstance;
}


Vec3f PhysicsManager::calcForces() {
	Vec3f result;
	
	//iterate through forces --might need to pass in velocity?

	//The resultant force
	return result;
}

Vec3f PhysicsManager::calcPosition(Vec3f position, Vec3f velocity, float timeElapsed) {
	return position + velocity * timeElapsed; //closed form projectile equation of motion
}

Vec3f PhysicsManager::calcPlaneNormal(vector<Vec3f> vertices) {
	
	if (vertices.size() < 3) {
		Logger::Instance()->err("Trying to calculate plane normal with fewer than 3 vertices");
		return WORLD_UP_VECTOR;
	}
	Vec3f a = vertices[0];
	Vec3f b = vertices[1];
	Vec3f c = vertices[2];
	
	Vec3f norm = (b - a).cross( (c - a) );
	return norm.normalize();
}

Vec3f PhysicsManager::calcPlaneGravityDirection(Vec3f planeNormal) {
	Vec3f perp = planeNormal.cross(WORLD_UP_VECTOR);
	Vec3f gravityVector = planeNormal.cross(perp);
	return gravityVector.normalize();
}

float PhysicsManager::calcPointPlaneIntersectTime(Vec3f startPos, Vec3f endPos, Vec3f planeNormal, float planeDist) {
	Vec3f ray = endPos - startPos;
	return -(planeNormal.dot(startPos) + planeDist) / ray.dot(planeNormal);

}

Vec3f* PhysicsManager::calcPointPlaneIntersect(Vec3f startPos, Vec3f endPos, Vec3f planeNormal, float planeDist) {
	
	float p = startPos.dot(planeNormal) + planeDist;
	//used to determine if the start/end positions changed relative to the plane
	enum {PlaneFront, PlaneBack, OnPlane};
	int pStartLoc;
	int pDestLoc;
    if ( p > 0.0f ) pStartLoc = PlaneFront;
    else if ( p < 0.0f ) pStartLoc = PlaneBack;
    else pStartLoc = OnPlane;
    
	
    p = endPos.dot(planeNormal) + planeDist;
    if( p > 0.0f ) pDestLoc = PlaneFront;
    else if (p < 0.0f ) pDestLoc = PlaneBack;
    else pDestLoc = OnPlane;

    if (pStartLoc == pDestLoc) return NULL;
	
	//get the length of time after startPos to collison
	float collTime = calcPointPlaneIntersectTime(startPos, endPos, planeNormal, planeDist);
	
	return new Vec3f(calcPosition(startPos, endPos-startPos, collTime));
}

Vec3f* PhysicsManager::calcSpherePlaneIntersect(float radius, Vec3f startPos, Vec3f endPos, Vec3f planeNormal, vector<Vec3f> planeVertices ) {
	//offset the plane in the direction of its normal by the radius of the sphere
	Vec3f offset = radius * planeNormal;

	//offset the planeVertices
	for (unsigned int i = 0; i < planeVertices.size(); i++) {
		planeVertices[i] += offset;
	}

	//calculate offset plane dist
	float planeDist = -planeNormal.dot(planeVertices[0]);

	Vec3f* collPoint = calcPointPlaneIntersect(startPos, endPos, planeNormal, planeDist);
	if (collPoint) return collPoint; //if the point was a valid collision on the offset plane
	return NULL;
}

Vec3f PhysicsManager::calcPlaneReflectionVelocity(Vec3f velocity, Vec3f planeNormal) {
	Vec3f direction = velocity.normalize();
	float mag = velocity.magnitude();

	//by reflection formula
	Vec3f reflectedDirection = 2 * (-direction).dot(planeNormal) * planeNormal + direction;
	return mag * reflectedDirection;
}