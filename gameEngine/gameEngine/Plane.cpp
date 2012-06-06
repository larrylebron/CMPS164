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

	//if the min/max y vertices are equal, plane is flat, set gravity direction to 0
	if(minBounds[1] == maxBounds[1]) {
		gravityDirection = Vec3f(0,0,0);
		sloped = false;
	} else {
		//set the gravity direction for a sloped tile
		gravityDirection = PhysicsManager::Instance()->calcPlaneGravityDirection(normal);
		sloped = true;
	}
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

bool Plane::inSimpleBounds(Vec3f p) {	

	//first check p against the simple bounding box
	if (p[0] < minBounds[0] || p[1] < minBounds[1] || p[2] < minBounds[2] ||
		p[0] > maxBounds[0] || p[1] > maxBounds[1] || p[2] > maxBounds[2]) return false;
	return true;
	//was using this more complicated successive checking, but I think it was actualy too precise
	//for the level files
}

bool Plane::preciseContainsPoint(Vec3f p) {
	//check p against infinite plane
	
	if ( p.dot(normal) != dist ) return false;

	/*draw a vector from point to each vertex
	If the sum of all angles is very close to 2PI, it's on the finite plane
	*/
	double angleSum = 0;
	
	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vec3f v1 = (p - vertices[i]).normalize();
		
		//make sure it loops around
		Vec3f v2 = (i == vertices.size()-1) ? (p-vertices[0]).normalize() : (p - vertices[i+1]).normalize();

		angleSum += acos(v1.dot(v2));
	}
	if ( fabs(angleSum - (2 * PI)) < 0.1)
        return true;
    else
        return false;
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
	ss << "\nPlane " << id << ": \n";
	ss << "normal: " << normal << endl;
	ss << "vertices: \n";
	for (unsigned int i = 0; i < vertices.size(); i++) {
		ss << vertices[i] << endl;
	}
	return ss.str();
}
