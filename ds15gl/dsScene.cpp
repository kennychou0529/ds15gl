#include "dsScene.h"
#include "dsTexture.h"
#include "dsText.h"
#include "dsTextManager.h"

extern dsTextManager dstext;

DSScene::DSScene() {}

DSScene::~DSScene() {
	if (glIsList(groud)) {
		glDeleteLists(groud, 1);
	}
}

void DSScene::show(){
	glViewport(0, 0, window_width - status_bar_width, window_height);
	dsSkyBox.show();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//glBegin(GL_TRIANGLES);
	//{
	//	glNormal3d(0.0, 0.0, 1.0);
	//	glVertex3d(10.0, 0.0, 0.0);
	//	glVertex3d(0.0, 10.0, 0.0);
	//	glVertex3d(-10.0, 0.0, 0.0);
	//}
	//glEnd();

	GLdouble len = 20.0;
	GLuint n = 10;

	GLdouble delta_x = len / n;

	for (GLdouble x = - len; x < len - 0.5 * delta_x; x += delta_x) {
		for (GLdouble y = - len; y < len - 0.5 * delta_x; y += delta_x) {
			glBegin(GL_QUADS);
			{
				glNormal3d(0.0, 0.0, 1.0);
				glVertex3d(x, y, 0);
				glVertex3d(x + delta_x, y, 0);
				glVertex3d(x + delta_x, y + delta_x, 0);
				glVertex3d(x, y + delta_x, 0);
			}
			glEnd();
		}
	}


	glDisable(GL_LIGHTING);

	//// 以下是诡异的文字
	//glRasterPos2d(4.0, 2.0);
	//drawString("Hello, world!");

	////英文不正常
	//glRasterPos2d(8.0, 20.0);
	//str.drawString(L"English Test",32);
	//glRasterPos2d(8.0, 30.0);
	//str.drawString(L"中文测试测试中文",32);
	//glRasterPos2d(8.0, 40.0);
	//str.drawString(L"中文English混合",32);

	std::wstring my_str = L"您好，伟大的谈至勋。\n众所周知，您是无所不能的，但是：\n您能不能告诉我们，为什么您这么厉害呢？";
	glPushMatrix();
	glLoadIdentity();
	dstext.print(5, 400, my_str);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	dsShowLightSource();

	dsShowAxes();
}

void DSScene::initialize() {
	dsSkyBox.load(3);
}
