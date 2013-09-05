#include "dsTools.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>
#include "dsLight.h"
#include "dsVector2f.h"
#include "dsVector.h"
#include "dsSoundManager.h"
#include "dsEye.h"

// 全局使用
int window_width = 1280;
int window_height = 720;
int status_bar_width = 200;

// 文字管理器
dsTextManager dstext;
dsTextManager dstext_small;

// 声音管理
//已移入Frame
//DSSoundManager* soundManager;

#ifdef WIN32
const char* font_file_name = "C:/Windows/Fonts/msyhbd.ttc";
// const char* font_file_name = "C:/Windows/Fonts/wqy-zenhei.ttc";
// const char* font_file_name = "data/xhei_bold.ttf";
#elif defined __unix__
const char* font_file_name = "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc";
#endif

const int font_height = 32;
const int font_height_small = 13;

GLdouble axeLength = eye_sphere[0] * 0.5;

// bool keyboard_enabled = true;

void dsSpecialKeyDown(int key, int x, int y) {
    saveEyeInfo();

    switch (key) {
    case GLUT_KEY_UP:
        rdir |= UP;
        break;
    case GLUT_KEY_DOWN:
        rdir |= DOWN;
        break;
    case GLUT_KEY_LEFT:
        rdir |= LEFT;
        break;
    case GLUT_KEY_RIGHT:
        rdir |= RIGHT;
        break;
    case GLUT_KEY_F1:
        mdir |= UP;
        break;
    case GLUT_KEY_F2:
        mdir |= DOWN;
        break;
    default:
        break;
    }

    axeLength = eye_sphere[0] * 0.5;
}

void dsSpecialKeyUp(int key, int x, int y) {
    saveEyeInfo();

    switch (key) {
    case GLUT_KEY_UP:
        rdir &= ~UP;
        break;
    case GLUT_KEY_DOWN:
        rdir &= ~DOWN;
        break;
    case GLUT_KEY_LEFT:
        rdir &= ~LEFT;
        break;
    case GLUT_KEY_RIGHT:
        rdir &= ~RIGHT;
        break;
    case GLUT_KEY_F1:
        mdir &= ~UP;
        break;
    case GLUT_KEY_F2:
        mdir &= ~DOWN;
        break;
    default:
        break;
    }
}

void dsKeyUp(unsigned char key, int x, int y) {
    saveEyeInfo();

    switch (key) {
    case 'a':
    case 'A':
        idir &= ~LEFT;
        break;
    case 'w':
    case 'W':
        idir &= ~UP;
        break;
    case 'd':
    case 'D':
        idir &= ~RIGHT;
        break;
    case 's':
    case 'S':
        idir &= ~DOWN;
        break;
    default:
        break;
    }
}

void dsKeyDown(unsigned char key, int x, int y) {
    saveEyeInfo();

    switch (key) {
    case 'a':
    case 'A':
        idir |= LEFT;
        break;
    case 'w':
    case 'W':
        idir |= UP;
        break;
    case 'd':
    case 'D':
        idir |= RIGHT;
        break;
    case 's':
    case 'S':
        idir |= DOWN;
        break;
    case 'm':
        void centerMoveTo(float, float);
        centerMoveTo(10, 10);
        break;
    default:
        break;
    }
}

//效果不理想
//void dsPassiveMonitionFunc(int x,int y){
//  extern int width;
//  extern int height;
//  if(x<width/20){
//      center[0]-=viewMoveSpeed;
//  }else if(x>width*19/20){
//      center[0]+=viewMoveSpeed;
//  }
//}

// 测试用的函数，画一个直角坐标系
void dsShowAxes() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60, 1, 2, 1000);
    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, 100, 100);
    //glPushMatrix();
    //glLoadIdentity();
    glTranslated(center[0], center[1], center[2]);

    //glutSolidCube(1);
    glLineWidth(1.0);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    {
        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(axeLength, 0.0, 0.0);

        glColor3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, axeLength, 0.0);

        glColor3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, axeLength);
    }
    glEnd();
    //glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void fillRectange2D(int x, int y, int width, int length) {
    int height = -10;
    glBegin(GL_QUADS);
    {
        glVertex3d(x, y, height);
        glVertex3d(x + width, y, height);
        glVertex3d(x + width, y + length, height);
        glVertex3d(x, y + length, height);
    }
    glEnd();
}

void drawRectange2D(int x, int y, int width, int length) {
    int height = -10;
    glBegin(GL_LINE_LOOP);
    {
        glVertex3d(x, y, height);
        glVertex3d(x + width, y, height);
        glVertex3d(x + width, y + length, height);
        glVertex3d(x, y + length, height);
    }
    glEnd();
}
