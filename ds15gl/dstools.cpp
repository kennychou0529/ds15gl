#include "dstools.h"

GLdouble eye_sphere[3] = {20.0, 0.01, 0.01};

void dsSet() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);
	int window_width = glutGet(GLUT_WINDOW_WIDTH);
	gluPerspective(60, (double)window_width / window_height, 0.01, 20000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLdouble eye[3];
	dsSphereToOrtho3dv(eye_sphere, eye);
	GLdouble up[3] = {0.0, 0.0, 1.0};
	gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, up[0], up[1], up[2]);
}

void dsSpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		if (eye_sphere[1] > 0.1) {
			eye_sphere[1] -= 0.1;
		}
		break;
	case GLUT_KEY_DOWN:
		if (eye_sphere[1] < 3.1415926 - 0.1) {
			eye_sphere[1] += 0.1;
		}
		break;
	case GLUT_KEY_LEFT:
		eye_sphere[2] -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		eye_sphere[2] += 0.1;
		break;
	case GLUT_KEY_F1:
		eye_sphere[0] += 0.1;
		break;
	case GLUT_KEY_F2:
		eye_sphere[0] -= 0.1;
		break;
	}
}