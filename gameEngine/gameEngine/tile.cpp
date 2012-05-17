#include "tile.h"

tile::~tile()
{
	if (vertices) delete[] vertices;
	if (neighbors) delete[] neighbors;
}

tile::tile(int pID, int pNumVerts, int pNumEdges, Vec3f* pVertices, int* pNeighbors)
{
	log = Logger::Instance();
	id = pID;
	numVertices = pNumVerts;
	numEdges = pNumEdges;

	vertices = new Vec3f[numVertices];
	for (int i = 0; i < numVertices; i++) {
		vertices[i] = pVertices[i];
	}
	neighbors = new int[numEdges];
	for (int i = 0; i < numEdges; i++) {
		neighbors[i] = pNeighbors[i];
	}

	initNormal();
	initBounds(); //initialize min/max bound vectors for quick point testing

	//Build walls
	for (int i = 0; i < numEdges; i++) {
		if (neighbors[i] == 0) {
			Vec3f start = vertices[i];
			//It might be connecting the last vertex with the first one
			Vec3f end = (i == numEdges - 1) ? vertices[0] : vertices[i+1];
			CMMPointer<Wall> tempWall = new Wall(start, end, WALL_HEIGHT, WALL_COLOR );
			walls.push_back(tempWall);
		}
	}
}

void tile::initNormal() {

	normal = Vec3f(0,0,0);

	for (int i = 0; i < numVertices; i++) {
		Vec3f current = vertices[i];
		Vec3f next = vertices[ (i+1) % numVertices]; //allows for calculation of last edge to connect to 1st vert
	
		normal[0] += (current[1] - next[1]) * (current[2] + next[2]);
		normal[1] += (current[2] - next[2]) * (current[0] + next[0]);
		normal[2] += (current[0] - next[0]) * (current[1] + next[1]);
	}

	normal = normal.normalize();
}

void tile::initBounds() {
	minBounds = maxBounds = vertices[0];
	for (int i = 0; i < numVertices; i++) {
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
  for (i = 0, j = numVertices-1; i < numVertices; j = i++) {
    if ( ((vertices[i][2]>p[2]) != (vertices[j][2]>p[2])) &&
	 (p[0] < (vertices[j][0]-vertices[i][0]) * (p[2]-vertices[i][2]) / (vertices[j][2]-vertices[i][2]) + vertices[i][0]) )
       in = !in;
  }
  return in;
}

Vec3f tile::getNormal() 
{
	Vec3f nCopy = normal;
	return nCopy;
}

Vec3f* tile::getVertices() 
{
	Vec3f* vCopy= new Vec3f[numVertices];
	for (int i = 0; i < numVertices; i++) {
		vCopy[i] = vertices[i];
	}
	return vCopy;
}

int tile::getNumVertices() 
{
	int cNumVerts = numVertices;
	return cNumVerts;
}

int tile::getNumEdges() 
{
	int cNumEdges = numEdges;
	return cNumEdges;
}

int* tile::getNeighbors(){
	int* nCopy= new int[numEdges];
	for (int i = 0; i < numEdges; i++) {
		nCopy[i] = neighbors[i];
	}
	return nCopy;
}

int tile::getId() {
	int cId = id;
	return cId;
}

string tile::toString() {
	std::stringstream ss;
	ss << "tile id: " << id << endl << "normal: " << normal[0] <<
		", " << normal[1] << ", " << normal[2] << endl;
	return ss.str();
	/*
	cout << "tile id: " << id << endl;
	cout << "numVerts: " << numVertices << endl;
	for (int i = 0; i < numVertices; i++) {
		cout << vertices[i] << endl;
	}
	cout << "NumEdges: " << numEdges << endl;
	for (int i = 0; i < numEdges; i++) {
		cout << neighbors[i] << endl;
	}
	cout << "Normal: " << normal << endl;
	*/
}
