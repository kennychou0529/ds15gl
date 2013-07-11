#include "dsScene.h"
#include "dsTexture.h"
#include "dsText.h"

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
	glBegin(GL_TRIANGLES);
	{
		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(10.0, 0.0, 0.0);
		glVertex3d(0.0, 10.0, 0.0);
		glVertex3d(-10.0, 0.0, 0.0);
	}
	glEnd();

	glDisable(GL_LIGHTING);

	// 以下是诡异的文字
	glRasterPos2d(4.0, 2.0);
	drawString("Hello, world!");

	//英文不正常
	glRasterPos2d(8.0, 20.0);
	str.drawString(L"English Test",32);
	glRasterPos2d(8.0, 30.0);
	str.drawString(L"中文测试",32);


	glEnable(GL_LIGHTING);

	dsShowLightSource();

	dsShowAxes();
}

void DSScene::initialize() {
	dsSkyBox.load(3);
}
