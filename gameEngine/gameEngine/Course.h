#pragma once

#include "util.h"
#include "level.h"

class Course : public IMMObject
{
public:
	Course();
	~Course();
	void addLevel(int pId, CMMPointer<level> level);
	void nextLevel();
	void previousLevel();
	bool won();
	int getNumLevels();
	void setNumLevels(int num);
	string getCourseName();
	void setCourseName(string name);
	CMMPointer<level> getCurrentLevel();
	int getCurrentLevelId();

	
	AUTO_SIZE;
private:
	// get pointer to current level
	std::map<int, CMMPointer<level>> levels;
	int numLevels;
	int currentLevel;
	int completedLevels;
	string courseName;
};

