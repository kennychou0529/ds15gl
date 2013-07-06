#include "dsStatusBar.h"
#include "dstools.h"

DSStatusBar::DSStatusBar(void)
{
	width=150;
}


DSStatusBar::~DSStatusBar(void)
{
}

void DSStatusBar::show(){
	
	//更改投影方式为2D平行投影
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,width,0,window_height,0,1000);	
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	glViewport(window_width-width,0,width,window_height);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE);
	gluLookAt(0,0,200,0,0,0,0,1,0);
	glColor3d(0,0,0);
	//fillRectange2D(0,0,width,window_height);

	//这里画小地图，战场，人物等状态信息
	//例如
	glColor3f(1,0,1);	
	glutSolidTeapot(100);
	
	glEnable(GL_TEXTURE);
	glEnable(GL_LIGHTING);
	//回到透视投影
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DSStatusBar::setSize(int width,int height){
	window_height=height;
	window_width=width;
}