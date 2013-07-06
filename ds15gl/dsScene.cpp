#include "dsScene.h"
#include "dstexture.h"

DSScene::DSScene() {}

DSScene::~DSScene() {
	if (groud != NULL) {
		glIsList(groud);
		glDeleteLists(groud, 1);
	}
}

void DSScene::show(){
	glViewport(0, 0, window_width - 200, window_height);
	dsSkyBox.show();

	//glDisable(GL_TEXTURE_2D);
	//glBegin(GL_TRIANGLES);
	//{
	//	glVertex3d(10.0, 0.0, 0.0);
	//	glVertex3d(0.0, 10.0, 0.0);
	//	glVertex3d(-10.0, 0.0, 0.0);
	//}
	//glEnd();
	dsShowAxes();
}



void DSScene::initialize() {
	dsSkyBox.load(3);
}

void DSScene::setSize(int width, int height){
	window_height = height;
	window_width = width;
}