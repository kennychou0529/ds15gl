#include "dsStatusBar.h"
#include "dsTools.h"
#include "dsText.h"
#include <sstream>
#include <iomanip>
#include "dsTextManager.h"

extern dsTextManager dstext;
DSStatusBar::DSStatusBar() {}

DSStatusBar::~DSStatusBar() {}

void DSStatusBar::show() {
	// 更改投影方式为 2D 平行投影
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, status_bar_width, 0, window_height, 0.01, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(window_width - status_bar_width, 0, status_bar_width, window_height);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);



	gluLookAt(0.0, 0.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 1.0);

	//fillRectange2D(0,0,width,window_height);

	//这里画小地图，战场，人物等状态信息
	//例如
	glPushMatrix();
	glTranslated(status_bar_width / 2, status_bar_width / 2, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);	
	glutSolidTeapot(50);
	glPopMatrix();
	
	
	// 让我们在这里写一下眼睛位置
	glRasterPos2d(8.0, 8.0);
	std::ostringstream os;
	// os.unsetf(std::scientific);
	
	os << "(" << std::fixed << std::setprecision(2) << eye[0] << ", " << eye[1] << ", " << eye[2] << ")";
	drawString(os.str().c_str());
	
	glRasterPos2d(40.0, 24.0);
	drawString("Eye position");

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glLoadIdentity();
	dstext.print(5, 400, L"您好");
	glPopMatrix();

	//回到透视投影
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//void DSStatusBar::setSize(int width, int height){
//	window_height = height;
//	window_width = width;
//}
