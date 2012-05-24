#include "tile.h"

tile::~tile(){
}

tile::tile(int pID, vector<Vec3f> pVertices, vector<int> pNeighbors, Vec3f pColor)
	: Plane(pVertices, pID), Drawable (pColor)
{

	neighbors = pNeighbors;
	
	normal = PhysicsManager::Instance()->calcPlaneNormal(vertices); //initialize the normal
	buildEdgePlanes();

	highlighted = false;
}

void tile::toggleHighlight() {
	highlighted = !highlighted;
}



void tile::buildEdgePlanes() {
	for (int i = 0; i < neighbors.size(); i++) {
			//the edge boundary plane
			vector<Vec3f> planeVertices;

			Vec3f start = vertices[i];
			//It might be connecting the last vertex with the first one
			Vec3f end = (i == vertices.size() - 1) ? vertices[0] : vertices[i+1];

			//Assemble the plane vertices
			planeVertices.push_back(start);
			planeVertices.push_back( Vec3f(start[0], start[1] + BOUNDING_PLANE_HEIGHT, start[2]) );
			planeVertices.push_back( Vec3f(end[0], end[1] + BOUNDING_PLANE_HEIGHT, end[2]) );
			planeVertices.push_back( Vec3f(end) );

		//there is no neighboring tile
		if (neighbors[i] == 0) {
			edgePlanes.push_back(new Wall(planeVertices, WALL_COLOR));
		}//end if neighbors == 0

		else {
			edgePlanes.push_back(new Plane(planeVertices));
		
		}
	}//end iteration through neighbors
}//end buildBounds

void tile::draw() {
	if (highlighted) renderManager::Instance()->drawPolygon(vertices, normal, HIGHLIGHT_COLOR);
	else renderManager::Instance()->drawPolygon(vertices, normal, color);

	for (int i = 0; i < edgePlanes.size(); i++) {
		//only call draw on the walled edges
		if (neighbors[i] == 0) {
			Plane* bP = edgePlanes[i];
			Wall* w = static_cast<Wall*>(bP);
			w->draw();
		}
	}
}

vector<CMMPointer<Plane>> tile::getEdgePlanes() {
	vector<CMMPointer<Plane>>ePCopy = edgePlanes;
	return ePCopy;
}

int tile::getNumEdges() {
	return vertices.size();
}

vector<int> tile::getNeighbors() {
	vector<int>nCopy = neighbors;
	return nCopy;
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
