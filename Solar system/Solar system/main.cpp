#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "texture.h"
#include "solarsystem.hpp"
#include "camera.hpp"

int width, height;
TGA *stars, *moon, *mercuryinfo, *venusinfo, *earthinfo, *marsinfo, *jupiterinfo, *saturninfo, *uranusinfo, *neptuneinfo, *plutoinfo;
bool showOrbits = true;
int planetSelected = 0;

SolarSystem solarsystem;
Camera camera;

double time, timeSpeed;

struct ControlStates {
	bool moveForward, moveBackward, moveLeft, moveRight;
} controls;

void timer(int){
	glutPostRedisplay(); 
	glutTimerFunc(10, timer, 0);
}

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	char starsImage[] = "images/stars.tga";
	char moonImage[] = "images/moon.tga";
	char sunImage[] = "images/sun.tga";
	char mercuryImage[] = "images/mercury.tga";
	char venusImage[] = "images/venus.tga";
	char earthImage[] = "images/earth.tga";
	char marsImage[] = "images/mars.tga";
	char jupiterImage[] = "images/jupiter.tga";
	char saturnImage[] = "images/saturn.tga";
	char uranusImage[] = "images/uranus.tga";
	char neptuneImage[] = "images/neptune.tga";
	char plutoImage[] = "images/pluto.tga";
	char mercuryInfo[] = "imageinfo/mercury.tga";
	char venusInfo[] = "imageinfo/venus.tga";
	char earthInfo[] = "imageinfo/earth.tga";
	char marsInfo[] = "imageinfo/mars.tga";
	char jupiterInfo[] = "imageinfo/jupiter.tga";
	char saturnInfo[] = "imageinfo/saturn.tga";
	char uranusInfo[] = "imageinfo/uranus.tga";
	char neptuneInfo[] = "imageinfo/neptune.tga";
	char plutoInfo[] = "imageinfo/pluto.tga";
	stars = new TGA(starsImage);
	moon = new TGA(moonImage);
	TGA *sun = new TGA(sunImage);
	TGA* mercury = new TGA(mercuryImage);
	TGA* venus = new TGA(venusImage);
	TGA* earth = new TGA(earthImage);
	TGA* mars = new TGA(marsImage);
	TGA* jupiter = new TGA(jupiterImage);
	TGA* saturn = new TGA(saturnImage);
	TGA* uranus = new TGA(uranusImage);
	TGA* neptune = new TGA(neptuneImage);
	TGA* pluto = new TGA(plutoImage);
	mercuryinfo = new TGA(mercuryInfo);
	venusinfo = new TGA(venusInfo);
	earthinfo = new TGA(earthInfo);
	marsinfo = new TGA(marsInfo);
	jupiterinfo = new TGA(jupiterInfo);
	saturninfo = new TGA(saturnInfo);
	uranusinfo = new TGA(uranusInfo);
	neptuneinfo = new TGA(neptuneInfo);
	plutoinfo = new TGA(plutoInfo);
	solarsystem.addPlanet(0, 1, 500, 695500, sun->getTextureHandle(), 0); // sun
	solarsystem.addPlanet(57910000, 88, 58.6, 2440, mercury->getTextureHandle(), 1); // mercury
	solarsystem.addPlanet(108200000, 224.65, 243, 6052, venus->getTextureHandle(), 2); // venus
	solarsystem.addPlanet(149600000, 365, 1, 6371, earth->getTextureHandle(), 3); // earth
	solarsystem.addPlanet(227939100, 686, 1.03f, 3389, mars->getTextureHandle(), 4); // mars
	solarsystem.addPlanet(778500000, 4332, 0.4139, 69911, jupiter->getTextureHandle(), 5); // jupiter
	solarsystem.addPlanet(1433000000, 10759, 0.44375, 58232, saturn->getTextureHandle(), 6); // saturn
	solarsystem.addPlanet(2877000000, 30685, 0.718056, 25362, uranus->getTextureHandle(), 7); // uranus
	solarsystem.addPlanet(4503000000, 60188, 0.6713, 24622, neptune->getTextureHandle(), 8); // neptune
	solarsystem.addPlanet(5906380000, 90616, 6.39, 1137, pluto->getTextureHandle(), 9); // pluto

	solarsystem.addMoon(3, 7000000, 27.3, 27.3, 1738, moon->getTextureHandle());
	solarsystem.addMoon(2, 7000000, 27.3, 27.3, 1738, moon->getTextureHandle());

	time = 2.552f;
	timeSpeed = 0.1f;

	controls.moveForward = false;
	controls.moveBackward = false;
	controls.moveLeft = false;
	controls.moveRight = false;

	timer(0);
}

void drawCube(void){
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
}


void display(void){
	time += timeSpeed;
	solarsystem.calculatePositions(time);

	if (controls.moveForward) {camera.moveForward();}		
	if (controls.moveBackward) {camera.moveBackward();}
	if (controls.moveLeft) {camera.moveLeft();}
	if (controls.moveRight) {camera.moveRight();}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, (float)width / (float)height, 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.transformOrientation();

	glBindTexture(GL_TEXTURE_2D, stars->getTextureHandle());
	drawCube();

	camera.transformTranslation();



	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	solarsystem.render();
	glDisable(GL_LIGHTING);

	if (showOrbits)
		solarsystem.renderOrbits();

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 1){
		glBindTexture(GL_TEXTURE_2D, mercuryinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	if (planetSelected == 2){
		glBindTexture(GL_TEXTURE_2D, venusinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 3){
		glBindTexture(GL_TEXTURE_2D, earthinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 4){
		glBindTexture(GL_TEXTURE_2D, marsinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 5){
		glBindTexture(GL_TEXTURE_2D, jupiterinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 6){
		glBindTexture(GL_TEXTURE_2D, saturninfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 7){
		glBindTexture(GL_TEXTURE_2D, uranusinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 8){
		glBindTexture(GL_TEXTURE_2D, neptuneinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) width, (GLdouble) height, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (planetSelected == 9){
		glBindTexture(GL_TEXTURE_2D, plutoinfo->getTextureHandle());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(512.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(512.0f, 512.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, 512.0f);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) {
	if (key > '0' && key <= '9') {
		float v[3];
		solarsystem.getPlanetPosition(key - '0', v);
		camera.pointAt(v);
		planetSelected = key - '0';
	}
	switch (key) {
	case 'w':
		camera.increaseSpeed();
		break;
	case 's':
		camera.decreaseSpeed();
		break;
	case 'f':
		controls.moveForward = true;
		break;
	case 'b':
		controls.moveBackward = true;
		break;
	case 'l':
		controls.moveLeft = true;
		break;
	case 'r':
		controls.moveRight = true;
		break;
	case 'i':
		timeSpeed *= 2;
		break;
	case 'd':
		timeSpeed /= 2;
		break;
	case 'o':
		if(showOrbits == true){
			showOrbits = false;
		}
		else{
			showOrbits = true;
		}
		break;
	case 'x':
		planetSelected = 0;
		break;
	default:
		break;
	}
}

void keyUp(unsigned char key, int x, int y) {
	switch (key){
	case 'f':
		controls.moveForward = false;
		break;
	case 'b':
		controls.moveBackward = false;
		break;
	case 'l':
		controls.moveLeft = false;
		break;
	case 'r':
		controls.moveRight = false;
		break;
	case 'x':
		planetSelected = 0;
		break;
	default:
		break;
	}
}

void MouseClick(int button, int state, int x, int y){
	y = height - y;
	float x1 = x/width;
	float y1 = y/height;
	float vec1[3], vec2[3], vec3[3], vec4[3], vec5[3], vec6[3], vec7[3], vec8[3], vec9[3];
	solarsystem.getPlanetPosition(1, vec1);
	solarsystem.getPlanetPosition(2, vec2);
	solarsystem.getPlanetPosition(3, vec3);
	solarsystem.getPlanetPosition(4, vec4);
	solarsystem.getPlanetPosition(5, vec5);
	solarsystem.getPlanetPosition(6, vec6);
	solarsystem.getPlanetPosition(7, vec7);
	solarsystem.getPlanetPosition(8, vec8);
	solarsystem.getPlanetPosition(9, vec9);
	vec1[0] = (vec1[0]+1)/2; vec1[1] = (vec1[1]+1)/2;
	vec2[0] = (vec2[0]+1)/2; vec2[1] = (vec2[1]+1)/2;
	vec3[0] = (vec3[0]+1)/2; vec3[1] = (vec3[1]+1)/2;
	vec4[0] = (vec4[0]+1)/2; vec4[1] = (vec4[1]+1)/2;
	vec5[0] = (vec5[0]+1)/2; vec5[1] = (vec5[1]+1)/2;
	vec6[0] = (vec6[0]+1)/2; vec6[1] = (vec6[1]+1)/2;
	vec7[0] = (vec7[0]+1)/2; vec7[1] = (vec7[1]+1)/2;
	vec8[0] = (vec8[0]+1)/2; vec8[1] = (vec8[1]+1)/2;
	vec9[0] = (vec9[0]+1)/2; vec9[1] = (vec9[1]+1)/2;
	float radius1 = solarsystem.getRadiusOfPlanet(1)  * 0.000099;
	float radius2 = solarsystem.getRadiusOfPlanet(2)  * 0.000099;
	float radius3 = solarsystem.getRadiusOfPlanet(3)  * 0.000099;
	float radius4 = solarsystem.getRadiusOfPlanet(4)  * 0.000099;
	float radius5 = solarsystem.getRadiusOfPlanet(5)  * 0.000099;
	float radius6 = solarsystem.getRadiusOfPlanet(6)  * 0.000099;
	float radius7 = solarsystem.getRadiusOfPlanet(7)  * 0.000099;
	float radius8 = solarsystem.getRadiusOfPlanet(8)  * 0.000099;
	float radius9 = solarsystem.getRadiusOfPlanet(9)  * 0.000099;
	switch (button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_UP){
				if (sqrtf(powf(vec1[0],2)+powf(vec1[1],2)) <= radius1){
					planetSelected = 1;
					printf("1 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec2[0],2)+powf(vec2[1],2)) <= radius2){
					planetSelected = 2;
					printf("2 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec3[0],2)+powf(vec3[1],2)) <= radius3){
					planetSelected = 3;
					printf("3 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec4[0],2)+powf(vec4[1],2)) <= radius4){
					planetSelected = 4;
					printf("4 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec5[0],2)+powf(vec5[1],2)) <= radius5){
					planetSelected = 5;
					printf("5 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec6[0],2)+powf(vec6[1],2)) <= radius6){ 
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec7[0],2)+powf(vec7[1],2)) <= radius7){
					planetSelected = 7;
					printf("7 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec8[0],2)+powf(vec8[1],2)) <= radius8){
					planetSelected = 8;
					printf("8 \n");
					glutPostRedisplay();
				}
				else if (sqrtf(powf(vec9[0],2)+powf(vec9[1],2)) <= radius9){
					planetSelected = 9;
					printf("9 \n");
					glutPostRedisplay();
				}
				else{
					planetSelected = 0;
				}
			}
		break;
	}
}


void reshape(int w, int h){
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(MouseClick);
	glutMainLoop();
	return 0;
}