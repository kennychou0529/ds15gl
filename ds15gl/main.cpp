#include "dstools.h"

GLuint texGround;

void display() {
	dsSet(); // 这个函数需要修改
	dsSetLight();
	dsSetMaterial();

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

	glutPostRedisplay();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("DS15");
	glutDisplayFunc(display);
	glutSpecialFunc(dsSpecialKeys);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	texGround = dsLoadTextureBMP2D("ground.bmp");
	glutMainLoop();
	return 0;
}