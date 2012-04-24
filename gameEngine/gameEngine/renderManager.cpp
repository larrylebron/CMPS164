#include "renderManager.h"


renderManager::renderManager()
{
}


renderManager::~renderManager()
{
	cout << "render manager destructor";
}

void renderManager::drawLevel(CMMPointer<level> lev) {
	//get the level info
	std::map<int, CMMPointer<tile>> tiles = lev->getTiles();
	std::map<int, CMMPointer<ball>> balls = lev->getBalls();
	std::map<int, CMMPointer<tee>> tees = lev->getTees();
	std::map<int, CMMPointer<cup>> cups = lev->getCups();

	//

	std::map<int, CMMPointer<tile>>::iterator itT;
	for ( itT=tiles.begin() ; itT != tiles.end(); itT++ ) {
		drawTile((*itT).second);
	}
	
	std::map<int, CMMPointer<ball>>::iterator itB;
	for ( itB=balls.begin() ; itB!= balls.end(); itB++ ) {
		drawBall((*itB).second);
	}

	std::map<int, CMMPointer<cup>>::iterator itC;
	for ( itC=cups.begin() ; itC != cups.end(); itC++ ) {
		drawCup((*itC).second);
	}
	

	
}

void renderManager::drawTile(CMMPointer<tile> t) 
{
	GLfloat tileColor[3] = { 0.0f, 0.9f, 0.0f };
	glColor3fv(tileColor);
	Vec3f normal = t->getNormal();
	Vec3f* vertices = t->getVertices();
	int numVerts = t->getNumVertices();

	glBegin(GL_POLYGON);
		glNormal3f(normal[0], normal[1], normal[2]);
		for (int i = 0; i < numVerts; i++)
		{
			glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
		}
	glEnd();

	int numEdges = t->getNumEdges();
	int* neighbors = t->getNeighbors();

	for (int i = 0; i < numEdges; i++) {
		if (neighbors[i] == 0) {
			Vec3f start = vertices[i];
			Vec3f end = (i == numEdges - 1) ? vertices[0] : vertices[i+1];
			drawWall(start, end);
		}
	}
}

void renderManager::drawBall(CMMPointer<ball> b) 
{
	Vec3f position = b->getPosition();
	
	GLfloat ballColor[3] = { 1.0f, 0.0f, 0.0f };
	glColor3fv(ballColor);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(.05, 10, 10);
	glPopMatrix();
}


void renderManager::drawCup(CMMPointer<cup> c) 
{
	Vec3f position = c->getPosition();
	GLfloat ballColor[3] = { 0.0f, 0.0f, 0.0f };
	glColor3fv(ballColor);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(.07, 10, 10);
	glPopMatrix();
}

void renderManager::drawWall(Vec3f start, Vec3f end) {
	float wallHeight = 0.05f;
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex3f(start[0], start[1], start[2]);
	glVertex3f(start[0], start[1] + wallHeight, start[2]);
	glVertex3f(end[0], end[1] + wallHeight, end[2]);
	glVertex3f(end[0], end[1], end[2]);
	glEnd();
}

