#include "dstools.h"

GLuint texGround;
int width=800,height=600;


void display() {
	dsSet(); // 这个函数需要修改
	dsSetLight();
	dsSetMaterial();
	//添加视口，为什么之前的视口不能占满窗口
	glViewport(0,0,width,height);
	glBindTexture(GL_TEXTURE_2D, texGround);
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-8.0f, -8.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-8.0f, 8.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(8.0f, 8.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(8.0f, -8.0f, 0.0f);
	}
	glEnd();

	
	glutSwapBuffers();
}

void init(void){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	texGround = dsLoadTextureBMP2D("ground.bmp");
}

void reshapeFunc(int w,int h){
	width=w;
	height=h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,w/float(h),1,2000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void idle(){
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