/**
* CMPS 164 Game Engine
* Larry LeBron and Kwong Fai Jonathan Lew
*/

#include <time.h>
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glut.h>

#include "util.h"
#include "level.h"

int width = 1000;
int height = 1000;

//timer
double currTime; 

//perspective variables
int fov = 45;
double nearPlane = .1;
double farPlane = 10000;

/**
 Manages animation frame
 */
void new_frame() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glutSwapBuffers();
}

void cb_idle() {
	double now = clock() / (CLOCKS_PER_SEC / 1000);

	//update at 60 frames/sec
	if ( (now - currTime) > 16) {
		currTime = now;
		new_frame();
	}

	glutPostRedisplay();
}

void cb_display() {
	glutPostRedisplay();
}

void cb_mouse(int key, int state, int x, int y) {
}

void cb_motion(int x, int y) {
}

void updatePerspective (){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (double)width / (double)height, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void cb_keyboard(unsigned char key, int x, int y) {

	switch(key) {
		case 'q':
			exit(0);
			break;
	}
}

void cb_reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	width = w;
	height = h;
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Game Engine");

	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);
	//glutSpecialFunc(specialKeyboard);
	glutMouseFunc(cb_mouse);
	glutMotionFunc(cb_motion);
	glClearColor(0,0,0,0); // set background color
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glEnable( GL_TEXTURE_2D );
	
	//float ambient[4] = {0.5,0.5,0.5,1};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	//light1 = new Lighting(GL_LIGHT0, 0, 500, 0, 0);
	//light2 = new Lighting(GL_LIGHT1, 0, -500, 0, 0);

	//cam = new Camera(ent->getPos(), initCam);
	
	//initialize random number generator and timer
	srand((unsigned)time(0)); 
	currTime = clock() / (CLOCKS_PER_SEC / 1000);
	
	glutMainLoop();
	IMMObject::CollectRemainingObjects(); //not sure if this will kick in after glut quits

	return 0;
}






