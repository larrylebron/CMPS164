#include "Wall.h"

int Wall::wallIdCount = 0;

Wall::Wall(Vec3f start, Vec3f end, float pHeight, Vec3f pColor)
	: GameObject( ++wallIdCount ), Drawable(pColor)
{
	normal = calcNormal();
	height = pHeight;

	//Initialize vertex vector
	vertices.push_back(start);
	vertices.push_back( Vec3f(start[0], start[1] + height, start[2]) );
	vertices.push_back( Vec3f(end[0], end[1] + height, end[2]) );
	vertices.push_back( Vec3f(end) );
}
	
Wall::~Wall()
{
}

Vec3f Wall::calcNormal() {

	Vec3f tNormal = Vec3f(0,0,0);

	for (int i = 0; i < vertices.size(); i++) {
		Vec3f current = vertices[i];
		Vec3f next = vertices[ (i+1) % vertices.size()]; //allows for calculation of last edge to connect to 1st vert
	
		tNormal[0] += (current[1] - next[1]) * (current[2] + next[2]);
		tNormal[1] += (current[2] - next[2]) * (current[0] + next[0]);
		tNormal[2] += (current[0] - next[0]) * (current[1] + next[1]);
	}

	return tNormal.normalize();
}


void Wall::draw() {
	//draw the wall
	renderManager::Instance()->drawPolygon(vertices, normal, color);
}

string Wall::toString() {
	std::stringstream ss;
	//ss << "Wall info: " << 
	return ss.str();
}
