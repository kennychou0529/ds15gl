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
	dsShowAxes();
}



void DSScene::initialize() {
	dsSkyBox.load();
}

void DSScene::setSize(int width, int height){
	window_height = height;
	window_width = width;
}