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

#include "externalLibs\vec3f.h"
#include "externalLibs/mmanager.h"

const double PI = 3.1415926538;

//world parameters
const Vec3f WALL_COLOR(1.0f, 0.5f, 0.5f);
const float WALL_HEIGHT = .5f;

using std::string;
using std::cout;
using std::cerr;
using std::endl;

#endif