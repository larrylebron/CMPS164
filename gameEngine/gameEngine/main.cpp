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
#include "util.h"
#include "renderManager.h"


#include "PhysicsManager.h"

#include "fileReader.h"
#include "externalLibs\Timer.h"
#include "Logger.h"

//Frame length in seconds -- for 60 fps
const double FRAME_TIME = .0166666666666666666666;

//timer
Timer frameTimer;
double currTime;

//The Physics manager
PhysicsManager* pM;

//List of Physics Objects

//The render manager
renderManager* rM;

//The current level
CMMPointer<level> currLev;

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
mode	currentMode = CAMERA_PAN;	// Current transformation mode
string	instructions[NUM_MODES] =
{"CAMERA PAN MODE: Hold down left mouse button and drag to pan camera along X and Y Axis. Press \"W\" and \"S\" to pan camera along Z Axis.",
"CAMERA ZOOM MODE: Press \"W\" and \"S\" to zoom camera in and out. ",
"CAMERA ROTATE X MODE: Hold down left mouse button and drag up and down to rotate camera along X and Y Axes. Press \"W\" and \"S\" to rotate camera along Z Axis."};
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
float cameraTimeStep = 0.00001f;
float cameraCurrent = 0.0f;
Vec3f cameraPath;

// Camera and Light Variables
GLfloat lightColor[]	= {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPos[]		= {1.0f, 1.0f, 1.0f, 1.0f}; 

// Viewport Variables
int viewportX = 0, viewportY = 0, viewportWidth = 1024, viewportHeight = 768;

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

void resetTransformations() {
	cameraZoom = 1.0f;
	cameraInitialZ = 10.f;
	cameraPan = Vec3f(0.0f, 1.0f, cameraInitialZ);
	rotatedCameraDirection = cameraDirection = Vec3f(0.0f, 0.0f, -cameraInitialZ);
	rotatedCameraUp = cameraUp = Vec3f(0.0f, 1.0f, 0.0f);

	cameraRotate[0] = cameraRotate[1] = cameraRotate[2] = 0.0f;
}

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
}

void updateCamera(int w = viewportWidth, int h = viewportHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cameraFOV * cameraZoom, (double)w / (double)h, cameraZNear, cameraZFar);

	// only in camera rotate mode, the rotatedCameraDirection would have changed
	if (currentMode == CAMERA_ROTATE) {
		updateCameraDirection();
	}

	std::map<int, CMMPointer<ball>> ballMap = currLev->getBalls();
	Vec3f ballPos = ballMap.begin()->second->getPosition();
	Vec3f destination = cameraPan + rotatedCameraDirection;
	if (currentMode == PLAY_GAME) {
		gluLookAt(ballPos[0], ballPos[1] + 5.0f, ballPos[2], ballPos[0], ballPos[1], ballPos[2], 0.0f, 0.0f, -1.0f);
	}
	else if (currentMode == SHOW_PATH) {
		Vec3f camPos = ballPos + (cameraPath * cameraCurrent);
		gluLookAt(camPos[0], camPos[1] + 5.0f, camPos[2], camPos[0], camPos[1], camPos[2], 0.0f, 0.0f, -1.0f);
		
		if (cameraCurrent <= 1.0f){
			cameraCurrent += cameraTimeStep;
		} else {
			//currentMode == PLAY_GAME;
		}
	} else {
		gluLookAt(cameraPan[0], cameraPan[1], cameraPan[2], destination[0], destination[1], destination[2], rotatedCameraUp[0], rotatedCameraUp[1], rotatedCameraUp[2]);
	}
	
	glutPostRedisplay();
}

/**
 Manages animation frame
 */
void new_frame() {

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
		
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotateM[0], 1, 0, 0);
	glRotatef(rotateM[1], 0, 1, 0);
	glRotatef(rotateM[2], 0, 0, 1);
	glScalef(zoom, zoom, zoom);

	//process ui -- any new impulse forces?

	//update simulation

	rM->drawLevel(currLev);
	glPopMatrix();
	
	
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
		std::map<int, CMMPointer<ball>> ballMap = currLev->getBalls();
		Vec3f ballCurrPos = ballMap.begin()->second->getPosition();
		Vec3f newPos = ballCurrPos + Vec3f(0.00001f, 0.0f, -0.00001f);
		ballMap.begin()->second->setPosition(newPos);	
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
	std::map<int, CMMPointer<ball>> ballMap = currLev->getBalls();
	Vec3f ballPos = ballMap.begin()->second->getPosition();
	std::map<int, CMMPointer<cup>> cupMap = currLev->getCups();
	Vec3f cupPos = cupMap.begin()->second->getPosition();
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

	//Initialize fileReader, read in file, quit if reader fails
	fR = new fileReader();
	if( !fR->readFile("hole.01.db", currLev) ) {
		Logger::Instance()->err("file reader failed");
		return(1);
	}

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
	cout << "Use Right Click to Select Modes. Starting Mode: CAMERA PAN." << endl;

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

	//initialize render Manager
	rM = renderManager::Instance();

	pM = PhysicsManager::Instance();

	//Start the timer
	frameTimer.start();
	currTime = 0;

	//for debugging
	Logger* log = Logger::Instance();
	
	glutMainLoop();

	return 0;
}






