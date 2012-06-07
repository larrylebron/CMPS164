/**
* CMPS 164 Game Engine
* Larry LeBron and Kwong Fai Jonathan Lew
*/

#include <time.h>
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

#include "level.h"
#include "Course.h"
#include "util.h"
#include "HUD.h"
#include "Player.h"

#include "PhysicsManager.h"

#include "fileReader.h"
#include "externalLibs\Timer.h"
#include "Logger.h"


//timer
Timer frameTimer;
double currTime;

//List of Physics Objects

//The current level
CMMPointer<level> currLev;
CMMPointer<Course> course;
CMMPointer<HUD> hud = new HUD();

//the file reader
fileReader* fR;

//camera modes
typedef enum {
	TRANSLATE,
	ROTATE,
	ZOOM,
	CAMERA_PAN,
	CAMERA_ZOOM,
	CAMERA_ROTATE,
	PLAY_GAME,
	SHOW_PATH,
	NUM_MODES,
} mode;

typedef enum {
	DECREASE = -1,
	INCREASE = 1,
} change;

typedef enum {
	MINIGOLF,
	BOCCE,
} gameType;

//////////////////////
// Struct for color //
//////////////////////

struct Color {
	GLfloat rgb[3];

	Color(){
		rgb[0] = rgb[1] = rgb[2] = 0.0f;
	}

	Color (int R, int G, int B) {
		rgb[0] = R/255.0f;
		rgb[1] = G/255.0f;
		rgb[2] = B/255.0f;
	}
};

///////////////
// VARIABLES //
///////////////

// Mouse Variables
int		btn[2] = {0};			// Current button state
int		mouse_x, mouse_y;			// Current mouse position

// Transformation and Shading Mode Variables
mode	currentMode = PLAY_GAME;	// Current transformation mode
string	instructions[NUM_MODES] =
{"FREELOOK MODEL TRANSLATE MODE: Hold down left mouse button and drag to translate model along X and Y Axis. Press \"W\" and \"S\" to translate model along Z Axis.",
"FREELOOK MODEL ZOOM MODE: Press \"W\" and \"S\" to zoom model in and out - model zooms equally along all axes.",
"FREELOOK MODEL ROTATE MODE: Hold down left mouse button and drag to rotate model around X or Y Axis.",
"FREELOOK CAMERA PAN MODE: Hold down left mouse button and drag to pan camera along X and Y Axis. Press \"W\" and \"S\" to pan camera along Z Axis.",
"FREELOOK CAMERA ZOOM MODE: Press \"W\" and \"S\" to zoom camera in and out. ",
"FREELOOK CAMERA ROTATE X MODE: Hold down left mouse button and drag up and down to rotate camera along X and Y Axes. Press \"W\" and \"S\" to rotate camera along Z Axis.",
"PLAYGAME MODE: \"I\" and \"K\" to increase or decrease power, \"J\" and \"L\" to add anti-clockwise or clockwise angle, \"X\" to shoot.",
"SHOW PATH MODE: Click to stop showing path"};
bool	useKeypressForRotate = !false;
bool	inverseAxis = !false;

// Model Transformation Variables
float	translate[3], rotateM[3];			// Current rotation values
float zoom = 1.0f;

float	cameraZoom, cameraInitialZ;
Vec3f cameraPan, cameraDirection, rotatedCameraDirection, cameraUp, rotatedCameraUp;
float cameraRotate[3];

// Other Fixed Transformation Variables
float rotationDullFactor = 10.0f;
float cameraFOV = 45.0f;
float cameraZNear = 1.0f;
float cameraZFar = 200.f;
float cameraRotationDullFactor = 2.0f;
float cameraTimeStep = 0.000001f;
float cameraCurrent = 0.0f;
Vec3f cameraPath;

// Camera and Light Variables
GLfloat lightColor[]	= {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos[]		= {1.0f, 1.0f, 1.0f, 1.0f}; 

// Viewport Variables
int viewportX = 0, viewportY = 0;

float angle = PI, power = 0.5;

bool addStroke = false;
bool choseGameType = false;
gameType gType = BOCCE;
bool isBocce = (gType == BOCCE);
int currBall = 0;

///////////////////////
// Utility Functions //
///////////////////////

float degToRad(float deg)
{
	return deg / 180.0f * PI;
}

float bringWithinRange (float f, float range = 360.0f)
{
	if (f > range)
		return f - range;
	else if (f < -range)
		return f + range;
	else
		return f;
}

//////////////////////////////
// Camera Related Functions //
//////////////////////////////


void updateCameraDirection() {
	// keep all angles within range to prevent overflow
	cameraRotate[0] = bringWithinRange(cameraRotate[0]);
	cameraRotate[1] = bringWithinRange(cameraRotate[1]);
	cameraRotate[2] = bringWithinRange(cameraRotate[2]);

	float xAngle = degToRad(cameraRotate[0]), yAngle = degToRad(cameraRotate[1]), zAngle = degToRad(cameraRotate[2]);

	// first calculation is made from the neutral cameraDirection
	rotatedCameraDirection = Vec3f(cameraDirection[0] * cos(yAngle) + cameraDirection[2] * sin(yAngle), cameraDirection[1], cameraDirection[2] * cos(yAngle) - cameraDirection[0] * sin(yAngle)); // rotate about y
	rotatedCameraDirection = Vec3f(rotatedCameraDirection[0], rotatedCameraDirection[1] * cos(xAngle) - rotatedCameraDirection[2] * sin(xAngle), rotatedCameraDirection[2] * cos(xAngle) + rotatedCameraDirection[1] * sin(xAngle)); // rotate about x
	rotatedCameraDirection = Vec3f(rotatedCameraDirection[0] * cos(zAngle) - rotatedCameraDirection[1] * sin(zAngle), rotatedCameraDirection[1] * cos(zAngle) + rotatedCameraDirection[0] * sin(zAngle), rotatedCameraDirection[2]); // rotate about z
	rotatedCameraUp = Vec3f(cameraUp[0] * cos(zAngle) - cameraUp[1] * sin(zAngle), cameraUp[1] * cos(zAngle) + cameraUp[0] * sin(zAngle), cameraUp[2]); // cameraUp

	glutPostRedisplay();
}

void updateCamera(int w = viewportWidth, int h = viewportHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cameraFOV * cameraZoom, (double)w / (double)h, cameraZNear, cameraZFar);
	
	Vec3f ballPos = currLev->getBallPos(currBall);
	Vec3f destination = cameraPan + rotatedCameraDirection;
	if (currentMode == PLAY_GAME) {
		gluLookAt(ballPos[0], ballPos[1] + 15.0f, ballPos[2], ballPos[0], ballPos[1], ballPos[2], 0.0f, 0.0f, -1.0f);
	}
	else if (currentMode == SHOW_PATH) {
		Vec3f camPos = ballPos + (cameraPath * cameraCurrent);
		gluLookAt(camPos[0], camPos[1] + 15.0f, camPos[2], camPos[0], camPos[1], camPos[2], 0.0f, 0.0f, -1.0f);

		if (cameraCurrent <= 1.0f){
			cameraCurrent += cameraTimeStep;
		} else {
			//currentMode == PLAY_GAME;
		}
	} else {
		gluLookAt(cameraPan[0], cameraPan[1], cameraPan[2], destination[0], destination[1], destination[2], rotatedCameraUp[0], rotatedCameraUp[1], rotatedCameraUp[2]);
		updateCameraDirection();
	}

}


void resetShooting() {
	if (!KEEP_LAST_SHOOTING_SETTINGS) {
		angle = PI;
		power = 0.5;
	}
}

void resetTransformations() {
	cameraZoom = 1.0f;
	cameraInitialZ = 10.f;
	cameraPan = Vec3f(0.0f, 1.0f, cameraInitialZ);
	rotatedCameraDirection = cameraDirection = Vec3f(0.0f, 0.0f, -cameraInitialZ);
	rotatedCameraUp = cameraUp = Vec3f(0.0f, 1.0f, 0.0f);

	cameraRotate[0] = cameraRotate[1] = cameraRotate[2] = 0.0f;
	
	updateCameraDirection();
}

void switchToOrtho() {
    glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
		glOrtho( 0, viewportWidth , viewportHeight , 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void switchBackToFrustum() {
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/**
Manages animation frame
*/

bool lastStateActive = false;

void new_frame() {
	hud->updateInfo(course, angle, power);
	
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (currentMode != PLAY_GAME && currentMode != SHOW_PATH) {
		glTranslatef(translate[0], translate[1], translate[2]);
		glRotatef(rotateM[0], 1, 0, 0);
		glRotatef(rotateM[1], 0, 1, 0);
		glRotatef(rotateM[2], 0, 0, 1);
		glScalef(zoom, zoom, zoom);
	}
	//check to see if level is complete -- if it is, delete it & load the next or exit
	//main should have a queue of all the levels
	if (currLev.isValid() && currLev->isComplete()) {
		//load next level or exit game here
		
		// display
	}

	//if the current level exists
	if (currLev.isValid()) {
		CMMPointer<ball> aBall = currLev->getBall(currBall);

		//ball is inactive, so show the UI
		if ( !aBall->isActive() ) {
			// show shooting UI if in playgame mode and game not complete
			hud->setShowShootingUI(!currLev->isComplete() && currentMode == PLAY_GAME);

			cout << currLev->getPlayerTurn();
			
			if (gType == BOCCE && !currLev->isComplete() && lastStateActive) {
				
				Vec3f color = (currBall % 2 == 0) ? BALL_P1_COLOR : BALL_P2_COLOR;

				CMMPointer<ball>* tempBall = new CMMPointer<ball>(new ball(0, currLev->getTeePos(), color, BALL_RADIUS));
				currLev->addBall(currBall + 1, tempBall, true);
				currBall++;
			}

			// if returning back to inactive form active	
			if (lastStateActive) {
				// reattach menu
				glutAttachMenu( GLUT_RIGHT_BUTTON );
				
				// if game not won, current stroke
				if (!currLev->isComplete()) {
					currLev->addStroke();
				}
			}
			
		} else {
			// detach menu to prevent menu from screwing up
			glutDetachMenu( GLUT_RIGHT_BUTTON );
			resetShooting();
			hud->setShowShootingUI(false);

		}
		
		lastStateActive = aBall->isActive();

		aBall->doSimulation();
		currLev->update(isBocce);
	}

	switchToOrtho();

	// draw HUD here
	hud->draw(isBocce);

	switchBackToFrustum();

	glFlush();
	glutSwapBuffers();
	IMMObject::CollectGarbage();
}

void cb_idle() {
	double now = frameTimer.getElapsedTime();

	//start a new frame if proper amount of time has elapsed
	if ( (now - currTime) > FRAME_TIME) {
		currTime = now;
		new_frame();
	}

	glutPostRedisplay();
}

void cb_display() {
	if (currentMode == PLAY_GAME) {
		updateCamera();
	} else if (currentMode == SHOW_PATH) {
		updateCamera();
	} else {
		glutPostRedisplay();
	}
}


void cb_mouse( int button, int state, int x, int y )
{

	// Store button state if mouse down
	if (state == GLUT_DOWN) {	
		if (currentMode == SHOW_PATH) {
			currentMode = PLAY_GAME;
			updateCamera();
		}

		btn[button] = 1;
	} else {
		btn[button] = 0;
	}

	mouse_x = x;
	mouse_y = glutGet( GLUT_WINDOW_HEIGHT ) - y;
}	

void cb_motion( int x, int y )
{
	float	 x_ratchet;			// X ratchet value
	float	 y_ratchet;			// Y ratchet value


	if ( !btn[ 0 ] ) {			// Left button not depressed?
		return;
	}

	// scale by z camera distance
	x_ratchet = glutGet(GLUT_WINDOW_WIDTH) / cameraPan[2];
	y_ratchet = glutGet(GLUT_WINDOW_HEIGHT) / cameraPan[2];

	y = glutGet( GLUT_WINDOW_HEIGHT ) - y; // Windows convention has y = 0 at top, GL has y = 0 at bottom, so reverse y

	int axisDirection;

	switch( currentMode ) {
	case TRANSLATE:	// translation
		translate[0] += (float) ( x - mouse_x ) / x_ratchet;
		translate[1] += (float) ( y - mouse_y ) / y_ratchet;
		break;
	case ROTATE: // rotation
		// reverse the axis because its more intuitive
		rotateM [1] += (float) ( x - mouse_x ) / x_ratchet * rotationDullFactor;
		rotateM [0] -= (float) ( y - mouse_y ) / y_ratchet * rotationDullFactor;
		break;
	case ZOOM:
		// does nothing, keyboard only
		break;
	case CAMERA_PAN:
		axisDirection = (inverseAxis) ? -1 : 1;
		cameraPan = cameraPan + (Vec3f((float) ( x - mouse_x ) / x_ratchet, (float) ( y - mouse_y ) / y_ratchet, 0.0f) * axisDirection);
		break;
	case CAMERA_ZOOM:
		// does nothing, keyboard only
		break;
	case CAMERA_ROTATE:
		cameraRotate[0] -= (float) ( y - mouse_y ) / y_ratchet * cameraRotationDullFactor;
		cameraRotate[1] += (float) ( x - mouse_x ) / x_ratchet * cameraRotationDullFactor;
		break;
	case PLAY_GAME: SHOW_PATH:
		// no commands
		break;
	}

	// Update cursor position
	mouse_x = x;				
	mouse_y = y;

	updateCamera();
}	

void handleUpDown(int direction)
{
	switch( currentMode ) {
	case TRANSLATE: // handles Z translation
		translate[2] += 0.1f * direction * cameraPan[2];
		break;
	case ZOOM: // handles zooming
		zoom += 0.05f * direction;
		break;
	case CAMERA_PAN: // handles z panning
		cameraPan = cameraPan + Vec3f(0.0f, 0.0f, 0.5f * direction);
		break;
	case CAMERA_ZOOM: // handles camera zooming
		cameraZoom += 0.05f * direction;
		break;

	case ROTATE:
		if (useKeypressForRotate)
		{
			rotateM [2] += 2.0f * direction;
		}
		break;
	case CAMERA_ROTATE:
		cameraRotate[2] += direction * cameraRotationDullFactor;
		break;
	case PLAY_GAME:
		// up down used to adjust angles
		break;
	case SHOW_PATH:
		// no commands
		break;
	}
}

void resetCameraPath() {
	cameraCurrent = 0.0f;
	Vec3f ballPos = currLev->getBallPos(currBall);
	Vec3f cupPos = currLev->getCupPos();
	cameraPath = cupPos - ballPos;
}

void handle_menu( int ID ) {

	cout << endl << instructions[ID];
	switch( ID ) {
	case 0:	// translation
		currentMode = TRANSLATE;
		break;
	case 1:	// rotation
		currentMode = ROTATE;
		break;
	case 2:	// zooming
		currentMode = ZOOM;
		break;
	case 3: // camera panning
		currentMode = CAMERA_PAN;
		break;
	case 4: // cammera zooming
		currentMode = CAMERA_ZOOM;
		break;
	case 5: // cammera rotate X
		currentMode = CAMERA_ROTATE;
		break;
	case 6:
		currentMode = PLAY_GAME;
		break;
	case 7:
		// calculate path
		resetCameraPath();
		currentMode = SHOW_PATH;
		break;
	case 8: // Quit
		exit(0);
		break;
	}
	
	updateCamera();
	cout << endl;
}		

void cb_keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
		IMMObject::CollectRemainingObjects(); //Memory cleanup
		break;
	case 'w':
		handleUpDown(DECREASE);
		break;
	case 's':
		handleUpDown(INCREASE);
		break;
	case 'r':
		resetTransformations();
		break;
	case 'x':
		if (!currLev->isComplete() && currentMode == PLAY_GAME && !currLev->getBall(currBall)->isActive()){
			currLev->getBall(currBall)->applyForce(Vec3f(angle,power * MAX_POWER));
		}
		break;
	case 'b':
		course->previousLevel();
		currLev = course->getCurrentLevel();
		currLev->resetLevel();
		break;
	case 'n':
		// if (currLev->isComplete())
		course->nextLevel();
		currLev = course->getCurrentLevel();
		currLev->resetLevel();
		break;
	case 'i':
		if (power < 1.0)
			power += 0.05;
		break;
	case 'k':
		if ((power - 0.05) > 0.001)
			power -= 0.05;
		break;
	case 'j':
		angle += PI / 180;
		if (angle > 2 * PI)
			angle -= 2 * PI;
		break;
	case 'l':
		angle -= PI / 180;
		if (angle < 0)
			angle += 2 * PI;
		break;
	}

	updateCamera();
}

void cb_reshape(int w, int h) {
	glViewport(viewportX, viewportY, w, h);
	updateCamera();
}

int main(int argc, char** argv) {
	char mG[] = "minigolf"; //test string for first parameter

	if ( argc != 3 || (strcmp(mG, argv[1]) != 0) ) {
		Logger::Instance()->err("Usage Error: program requires two command line arguments in the form \"minigolf input_filename\"");
		return(1);
	}

	//Initialize level
	currLev = new level();
	course = new Course();

	//Initialize fileReader, read in file, quit if reader fails
	fR = new fileReader();
	if( !fR->readCourseFile(argv[2], course) ) {
		Logger::Instance()->err("course file reader failed");
		return(1);
	}

	currLev = course->getCurrentLevel();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(viewportWidth, viewportHeight);
	glutCreateWindow("Game Engine");

	resetTransformations();

	// Instructions
	cout << "==========================================" << endl;
	cout << "CMPS 164 - Lab 1: Minigolf Level Rendering" << endl;
	cout << "==========================================" << endl << endl;
	cout << "Instructions: " << endl;
	cout << "Use Right Click to Select Modes. Starting Mode: PLAYGAME MODE. Use \"B\" and \"N\" to browse through uncompleted levels." << endl;

	// Right Click Menu
	glutCreateMenu( handle_menu );	// Setup GLUT popup menu
	glutAddMenuEntry( "Free Look - Translate", 0);
	glutAddMenuEntry( "Free Look - Rotate", 1);
	glutAddMenuEntry( "Free Look - Zoom", 2);
	glutAddMenuEntry( "Free Look - Camera Pan", 3);
	glutAddMenuEntry( "Free Look - Camera Zoom", 4);
	glutAddMenuEntry( "Free Look - Camera Rotate", 5);
	glutAddMenuEntry( "Play Game", 6);
	glutAddMenuEntry( "Show Path from Ball to Cup", 7);
	glutAddMenuEntry( "Quit", 8);
	glutAttachMenu( GLUT_RIGHT_BUTTON );
	cout << instructions[currentMode] << endl;

	

	//Callback functions
	glutDisplayFunc(cb_display);
	glutMouseFunc(cb_mouse);
	glutMotionFunc(cb_motion);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);	

	//glutSpecialFunc(specialKeyboard);  

	glClearDepth(1.0);
	glClearColor(0,0,0,0); // set background color
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glShadeModel(GL_SMOOTH);

	//Add positioned light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//initialize random number generator
	srand((unsigned)time(0)); 

	//Start the timer
	frameTimer.start();
	currTime = 0;

	//for debugging
	Logger* log = Logger::Instance();

	glutMainLoop();

	return 0;
}
