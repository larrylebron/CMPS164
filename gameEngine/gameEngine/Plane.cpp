#include "Plane.h"

int Plane::planeIdCount = 0;

Plane::Plane(vector<Vec3f> pVertices, int pId)
	: GameObject( pId )
{
	//init plane vertices
	vertices = pVertices;

	//init normal
	normal = PhysicsManager::Instance()->calcPlaneNormal(vertices);

	//calculate dist from origin for all points on plane
	dist = -normal.dot(vertices[0]);

	//Initialize the simple bounds for quick testing
	initBounds();
}
	
Plane::~Plane()
{
}

void Plane::initBounds() {
	minBounds = maxBounds = vertices[0];
	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vec3f v = vertices[i];

		if (v[0] < minBounds[0]) minBounds[0] = v[0]; //new min x
		else if (v[0] > maxBounds[0]) maxBounds[0] = v[0]; //new max x

		if (v[1] < minBounds[1]) minBounds[1] = v[1]; //new min y
		else if (v[1] > maxBounds[1]) maxBounds[1] = v[1]; //new max y

		if (v[2] < minBounds[2]) minBounds[2] = v[2]; //new min z
		else if (v[2] > maxBounds[2]) maxBounds[2] = v[2]; //new max z
	}
}

bool Plane::containsPoint(Vec3f p) {	

	//first check p against the simple bounding box
	if (p[0] < minBounds[0] || p[1] < minBounds[1] || p[2] < minBounds[2] ||
		p[0] > maxBounds[0] || p[1] > maxBounds[1] || p[2] > maxBounds[2]) return false;

	//check p against infinite plane
	if ( p.dot(normal) != dist ) return false;
	

	//Check p against finite plane's dimensions
	//The following code is adapted from Randolph Franklin's http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html#3D%20Polygons

  unsigned int i, j = 0;
  bool in = false;
  for (i = 0, j = vertices.size()-1; i < vertices.size(); j = i++) {
    if ( ((vertices[i][2]>p[2]) != (vertices[j][2]>p[2])) &&
	 (p[0] < (vertices[j][0]-vertices[i][0]) * (p[2]-vertices[i][2]) / (vertices[j][2]-vertices[i][2]) + vertices[i][0]) )
       in = !in;
  }
  return in;
}

float Plane::getDist() {
	return dist;
}

vector<Vec3f> Plane::getVertices() {
	vector<Vec3f> vCopy = vertices;
	return vCopy;
}

string Plane::toString() {
	std::stringstream ss;
	ss << "Plane " << id << ": \n";
	ss << "normal: " << normal;
	ss << endl;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		ss << vertices[i] << endl;
	}	
	return ss.str();
}
