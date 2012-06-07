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


//world parameters
const Vec3f WALL_COLOR(1.0f, 0.5f, 0.5f);
const float BOUNDING_PLANE_HEIGHT = .1f;
const Vec3f BALL_COLOR(1.0f, 0.0f, 0.0f);
const Vec3f BALL_P1_COLOR(1.0f, 0.0f, 1.0f);
const Vec3f BALL_P2_COLOR(0.0f, 0.0f, 1.0f);
const float BALL_RADIUS = .05f;
const Vec3f CUP_COLOR(0.0f, 0.0f, 0.0f);
const float CUP_RADIUS = .07f;
const Vec3f TILE_COLOR(0.0f, 0.9f, 0.0f);
const Vec3f HIGHLIGHT_COLOR(1,1,1);
const Vec3f WORLD_UP_VECTOR(0,1,0);
const float DEFAULT_FRICTION_MAGNITUDE = .05f;//the velocity magnitude under which the ball won't move
const float GRAVITY_MAGNITUDE = .06f;
const float MAX_POWER = 5.0f;
const float KEEP_LAST_SHOOTING_SETTINGS = false;

const float UI_BORDER = 10;
const float HOLE_INFO_HEIGHT_A = 70;
const float HOLE_INFO_HEIGHT_B = 30;
const float HOLE_INFO_WIDTH_A = 400;
const float HOLE_INFO_WIDTH_B = 100;
const float RESULT_HEIGHT = 60;
const float RESULT_WIDTH = 240;
const int DUMMY_VALUE = 10000;
const float SHOOTING_UI_HEIGHT = 300;
const float SHOOTING_UI_WIDTH = 300;
const float POWER_WIDTH = 200;
const float POWER_HEIGHT = 30;
const float POWER_BORDER = 4;

const int viewportWidth = 1024;
const int viewportHeight = 768;

const int NUM_BPLAYERS = 2;
const int NUM_BBALLS = 4;

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

#endif