#include "dstools.h"
#include "dsScene.h"

int width = 800, height = 600;
dsScene scene;

void display() {
	dsSet(); // 这个函数需要修改
	dsSetLight();
	dsSetMaterial();

	// 添加视口，为什么之前的视口不能占满窗口
	glViewport(0, 0, width, height);

	scene.showScene();
	
	dsShowAxes();
	
	glDisable(GL_TEXTURE_2D);
	glutSolidTeapot(5.0);
	glEnable(GL_TEXTURE_2D);

	dsShowLightSource();

	glutSwapBuffers();
}

void init() {
	scene.initScene();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	// texGround = dsLoadTextureBMP2D("ground.bmp");
}

// 当窗口大小被修改时自动调用此函数
void reshapeFunc(int w, int h) {
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / double(h), 2, 2000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void idle() {
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