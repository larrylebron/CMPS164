#pragma once
#include "util.h"
#include "Course.h"


class HUD : public IMMObject
{
public:
	HUD(void);
	~HUD(void);
	AUTO_SIZE;
	
	void updateInfo(CMMPointer<Course> course, float gAngle, float gPower);
	void toggleShowScore();
	void toggleShowHoleInfo();
	void setShowShootingUI(bool show);
	//void toggleShowWinMessage();
	void draw(bool isBocce);
private:
	bool showScore;
	bool showHoleInfo;
	bool showShootingUI;
	bool showWinMessage;
	bool init;
	int* scores;
	int holeNumber;
	string holeName;
	int toPar;
	int stroke;
	float angle;
	float power;
};

