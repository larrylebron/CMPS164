#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glut.h>
#include "util.h"
#include "level.h"
class renderManager : public IMMObject
{
public:
	renderManager();
	~renderManager();

	//draw the level
	void drawLevel(CMMPointer<level> lev);
	//draw a tile
	void drawTile(CMMPointer<tile> t);
	//draw a ball
	void drawBall(CMMPointer<ball> b);
	//draw a cup
	void drawCup(CMMPointer<cup> c);
	AUTO_SIZE;
};

