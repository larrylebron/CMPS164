#include "renderManager.h"

renderManager* renderManager::m_pInstance = 0;  

renderManager::renderManager() {
}

/** This function is called to create an instance of the class. 
    Calling the constructor publicly is not allowed. The constructor 
    is private and is only called by this Instance function.
*/  
renderManager* renderManager::Instance()
{
   if (!m_pInstance) {   // Only allow one instance of class to be generated.
      m_pInstance = new renderManager();
   }

   return m_pInstance;
}


renderManager::~renderManager()
{
	cout << "render manager destructor";
}

void renderManager::drawPolygon(vector<Vec3f> vertices, Vec3f normal, Vec3f color) {
	
	glBegin(GL_POLYGON);

	glColor3f(color[0], color[1], color[2]);
	glNormal3f(normal[0], normal[1], normal[2]);
	for (vector<Vec3f>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		Vec3f v = *it;
		glVertex3f(v[0], v[1], v[2]);
	}
	
	glEnd();
}

void renderManager::drawSphere(float radius, Vec3f normal, Vec3f position, Vec3f color) {
	
	GLfloat sphereColor[3] = { color[0], color[1], color[2] };
	glColor3fv(sphereColor);
	glNormal3f(normal[0], normal[1], normal[2]);

	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glutSolidSphere(radius, 10, 10); //our default params for sphere quality
	glPopMatrix();
}

