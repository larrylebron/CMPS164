#pragma once
#include "util.h"
#include "Course.h"



class HUD : public IMMObject
{
public:
	HUD(string pName);
	~HUD(void);
	AUTO_SIZE;
	
	void updateBocceScore(int score, int winner);
	void updateInfo(CMMPointer<Course> course, float gAngle, float gPower);
	void toggleShowScore();
	void toggleShowHoleInfo();
	void setShowShootingUI(bool show);
	//void toggleShowWinMessage();
	void draw(bool isBocce);
private:
	//load and save the high score file
	void drawScoreTable(float& x, float& y, std::ostringstream& stream);
	void drawFinalScore(float& x, float& y, std::ostringstream& stream);
	void loadHighScoresFile(char* filename = "highScores.txt");
	void saveHighScoresFile(char* filename = "highScores.txt");
	//update the high scores with the player's current score
	void updateHighScores();
	//the high score names and scores
	vector<string> highScoreNames;
	vector<int> highScores;

	bool showScore;
	bool showHoleInfo;
	bool showShootingUI;
	bool showWinMessage;
	bool gameWon;
	bool init;
	int* scores;
	int holeNumber;
	int numHoles;
	string holeName;
	int toPar;
	int stroke;
	float angle;
	float power;
	int finalScore;
	int bocceScore;
	int boccePlayer;
	string name;

};

