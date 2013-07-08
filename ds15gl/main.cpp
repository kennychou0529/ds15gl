#include "dstools.h"
#include "dsFrame.h"
#include <ctime>
#include <iostream>

//目前只支持WIN32和Linux
#ifdef WIN32
#define SLEEP(mm) Sleep(mm)
#else
//这个还不知道行不行
#include <unistd.h>
#define SLEEP(mm) usleep(mm*1000)
#endif // WIN32


int width = 800, height = 600;
//dsScene scene;
DSFrame frame;
const int mspf = 33; //每帧时间

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	dsSet(); // 设置视角

	frame.display();

	glutSwapBuffers();

	//打印GL错误
	GLenum errCode;
	const GLubyte *errString;

	while ((errCode = glGetError()) != GL_NO_ERROR){
		errString = gluErrorString(errCode);
		std::cerr << errString << std::endl;
	}

}

void init() {
	//scene.initScene();
	dsSetMaterial();
	dsSetLight();
	frame.initialize();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//dsSet();
	// texGround = dsLoadTextureBMP2D("ground.bmp");
}

// 当窗口大小被修改时自动调用此函数
void reshapeFunc(int w, int h) {
	width = w;
	height = h;
	frame.setSize(w,h);
//	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (width - 200) / double(height), 2, 20000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void idle() {
	static long t = clock();
	long deltaT = clock() - t;
	if(deltaT > 0 && deltaT < mspf)
		//阻塞该线程
		SLEEP(mspf - deltaT);
	t = clock();

	glutPostRedisplay();
	
}

int main(int argc, char* argv[])
{
	vector2f v(1,2);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800,600);
	glutCreateWindow("DS15");
	glutDisplayFunc(display);
	//glutMouseFunc(dsMouseFunc);
	//glutPassiveMotionFunc(dsPassiveMonitionFunc);
	glutSpecialFunc(dsSpecialKeys);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyFunc);
	init();
	glutMainLoop();
	return 0;
}