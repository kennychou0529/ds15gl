#include "dsTools.h"
#include "dsFrame.h"
#include <ctime>
#include <iostream>

//Ŀǰֻ֧�� WIN32 �� Linux
#ifdef WIN32
  #define SLEEP(mm) Sleep(mm)
#else
  //�������֪���в���
  #include <unistd.h>
  #define SLEEP(mm) usleep(mm*1000)
#endif // WIN32

//dsScene scene;
DSFrame frame;

// ÿ֡ʱ�䣬����
static const int mspf = 33;

void dsDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	dsSet(); // �����ӽ�

	frame.display();

	glutSwapBuffers();

	// ��ӡ GL ����
	GLenum errCode;
	const GLubyte* errString;

	while ((errCode = glGetError()) != GL_NO_ERROR){
		errString = gluErrorString(errCode);
		std::cerr << errString << std::endl;
	}

}

void dsInit() {
	dsSetMaterial();
	dsSetLight();
	frame.initialize();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

// �����ڴ�С���޸�ʱ�Զ����ô˺���
void dsReshape(int w, int h) {
	window_width = w;
	window_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (window_width - status_bar_width) / double(window_height), 2, 20000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void dsIdle() {
	static long t = clock();
	long deltaT = clock() - t;
	if (deltaT > 0 && deltaT < mspf)
		// �������߳�
		SLEEP(mspf - deltaT);
	t = clock();

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("DS 15th");
	glutDisplayFunc(dsDisplay);
	// glutMouseFunc(dsMouseFunc);
	// glutPassiveMotionFunc(dsPassiveMonitionFunc);
	glutSpecialFunc(dsSpecialKeys);
	glutReshapeFunc(dsReshape);
	glutIdleFunc(dsIdle);
	glutKeyboardFunc(dsKeys);
	dsInit();
	glutMainLoop();
	return 0;
}
