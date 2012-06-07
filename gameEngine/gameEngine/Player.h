#pragma once

#include <vector>
#include "util.h"

class Player : public IMMObject
{
public:
	Player(void);
	~Player(void);
	string getName();
	void setName(string name);
	vector<int> getScores();
	void addNewScore(int score);
	void updateScore(int score);
	AUTO_SIZE;
private:
	vector<int> scores;
	string name;
};

