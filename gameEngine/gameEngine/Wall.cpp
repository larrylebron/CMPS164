#include "Wall.h"


Wall::Wall(vector<Vec3f> pVertices, Vec3f pColor)
	: Plane(pVertices), Drawable(pColor)
{
}
	
Wall::~Wall()
{
}

void Wall::draw() {
	//draw the wall
	renderManager::Instance()->drawPolygon(vertices, normal, color);
}

string Wall::toString() {
	std::stringstream ss;
	
	ss << "\nWall " << id << ": \n";
	ss << "normal: " << normal << endl;
	ss << "vertices: \n";
	for (unsigned int i = 0; i < vertices.size(); i++) {
		ss << vertices[i] << endl;
	}
	return ss.str();
}
