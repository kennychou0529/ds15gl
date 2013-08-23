#include <iostream>
#include "dsLight.h"
#include "dsTools.h"
#include "dsFrame.h"
#include "dsTextManager.h"
#include "dsModel.h"

//
//// 这可以避免在 Windows 下出现命令行窗口
//#ifdef WIN32
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"") 
//#endif

DSFrame frame;

// 每帧时间，毫秒
//static const int mspf = 33;
// static auto sleep_time = std::chrono::milliseconds(33);

void dsDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    dsSetEye(); // 设置视角
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
    dstext_small.init(font_file_name, font_height_small);
    //time_manager.recordTime();
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
    // std::this_thread::sleep_for(sleep_time);
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
