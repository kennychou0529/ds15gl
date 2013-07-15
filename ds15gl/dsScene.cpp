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
	str.drawString(L"中文测试测试中文",32);
	glRasterPos2d(8.0, 40.0);
	str.drawString(L"中文English混合",32);
	
	glPushMatrix();
	glLoadIdentity();
	dstext.print(5, 400, L"您好，伟大的谈至勋。\n众所周知，您是无所不能的，但是：\n您能不能告诉我们，为什么您这么厉害呢？");
	glPopMatrix();

	glEnable(GL_LIGHTING);

	dsShowLightSource();

	dsShowAxes();
}

void DSScene::initialize() {
	dsSkyBox.load(3);
}
