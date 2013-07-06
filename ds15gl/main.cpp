#include "dstools.h"
#include "dsFrame.h"
#include <ctime>

int width = 800, height = 600;
//dsScene scene;
DSFrame frame;
const int mspf=33; //每帧时间

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	dsSet(); // 这个函数需要修改	

	//dsShowAxes();
	//scene.show();
	frame.display();
	
	
	/*glDisable(GL_TEXTURE_2D);
	glutSolidTeapot(5.0);
	glEnable(GL_TEXTURE_2D);*/

	//dsShowLightSource();
	glutSwapBuffers();
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
	gluPerspective(60, width / double(height), 2, 20000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void idle() {
	static long t=clock();
	long deltaT=clock()-t;
	if(deltaT>0&&deltaT<mspf)
		//阻塞该线程
		Sleep(mspf-deltaT);
	t=clock();

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