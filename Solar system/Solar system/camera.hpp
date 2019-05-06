#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Camera {
private:
	float forwardVector[3];
	float rightVector[3];
	float upVector[3];
	float position[3];
	float cameraSpeed;
	float cameraTurnSpeed;

public:
	Camera(void);
	void transformOrientation(void);
	void transformTranslation(void);
	void pointAt(float *targetVector);
	void moveForward(void);
	void moveBackward(void);
	void moveLeft(void);
	void moveRight(void);
	void increaseSpeed(void);
	void decreaseSpeed(void);
};

void addVector(float *v1, float *v2) {
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

void mulVector(float *v, float val) {
	v[0] *= val;
	v[1] *= val;
	v[2] *= val;
}

void copyVector(float *v1, float *v2) {
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

void initializeVector(float *v, float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

float lengthOfVector(float *v) {
	return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void normaliseVector(float *v) {
	mulVector(v, 1 / lengthOfVector(v));
}

void matrixMulVector(float *matrix, float *v1, float *v2) {
	v2[0] = matrix[0] * v1[0] + matrix[1] * v1[1] + matrix[2] * v1[2];
	v2[1] = matrix[3] * v1[0] + matrix[4] * v1[1] + matrix[5] * v1[2];
	v2[2] = matrix[6] * v1[0] + matrix[7] * v1[1] + matrix[8] * v1[2];
}

void rotationMatrix(float* matrix, float* axis, float angle) {
	float cos1 = cos(angle);
	float cos2 = 1 - cos1;
	float sin1 = sin(angle);
	matrix[0] = cos1 + axis[0]*axis[0]*cos2;
	matrix[1] = axis[0] * axis[1] * cos2 - axis[2]*sin1;
	matrix[2] = axis[0] * axis[2] * cos2 + axis[1]*sin1;
	matrix[3] = axis[1] * axis[0] * cos2 + axis[2]*sin1;
	matrix[4] = cos1 + axis[1] * axis[1] * cos2;
	matrix[5] = axis[1] * axis[2] * cos2 - axis[0] * sin1;
	matrix[6] = axis[2] * axis[0] * cos2 - axis[1] * sin1;
	matrix[7] = axis[2] * axis[1] * cos2 + axis[0] * sin1;
	matrix[8] = cos1 + axis[2] * axis[2] * cos2;
}

void rotateVector(float *v, float *axis, float angle, float *v1) {
	float matrix[9];
	rotationMatrix(matrix, axis, angle);
	matrixMulVector(matrix, v, v1);
}

Camera::Camera(void) {
	cameraSpeed = 0.005f;
	cameraTurnSpeed = 0.01f;
	initializeVector(position, 0.764331460f, -1.66760659f, 0.642456770);
	initializeVector(forwardVector, -0.398769796f, 0.763009906f, -0.508720219f);
	initializeVector(rightVector, 0.886262059f, 0.463184059f, 0.000000000f);
	initializeVector(upVector, -0.235630989f, 0.450859368f, 0.860931039f);
}

void Camera::increaseSpeed(void) {
	if (cameraSpeed < 1.0f) {
		cameraSpeed *= 2;
	}
}

void Camera::decreaseSpeed(void) {
	if (cameraSpeed > 0.000001f) {
		cameraSpeed /= 2;
	}
}

void Camera::pointAt(float *targetVector) {
	float v[3];
	float zaxis[3] = { 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < 3; i++) {
		v[i] = targetVector[i] - position[i];
	}
	normaliseVector(v);
	copyVector(forwardVector, v);
	rotateVector(forwardVector, zaxis, -1.57079632679f, v);
	v[2] = 0.0f;
	normaliseVector(v);
	copyVector(rightVector, v);
	rotateVector(forwardVector, rightVector, 1.57079632679f, v);
	copyVector(upVector, v);
}

void Camera::moveForward(void) {
	float v[3];
	copyVector(v, forwardVector);
	mulVector(v, cameraSpeed);
	addVector(position, v);
}

void Camera::moveRight(void) {
	float v[3];
	copyVector(v, rightVector);
	mulVector(v, cameraSpeed);
	addVector(position, v);
}

void Camera::moveBackward(void) {
	float v[3];
	copyVector(v, forwardVector);
	mulVector(v, -cameraSpeed);
	addVector(position, v);
}

void Camera::moveLeft(void) {
	float v[3];
	copyVector(v, rightVector);
	mulVector(v, -cameraSpeed);
	addVector(position, v);
}

void Camera::transformOrientation(void) {
	gluLookAt(0.0f, 0.0f, 0.0f, forwardVector[0], forwardVector[1], forwardVector[2], upVector[0], upVector[1], upVector[2]);
}

void Camera::transformTranslation(void) {
	glTranslatef(-position[0], -position[1], -position[2]);
}
