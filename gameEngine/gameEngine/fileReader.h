#ifndef FILE_READER_H
#define FILE_READER_H

#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include "util.h"
#include "Logger.h"
#include "level.h"

using std::ifstream;


static const string DataTypeTile = "tile";
static const string DataTypeTee = "tee";
static const string DataTypeCup = "cup";

class fileReader
{
public:
	fileReader();
	~fileReader();
	//reads in the level file. Returns false if an error occurs
	bool readFile(char* filename, CMMPointer<level> map);
	vector<string> strSplit(string data, string delims = " ");
};

#endif