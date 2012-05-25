#include "cup.h"


cup::cup(int pID, Vec3f pPosition, Vec3f pColor, float pRadius)
	: GameObject(pID), Drawable(pColor)
{
	position = pPosition;
	radius = pRadius;
}


cup::~cup()
{
}



void cup::draw() {
	renderManager::Instance()->drawSphere(radius, normal, position, color);
}

Vec3f cup::getPosition() {
	Vec3f posCopy = position;
	return posCopy;
}

float cup::getRadius() {
	return radius;
}

string cup::toString() {
	std::stringstream ss;
	ss << "cup id: " << id << endl << "position: " << position << endl;
	return ss.str();
}
