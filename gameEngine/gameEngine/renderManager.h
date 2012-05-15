#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include "externalLibs\mmanager.h"
#include "level.h"

class renderManager
{
public:
	static renderManager* Instance();//returns an instance of the manager

	//draw the level
	void drawLevel(CMMPointer<level> lev);
	//draw a tile
	void drawTile(CMMPointer<tile> t);
	//draw a ball
	void drawBall(CMMPointer<ball> b);
	//draw a cup
	void drawCup(CMMPointer<cup> c);
	//draw a wall
	void drawWall(Vec3f start, Vec3f end);

	//Draw a polygon with vertices, normal and color
	void drawPolygon(std::vector<Vec3f> vertices, Vec3f normal, Vec3f color);

private:
	renderManager(); //private constructor for singleton
	static renderManager* m_pInstance; //static instance of the manager
	~renderManager();

};

#endif