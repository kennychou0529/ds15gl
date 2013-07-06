#include "dsFrame.h"


DSFrame::DSFrame(void)
{	
	setSize(800,600);
}

void DSFrame::initialize(void){
	scene.initialize();
}

DSFrame::~DSFrame(void)
{
}


void DSFrame::display(){
	scene.show();
	statusBar.show();
}

void DSFrame::setSize(int width,int height){
	window_height=height;
	window_width=width;
	statusBar.setSize(width,height);
	scene.setSize(width,height);
}