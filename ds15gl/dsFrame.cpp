#include "dsFrame.h"


dsFrame::dsFrame(void)
{	
	setSize(800,600);
}

void dsFrame::initializ(void){
	scene.initScene();
}

dsFrame::~dsFrame(void)
{
}


void dsFrame::display(){
	scene.show();
	statusBar.show();
}

void dsFrame::setSize(int width,int height){
	window_height=height;
	window_width=width;
	statusBar.setSize(width,height);
	scene.setSize(width,height);
}