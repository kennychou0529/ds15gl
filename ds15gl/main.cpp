#include <iostream>
#include "dsLight.h"
#include "dsTools.h"
#include "dsFrame.h"
#include "dsTextManager.h"
#include "dsModel.h"
#include "dsEye.h"
#include <alut.h>
#include "dsSoundManager.h"

//
//// 这可以避免在 Windows 下出现命令行窗口
//#ifdef WIN32
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
//#endif

DSFrame frame;

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
    glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH); // why
    glDepthFunc(GL_LEQUAL); // why
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // why
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    dstext.init(font_file_name, font_height);
    dstext_small.init(font_file_name, font_height_small);

    //this object must be initialized after main function
    //soundManager = DSSoundManager::getSoundManager();
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
    glutPostRedisplay();
}

//some object need to be destroyed
void destroy() {
    //delete soundManager;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    // alutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("DS 15th");
    glutDisplayFunc(dsDisplay);
    // glutMouseFunc(dsMouseFunc);
    // glutPassiveMotionFunc(dsPassiveMonitionFunc);
    glutSpecialFunc(dsSpecialKeyDown);
    glutSpecialUpFunc(dsSpecialKeyUp);
    glutReshapeFunc(dsReshape);
    glutIdleFunc(dsIdle);
    glutKeyboardFunc(dsKeyDown);
    glutKeyboardUpFunc(dsKeyUp);
    dsInit();
    glutMainLoop();
    destroy();
    return 0;
}
