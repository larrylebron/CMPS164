#include "tile.h"

tile::~tile(){
}

tile::tile(int pID, vector<Vec3f> pVertices, vector<int> pNeighbors, Vec3f pColor)
	: GameObject(id), Drawable (pColor)
{
	log = Logger::Instance();

	vertices = pVertices;
	neighbors = pNeighbors;

	initBounds(); //initialize min/max bound vectors for quick point testing
	normal = calcNormal(); //initialize the normal
	buildWalls();
}

void tile::buildWalls() {
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i] == 0) {
			Vec3f start = vertices[i];
			//It might be connecting the last vertex with the first one
			Vec3f end = (i == vertices.size() - 1) ? vertices[0] : vertices[i+1];
			CMMPointer<Wall> tempWall = new Wall(start, end, WALL_HEIGHT, WALL_COLOR );
			walls.push_back(tempWall);
		}
	}
}

Vec3f tile::calcNormal() {

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

void tile::initBounds() {
	minBounds = maxBounds = vertices[0];
	for (int i = 0; i < vertices.size(); i++) {
		Vec3f v = vertices[i];

		if (v[0] < minBounds[0]) minBounds[0] = v[0]; //new min x
		else if (v[0] > maxBounds[0]) maxBounds[0] = v[0]; //new max x

		if (v[1] < minBounds[1]) minBounds[1] = v[1]; //new min y
		else if (v[1] > maxBounds[1]) maxBounds[1] = v[1]; //new max y

		if (v[2] < minBounds[2]) minBounds[2] = v[2]; //new min z
		else if (v[2] > maxBounds[2]) maxBounds[2] = v[2]; //new max z
	}
}

bool tile::containsPoint(Vec3f p) {	

	//first check p against the simple bounding box
	if (p[0] < minBounds[0] || p[1] < minBounds[1] || p[2] < minBounds[2] ||
		p[0] > maxBounds[0] || p[1] > maxBounds[1] || p[2] > maxBounds[2]) return false;

	//check p against infinite plane
	if ( p.dot(normal) != vertices[0].dot(normal) ) return false;
	

	//Check p against finite plane's dimensions
	//The following code is adapted from Randolph Franklin's http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html#3D%20Polygons

  int i, j = 0;
  bool in = false;
  for (i = 0, j = vertices.size()-1; i < vertices.size(); j = i++) {
    if ( ((vertices[i][2]>p[2]) != (vertices[j][2]>p[2])) &&
	 (p[0] < (vertices[j][0]-vertices[i][0]) * (p[2]-vertices[i][2]) / (vertices[j][2]-vertices[i][2]) + vertices[i][0]) )
       in = !in;
  }
  return in;
}

void tile::draw() {
	renderManager::Instance()->drawPolygon(vertices, normal, color);

	for (int i = 0; i < walls.size(); i++) {
		walls[i]->draw();
	}
}

Vec3f tile::getNormal() 
{
	Vec3f nCopy = normal;
	return nCopy;
}

vector<Vec3f> tile::getVertices() {
	vector<Vec3f> vCopy = vertices;
	return vCopy;
}

vector<int> tile::getNeighbors(){
	vector<int> nCopy = neighbors;
	return nCopy;
}

int tile::getNumEdges() {
	return neighbors.size();
}

string tile::toString() {
	std::stringstream ss;
	ss << "tile id: " << id << endl << "normal: " << normal[0] <<
		", " << normal[1] << ", " << normal[2] << endl;
	return ss.str();
	/*
	cout << "tile id: " << id << endl;
	cout << "numVerts: " << vertices.size() << endl;
	for (int i = 0; i < vertices.size(); i++) {
		cout << vertices[i] << endl;
	}
	cout << "NumEdges: " << numEdges << endl;
	for (int i = 0; i < numEdges; i++) {
		cout << neighbors[i] << endl;
	}
	cout << "Normal: " << normal << endl;
	*/
}
