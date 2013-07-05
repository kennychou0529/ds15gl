#include "dsStatusBar.h"
#include "dstools.h"

dsStatusBar::dsStatusBar(void)
{
	width=150;
}


dsStatusBar::~dsStatusBar(void)
{
}

void dsStatusBar::show(){
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,100,0,200,0,window_height);	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(window_width-width,0,width,window_height);
	//glPushAttrib(GL_PROJECTION);
	
	gluLookAt(0,0,10,0,0,0,0,1,0);
	glColor3d(1,1,1);	
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	//drawRectange(0,0,100,window_height,0);
	glutSolidCube(100);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	//glViewport(window_width-width,0,width,window_height);
	glMatrixMode(GL_MODELVIEW);
}

void dsStatusBar::setSize(int width,int height){
	window_height=height;
	window_width=width;
}