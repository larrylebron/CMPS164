#include "HUD.h"



HUD::HUD(void)
{
	showScore = false;
	showHoleInfo = true;
	showShootingUI = false;
	showWinMessage = false;
	init = false;

	holeNumber = 1;
	holeName = "";
	toPar = 1;
	stroke = 1;
	angle = PI;
	power = 0.5;
}


HUD::~HUD(void)
{
}

void HUD::updateInfo(CMMPointer<Course> course, float gAngle, float gPower)
{
	if (!init)
	{
		scores = new int[course->getNumLevels()];

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

string getScoreString (int score) {
	std::ostringstream stream;
	if (score > 0)
	{
		stream << "+" << score;
	}
	else if (score < 0)
	{
		stream << score;
	}

	return stream.str();
}

void HUD::draw() {
	// if showing score screen, only draw that
	float x, y;
	x = y = UI_BORDER;
	std::ostringstream stream;
	string str;
	char *print;
			

	if (showScore) {
		// KIV
	} else {
		if (showHoleInfo) {
			
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
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x + UI_BORDER, y, print);
			
			stream.clear();
			stream.str("");
			stream << "Stroke: " << stroke;
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(viewportWidth - x - HOLE_INFO_WIDTH_B + UI_BORDER, y, print);

			y += 2 * UI_BORDER;
			stream.clear();
			stream.str("");
			stream << holeName;
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x + UI_BORDER, y, print);
			
			y += 2 * UI_BORDER;
			stream.clear();
			stream.str("");
			stream << toPar << " strokes to par";
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x + UI_BORDER, y, print);
		}

		// if won, hide UI
		if (showWinMessage) {
			y = viewportHeight - RESULT_HEIGHT;
			
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(x, y);
			glVertex2f(x, y + RESULT_HEIGHT - UI_BORDER); 
			glVertex2f(x + RESULT_WIDTH, y + RESULT_HEIGHT - UI_BORDER); 
			glVertex2f(x + RESULT_WIDTH, y);
			glEnd();


			y += 2 * UI_BORDER;
			stream.clear();
			stream.str("");
			stream << "Result: " << getScoreString(scores[holeNumber - 1]);
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x + UI_BORDER, y, print);

			y += 2 * UI_BORDER;
			stream.clear();
			stream.str("");
			stream << "Press 'n' for next level";
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x + UI_BORDER, y, print);

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
			stream.clear();
			stream.str("");
			stream << "Angle: ";
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x, y, print);

			y += SHOOTING_UI_HEIGHT - 5 * UI_BORDER;
			stream.clear();
			stream.str("");
			stream << "Power: ";
			str = stream.str();
			print = (char *)str.c_str();
			draw_string(x, y, print);

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