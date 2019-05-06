#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "globals.h"

class Moon {
private:
	float distanceFromPlanet;
	float radius;
	float orbitTime;
	float rotationTime;
	float position[3];
	GLuint textureHandle;
	float rotation;

public:
	Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void render(void);
	void renderOrbit(void);
	void calculatePosition(float time);
};

Moon::Moon(float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle) {
	this->distanceFromPlanet = distanceFromPlanet;
	this->orbitTime = orbitTime;
	this->rotationTime = rotationTime;
	this->radius = radius;
	this->textureHandle = textureHandle;
}

void Moon::calculatePosition(float time) {
	float angle = time * 3.1419f / orbitTime;
	position[0] = sin(angle) * distanceFromPlanet;
	position[1] = cos(angle) * distanceFromPlanet;
	position[2] = 0.0f;
	rotation = time * 360 / rotationTime;
}

void Moon::render(void) {
	glPushMatrix();
	glTranslatef(position[0] * distanceScale, position[1] * distanceScale, 0.0f);
	glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, radius*planetSizeScale, 30, 30);
	glPopMatrix();
}

void Moon::renderOrbit(void) {
	glBegin(GL_LINE_STRIP);
	for (float angle = 0.0f; angle < 6.283185307f; angle += 0.1f){
		glVertex3f(sin(angle) * distanceFromPlanet * distanceScale, cos(angle) * distanceFromPlanet * distanceScale, 0.0f);
	}
	glVertex3f(0.0f, distanceFromPlanet * distanceScale, 0.0f);
	glEnd();
}




