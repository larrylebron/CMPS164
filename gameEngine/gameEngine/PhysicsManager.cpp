#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::m_pInstance = 0;  

PhysicsManager::PhysicsManager() {
	log = Logger::Instance();
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

