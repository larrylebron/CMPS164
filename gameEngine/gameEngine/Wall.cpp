#include "Wall.h"

int Wall::numWalls = 0;

Wall::Wall(Vec3f start, Vec3f end, float pHeight, Vec3f pColor) {
	id = ++numWalls;
	
	height = pHeight;

	//Initialize vertex vector
	vertices.push_back(start);
	vertices.push_back( Vec3f(start[0], start[1] + height, start[2]) );
	vertices.push_back( Vec3f(end[0], end[1] + height, end[2]) );
	vertices.push_back( Vec3f(end) );

	//Initialize color
	color = pColor;

	//init normal
	initNormal();

}
	
Wall::~Wall()
{
}

void Wall::initNormal() {

	normal = Vec3f(0,0,0);
	int numVertices = vertices.size();
	for (int i = 0; i < numVertices; i++) {
		Vec3f current = vertices[i];
		Vec3f next = vertices[ (i+1) % numVertices]; //allows for calculation of last edge to connect to 1st vert
	
		normal[0] += (current[1] - next[1]) * (current[2] + next[2]);
		normal[1] += (current[2] - next[2]) * (current[0] + next[0]);
		normal[2] += (current[0] - next[0]) * (current[1] + next[1]);
	}
	
	normal *= -1; //because it's flipped for some reason
	normal = normal.normalize();
}

void Wall::draw() {
	//draw the wall
	//renderManager::Instance()->drawPolygon(vertices, normal, color);
}

string Wall::toString() {
	std::stringstream ss;
	//ss << "Wall info: " << 
	return ss.str();
}
