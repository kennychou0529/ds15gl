#include "dsTools.h"
#include "dsFrame.h"
#include "dsTextManager.h"
#include <ctime>
#include <iostream>
#include <thread>

////目前只支持 WIN32 和 Linux
//#ifdef WIN32
//  #define SLEEP(mm) Sleep(mm)
//#else
//  //这个还不知道行不行
//  #include <unistd.h>
//  #define SLEEP(mm) usleep(mm*1000)
//#endif // WIN32

DSFrame frame;

// 每帧时间，毫秒
//static const int mspf = 33;
static auto sleep_time = std::chrono::milliseconds(33);

void dsDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	dsSet(); // 设置视角

	frame.display();

	glutSwapBuffers();

	// 打印 GL 错误
	GLenum errCode;
	const GLubyte* errString;

	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << errString << std::endl;
	}

}

void dsInit() {
	dsSetMaterial();
	dsSetLight();
	frame.initialize();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH); // why
	glDepthFunc(GL_LEQUAL); // why
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // why
	dstext.init(font_file_name, font_height);
}

// 当窗口大小被修改时自动调用此函数
void dsReshape(int w, int h) {
	window_width = w;
	window_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (window_width - status_bar_width) / double(window_height), 0.2, 20000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void dsIdle() {
	//static long t = clock();
	//long deltaT = clock() - t;
	//if (deltaT > 0 && deltaT < mspf)
	//	// 阻塞该线程
	//	SLEEP(mspf - deltaT);
	//t = clock();
	std::this_thread::sleep_for(sleep_time);
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("DS 15th");
	glutDisplayFunc(dsDisplay);
	// glutMouseFunc(dsMouseFunc);
	// glutPassiveMotionFunc(dsPassiveMonitionFunc);
	glutSpecialFunc(dsSpecialKeys);
	glutReshapeFunc(dsReshape);
	glutIdleFunc(dsIdle);
	glutKeyboardFunc(dsKeys);
	dsInit();
	glutMainLoop();
	return 0;
}
