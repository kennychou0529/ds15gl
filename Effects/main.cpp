#include <GL/glut.h>
#include <iostream>
#include "particle.h"

using namespace std;
Emitter em;

void dsDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0,0,50,0,0,0,0,1,0);

	em.draw();

	glutSwapBuffers();

	// ��ӡ GL ����
	GLenum errCode;
	const GLubyte* errString;

	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << errString << std::endl;
	}
}



// �����ڴ�С���޸�ʱ�Զ����ô˺���
void dsReshape(int w, int h) {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 4/3.0f, 2, 200);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void dsIdle() {
	glutPostRedisplay();
}

int main(int argc,char** argv){
	glutInit(&argc,argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("DS 15th Effect Test");
	glutDisplayFunc(dsDisplay);
	
	glutReshapeFunc(dsReshape);
	glutIdleFunc(dsIdle);
	
	glutMainLoop();
	
	return 0;
}