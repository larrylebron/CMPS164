#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include "util.h"
#include "level.h"

using std::ifstream;
using std::cerr;
using std::vector;


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

