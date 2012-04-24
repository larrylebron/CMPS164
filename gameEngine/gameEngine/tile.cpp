#include "tile.h"


tile::tile()
{
}


tile::~tile()
{
	if (vertices) delete[] vertices;
	if (neighbors) delete[] neighbors;
}

tile::tile(int pID, int pNumVerts, int pNumEdges, Vec3f* pVertices, int* pNeighbors, Vec3f pNormal)
{
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
	normal  = pNormal;
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

void tile::printInfo() {
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
}
