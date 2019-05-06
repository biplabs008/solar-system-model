#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include "planets.hpp"

class SolarSystem {
private:
	std::vector<Planet> planets;

public:
	SolarSystem();
	void addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle, int index);
	void addMoon(int index, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle);
	void calculatePositions(float time);
	void render(void);
	void renderOrbits(void);
	float getRadiusOfPlanet(int index);
	void getPlanetPosition(int index, float *vec);
};
SolarSystem::SolarSystem() {

}

void SolarSystem::addPlanet(float distanceFromSun, float orbitTime, float rotationTime, float radius, GLuint textureHandle, int index) {
	planets.push_back(Planet(distanceFromSun, orbitTime, rotationTime, radius, textureHandle, index));
}

void SolarSystem::addMoon(int index, float distanceFromPlanet, float orbitTime, float rotationTime, float radius, GLuint textureHandle) {
	planets[index].addMoon(distanceFromPlanet, orbitTime, rotationTime, radius, textureHandle);
}

void SolarSystem::calculatePositions(float time) {
	int i = 0;
	for (i = 0; i < planets.size(); i++) {
		planets[i].calculatePosition(time);
	}
}

void SolarSystem::render(void) {
	int i = 0;
	for (i = 0; i < planets.size(); i++) {
		planets[i].render();
	}
}

void SolarSystem::renderOrbits(void) {
	int i = 0;
	for (i = 0; i < planets.size(); i++) {
		planets[i].renderOrbit();
	}
}

void SolarSystem::getPlanetPosition(int index, float *vec) {
	planets[index].getPosition(vec);
}

float SolarSystem::getRadiusOfPlanet(int index) {
	return planets[index].getRadius();
}

