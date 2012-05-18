#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include "util.h"

class renderManager
{
public:
	static renderManager* Instance();//returns an instance of the manager

	//Draw a polygon with vertices, normal and color
	void drawPolygon(vector<Vec3f> vertices, Vec3f normal, Vec3f color);

	//draw a sphere with radius and color at position
	void drawSphere(float radius, Vec3f normal, Vec3f position, Vec3f color);

private:
	renderManager(); //private constructor for singleton
	static renderManager* m_pInstance; //static instance of the manager
	~renderManager();

};

#endif