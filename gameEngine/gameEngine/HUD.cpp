#include "HUD.h"



HUD::HUD(string pName)
{
	showScore = false;
	showHoleInfo = true;
	showShootingUI = false;
	showWinMessage = false;
	init = false;
	gameWon = false;
	bocceScore = 0;
	boccePlayer = 0;

	name = pName;

	holeNumber = 1;
	holeName = "";
	toPar = 1;
	stroke = 1;
	angle = PI;
	power = 0.5;

	loadHighScoresFile();
}

void HUD::loadHighScoresFile(char* filename) {
	std::ifstream input(filename);

	//throws error if input fails
	if (input.fail()){
		std::string stringFile(filename); //string filename for passing to the logger
		Logger::Instance()->err("HUD failed to open high score file: " + stringFile);
		return;
	}
	string info;
	int lineNumber = 0;
	while( std::getline(input, info) ) {
		//even number lines are the high score names
		if (lineNumber % 2 == 0) { 
			highScoreNames.push_back(info);
		}
		else {
			highScores.push_back(atoi(info.c_str()));
		}
		lineNumber++;
	}
}

void HUD::updateHighScores() {
	//update the high scores file with the new score & name if it qualifies
	bool insertedScore = false;

	vector<int>::iterator scoreIt;
	vector<string>::iterator nameIt;

	for (scoreIt = highScores.begin(), nameIt = highScoreNames.begin(); 
		scoreIt != highScores.end() && !insertedScore; 
		scoreIt++, nameIt++) {
			if (finalScore < *scoreIt) {
				highScores.insert(scoreIt, finalScore);
				highScoreNames.insert(nameIt, name);
				highScores.pop_back();
				highScoreNames.pop_back();
				insertedScore = true;
			}
	}
}

void HUD::saveHighScoresFile(char* filename) {
	std::ofstream output(filename);

	//throws error if input fails
	if (output.fail()){
		std::string stringFile(filename); //string filename for passing to the logger
		Logger::Instance()->err("HUD failed to output to high score file: " + stringFile);
		return;
	}

	for (int i = 0; i < highScores.size(); i++) {
		output << highScoreNames[i] << endl << highScores[i] << endl;
	}
}



HUD::~HUD(void)
{
}

void HUD::updateBocceScore(int score, int winner) {
	bocceScore = score;
	boccePlayer = winner;
}

void HUD::updateInfo(CMMPointer<Course> course, float gAngle, float gPower)
{
	if (!init)
	{
		numHoles = course->getNumLevels();
		scores = new int[numHoles];

		for (int i = 0; i < course->getNumLevels(); i++) {
			scores[i] = DUMMY_VALUE;
		}

		init = true;
	}

	CMMPointer<level> current = course->getCurrentLevel();
	holeNumber = course->getCurrentLevelId();
	holeName = current->getLevelName();
	toPar = current->getPar();
	stroke = current->getStrokes();
	scores[holeNumber - 1] = current->getScore();
	showWinMessage = current->isComplete();
	angle = gAngle;
	power = gPower;

	glutPostRedisplay();
}

void HUD::toggleShowScore(){
	showScore = !showScore;
}

void HUD::toggleShowHoleInfo(){
	showHoleInfo = !showHoleInfo;
}

void HUD::setShowShootingUI(bool show){
	showShootingUI = show;
}

//void HUD::toggleShowWinMessage(){
//	showWinMessage = !showWinMessage;
//}

void draw_string( float x, float y, char *txt ) {
	glColor3f(0, 0, 0);
	glRasterPos2f( x, y);
	while( *txt != '\0' ) {
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *txt );
		txt++;
	}
}

void drawString (float x, float y, std::ostringstream& stream) {
	string str;
	char *print;
	str = stream.str();
	print = (char *)str.c_str();
	draw_string(x, y, print);

	stream.clear();
	stream.str("");
}

string getScoreString (int score) {
	std::ostringstream stream;

	if (score == DUMMY_VALUE) {
		stream << '-';
	}

	else if (score > 0) {
		stream << "+" << score;
	}
	else {
		stream << score;
	} 



	return stream.str();
}

void HUD::drawScoreTable(float& x, float& y, std::ostringstream& stream) {
	// first nine holes
	y += 10 * UI_BORDER;
	x += 4 * UI_BORDER;
	stream << "HOLE";
	drawString(x, y, stream);

	stream << "SCORE";
	drawString(x, y + 4 * UI_BORDER, stream);

	int halfWay = numHoles / 2;

	for (int i = 0; i < halfWay; i++) {
		x += 10 * UI_BORDER;
		stream << i + 1;
		drawString(x, y, stream);

		if (i == holeNumber - 1) {
			if (showWinMessage)
				stream << getScoreString(scores[i]);
			stream << "*";
		} else
			stream << getScoreString(scores[i]);
		drawString(x, y + 4 * UI_BORDER, stream);
	}

	// next nine holes
	y += 10 * UI_BORDER;
	x = 5 * UI_BORDER;
	stream << "HOLE";
	drawString(x, y, stream);

	stream << "SCORE";
	drawString(x, y + 4 * UI_BORDER, stream);

	for (int i = halfWay; i < numHoles; i++) {
		x += 10 * UI_BORDER;
		stream << i + 1;
		drawString(x, y, stream);

		if (i == holeNumber - 1) {
			if (showWinMessage)
				stream << getScoreString(scores[i]);
			stream << "*";
		} else
			stream << getScoreString(scores[i]);
		drawString(x, y + 4 * UI_BORDER, stream);
	}
}

void HUD::drawFinalScore(float& x, float& y, std::ostringstream& stream) {
	stream << "TOTAL SCORE";
	drawString(x, y, stream);

	// calculate score
	int tScore = 0;
	int numScores = 0;
	if (!showWinMessage && holeNumber == 1) {
		stream << "-";
	} else {
		for (int i = 0; i < numHoles; i++) {
			if (scores[i] != DUMMY_VALUE && ((i == holeNumber - 1 && showWinMessage) || (i < holeNumber - 1))) {
				tScore += scores[i];
				numScores++;
			}
		}
		stream << tScore;
	}
	drawString(x + 15 * UI_BORDER, y, stream);
	
	if (numScores == numHoles && !gameWon) {
		finalScore = tScore;
		gameWon = true;
		updateHighScores();
		saveHighScoresFile();
		toggleShowScore();
	}

	if (gameWon) {
		y += 6 * UI_BORDER;
		x = viewportWidth / 5;

		stream << "CONGRATULATIONS YOU HAVE COMPLETED THE GAME! HERE ARE THE HIGHSCORES:";
		drawString(x, y, stream);

		y += 4 * UI_BORDER;
		x = viewportWidth / 3;

		stream << "Name";
		drawString(x, y, stream);

		stream << "Score";
		drawString(viewportWidth / 2, y, stream); 

		for (int i = 0; i < highScoreNames.size(); i++) {					
			y += 3 * UI_BORDER;

			stream << highScoreNames[i];
			drawString(x, y, stream);

			stream << highScores[i];
			drawString(viewportWidth / 2, y, stream);
		}
	}
}

void HUD::draw(bool isBocce) {
	// if showing score screen, only draw that
	float x, y;
	x = y = UI_BORDER;
	std::ostringstream stream;

	if (showScore || gameWon) {
		if (!isBocce) {
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(x,y);
			glVertex2f(x,viewportHeight - y);
			glVertex2f(viewportWidth - x, viewportHeight - y);
			glVertex2f(viewportWidth - x, y);
			glEnd();

			drawScoreTable(x, y, stream);

			y += 10 * UI_BORDER;
			x = 5 * UI_BORDER;

			drawFinalScore(x, y, stream);
		}
	} else {
		if (showHoleInfo) {
			if (!isBocce) {
				// draw first box
				glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex2f(x, y);
				glVertex2f(x, y + HOLE_INFO_HEIGHT_A); 
				glVertex2f(x + HOLE_INFO_WIDTH_A, y + HOLE_INFO_HEIGHT_A); 
				glVertex2f(x + HOLE_INFO_WIDTH_A, y);

				glVertex2f(viewportWidth - x, y);
				glVertex2f(viewportWidth - x, y + HOLE_INFO_HEIGHT_B); 
				glVertex2f(viewportWidth - x - HOLE_INFO_WIDTH_B, y + HOLE_INFO_HEIGHT_B); 
				glVertex2f(viewportWidth - x - HOLE_INFO_WIDTH_B, y);
				glEnd();


				y += 2 * UI_BORDER;
				stream << "HOLE #" << holeNumber;
				drawString(x + UI_BORDER, y, stream);

				stream << "Stroke: " << stroke;
				drawString(viewportWidth - x - HOLE_INFO_WIDTH_B + UI_BORDER, y, stream);

				y += 2 * UI_BORDER;
				stream << holeName;
				drawString(x + UI_BORDER, y, stream);

				y += 2 * UI_BORDER;
				stream << toPar << " strokes to par";
				drawString(x + UI_BORDER, y, stream);
			} else {
				if (!showWinMessage) {
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glVertex2f(x, y);
					glVertex2f(x, y + HOLE_INFO_HEIGHT_A / 2); 
					glVertex2f(x + HOLE_INFO_WIDTH_A / 2, y + HOLE_INFO_HEIGHT_A / 2); 
					glVertex2f(x + HOLE_INFO_WIDTH_A / 2, y);
					glEnd();

					y += 2 * UI_BORDER;
					stream << "PLAYER " << boccePlayer + 1 << "'s turn";
					drawString(x + UI_BORDER, y, stream);
				}
			}
		}

		// if won, hide UI
		if (showWinMessage) {
			y = viewportHeight - RESULT_HEIGHT;

			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(x, y);
			glVertex2f(x, y + RESULT_HEIGHT - UI_BORDER); 
			glVertex2f(x + RESULT_WIDTH * 1.5, y + RESULT_HEIGHT - UI_BORDER); 
			glVertex2f(x + RESULT_WIDTH * 1.5, y);
			glEnd();

			if (!isBocce) {
				y += 2 * UI_BORDER;
				stream << "Result: " << getScoreString(scores[holeNumber - 1]);
				drawString(x + UI_BORDER, y, stream);

				y += 2 * UI_BORDER;
				stream << "Press 'n' for next level";
				drawString(x + UI_BORDER, y, stream);
			} else {
				y += 2 * UI_BORDER;
				stream <<  "Result: Player " << boccePlayer + 1 << " won with " << bocceScore << " points.";
				drawString(x + UI_BORDER, y, stream);

				y += 2 * UI_BORDER;
				stream << "Press 'r' to play again";
				drawString(x + UI_BORDER, y, stream);
			}
			// else if UI to be shown
		} else if (showShootingUI) {
			x = viewportWidth - SHOOTING_UI_WIDTH;
			y = viewportHeight - SHOOTING_UI_HEIGHT;

			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(x, y);
			glVertex2f(x, y + SHOOTING_UI_HEIGHT - UI_BORDER); 
			glVertex2f(x + SHOOTING_UI_WIDTH - UI_BORDER, y + SHOOTING_UI_HEIGHT - UI_BORDER); 
			glVertex2f(x + SHOOTING_UI_WIDTH - UI_BORDER, y);
			glEnd();

			x += UI_BORDER;
			y += 2 * UI_BORDER;
			stream << "Angle: ";
			drawString(x, y, stream);

			y += SHOOTING_UI_HEIGHT - 5 * UI_BORDER;
			stream << "Power: ";
			drawString(x, y, stream);

			y -= 1.8 * UI_BORDER;
			x += 65;
			glBegin(GL_QUADS);
			glColor3f(0.3, 0.3, 0.3);
			glVertex2f(x, y);
			glVertex2f(x, y + POWER_HEIGHT); 
			glVertex2f(x + POWER_WIDTH, y + POWER_HEIGHT); 
			glVertex2f(x + POWER_WIDTH, y);

			glColor3f(0.8, 0.3, 0.3);
			glVertex2f(x + POWER_BORDER, y + POWER_BORDER);
			glVertex2f(x + POWER_BORDER, y + POWER_HEIGHT - POWER_BORDER); 
			glVertex2f(x + (POWER_WIDTH - POWER_BORDER) * power, y + POWER_HEIGHT - POWER_BORDER); 
			glVertex2f(x + (POWER_WIDTH - POWER_BORDER) * power, y + POWER_BORDER);
			glEnd();

			glPushMatrix();
			glColor3f(0, 0, 0);
			glTranslatef(890, 580, 0);
			glBegin(GL_LINE_STRIP);
			float radius = 100;
			for (int i = 0; i <= 360; i++)
				glVertex2f(sin(i * 2 * PI / 360) * radius, cos(i * 2 * PI / 360) * radius);
			glEnd();

			glPushMatrix();
			glRotatef(-(angle - PI) / PI * 180, 0, 0, 1);
			glBegin(GL_LINE_STRIP);
			glVertex2f(0, 0);
			glVertex2f(0, -radius);
			glVertex2f(10, -radius * 0.9);
			glVertex2f(0, -radius);
			glVertex2f(-10, -radius * 0.9);
			glEnd();
			glPopMatrix();

			glPopMatrix();
		}
	}
}