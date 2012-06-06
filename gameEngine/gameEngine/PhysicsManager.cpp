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

 Vec3f PhysicsManager::calcFrictionVector(float frictionMagnitude, Vec3f velocity) {
	if (velocity.magnitude() == 0) return Vec3f(0,0,0);//if object is stationary
	return velocity.normalize() * -1 * frictionMagnitude;
 }

float PhysicsManager::calcPointPlaneIntersectTime(Vec3f startPos, Vec3f velocity, Vec3f planeNormal, float planeDist) {
	return -(planeNormal.dot(startPos) + planeDist) / velocity.dot(planeNormal);
}

bool PhysicsManager::pointPlaneIntersect(Vec3f startPos, Vec3f velocity, float duration, Vec3f planeNormal, float planeDist) {
	
	//get the length of time after startPos to collison
	float collTime = calcPointPlaneIntersectTime(startPos, velocity, planeNormal, planeDist);

	//collides in this duration
	if (collTime >= 0 && collTime <= duration) return true;
	else return false;
}

float PhysicsManager::calcSpherePlaneIntersectTime(float radius, Vec3f startPos, Vec3f velocity, Vec3f planeNormal, vector<Vec3f> planeVertices ) {
	
	//offset the plane in the direction of its normal by the radius of the sphere
	Vec3f offset = radius * planeNormal;

	//offset the planeVertices
	for (unsigned int i = 0; i < planeVertices.size(); i++) {
		planeVertices[i] += offset;
	}

	//calculate offset plane dist
	float planeDist = -planeNormal.dot(planeVertices[0]);

	//return the time at which the sphere's center hits the offset plane
	//same as the time the sphere's side hits the original plane
	return calcPointPlaneIntersectTime(startPos, velocity, planeNormal, planeDist);
}

Vec3f PhysicsManager::calcPlaneReflectionVelocity(Vec3f velocity, Vec3f planeNormal) {
	Vec3f direction = velocity.normalize();
	float mag = velocity.magnitude();

	//by reflection formula
	Vec3f reflectedDirection = 2 * (-direction).dot(planeNormal) * planeNormal + direction;
	return mag * reflectedDirection;
}