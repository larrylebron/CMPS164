#include "Course.h"


Course::Course(void)
{
	currentLevel = 1;
	numLevels = 0;
	completedLevels = 0;
	courseName = " ";
}


Course::~Course(void)
{
}

void Course::addLevel(int pId, CMMPointer<level> level)
{
	numLevels++;
	levels[pId] = level;
}

void Course::nextLevel()
{
	currentLevel++;

	if (currentLevel > numLevels)
		currentLevel = 1;
}

//void Course::completeLevel(int pId)

void Course::previousLevel()
{
	currentLevel--;

	if (currentLevel < 1)
		currentLevel = numLevels;
}

int Course::getNumLevels()
{
	return numLevels;
}

void Course::setNumLevels(int num)
{
	numLevels = num;
}

string Course::getCourseName()
{
	return courseName;
}

void Course::setCourseName(string name)
{
	courseName = name;
}

bool Course::won()
{
	return false;
}

CMMPointer<level> Course::getCurrentLevel()
{
	return levels[currentLevel];
}

int Course::getCurrentLevelId()
{
	return currentLevel;
}