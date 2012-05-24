/**
* CMPS 164 
* Central Repository for term and library inclusions
*/

#ifndef UTIL_H
#define UTIL_H
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "Logger.h"
#include "externalLibs\vec3f.h"
#include "externalLibs\mmanager.h"

const double PI = 3.1415926538;

//Frame length in seconds -- for 60 fps -- broke the physics in places
const double FRAME_TIME = .0166666666666666666666;

//for 30 fps
//const double FRAME_TIME = .03333333333333333333333;

//world parameters
const Vec3f WALL_COLOR(1.0f, 0.5f, 0.5f);
const float BOUNDING_PLANE_HEIGHT = .1;
const Vec3f BALL_COLOR(1.0f, 0.0f, 0.0f);
const float BALL_RADIUS = .05f;
const Vec3f CUP_COLOR(0.0f, 0.0f, 0.0f);
const float CUP_RADIUS = .07f;
const Vec3f TILE_COLOR(0.0f, 0.9f, 0.0f);
const Vec3f HIGHLIGHT_COLOR(1,1,1);

const Vec3f WORLD_UP_VECTOR(0,1,0);

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

#endif