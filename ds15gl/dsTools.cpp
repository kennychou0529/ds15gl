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
#include "dsFrame.h"

extern DSFrame frame;

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

void dsSpecialKeyDown(int key, int x, int y) {
    int previous_rdir = rdir;
    int previous_mdir = mdir;

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
    case GLUT_KEY_F3:
        if (frame.actors.round_finished) {
            frame.actors.enterNextRound();
        }

    default:
        break;
    }

    if (previous_mdir != mdir || previous_rdir != rdir) {
        saveEyeInfo();
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
    int previous_idir = idir;

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
        centerMoveTof(10, 10);
        break;
	case 'i':
		frame.initialize();
		break;
    default:
        break;
    }

    if (previous_idir != idir) {
        saveEyeInfo();
    }

}

void processHits(GLint hits, GLuint buffer[]) {
    GLuint names, *ptr;
    ptr = (GLuint*)buffer;
    GLuint* select = NULL;
    float deepth = 1e5;
    GLuint mnames;//
    for (int i = 0; i < hits; i++) {
        names = *ptr;
        ptr ++;
        float cdeepth = (float) * ptr / 0x7fffffff;
        printf("z1 is %g; ", (float)*ptr / 0x7fffffff);
        ptr ++;
        printf("z2 is %g\n", (float)*ptr / 0x7fffffff);
        ptr ++;
        if (cdeepth < deepth && !(*(ptr - 3) == 1 && *ptr == 1000)) {
            deepth = cdeepth;
            select = ptr;
            mnames = names;
        }

        ptr+=names;
    }
    if (select == NULL) {
        return;
    }
    ptr = select;
    printf("names are: ");
    for (int j = 0; j < mnames; j++) {
        printf("%d ", *ptr);
        if (*ptr > 0 && *ptr <= 400) {
            size_t x, y;
            frame.scene.map.getXY(*ptr, &x, &y);
            printf("选中了[%d,%d]格子", x, y);
        } else if (*ptr > 1000 && *ptr <= 1100) {
            printf(("选中了士兵" + frame.actors.intToString[*ptr - 1000]).c_str());
            frame.actors.selectSoldier(*ptr - 1000);
        }
        ptr++;
    }
    printf("\n");
}

#define SIZE 512
#define N 5
void dsMouseFunc(int button, int state, int x, int y) {
    GLuint selectBuf[SIZE];
    GLint hits;

    GLint viewport[4];
    if (state != GLUT_DOWN) {
        return;
    }
    if (button == GLUT_LEFT_BUTTON) {
        printf("left clicked at(%d,%d)", x, y);
        glViewport(0, 0, window_width - status_bar_width, window_height);
        glGetIntegerv(GL_VIEWPORT, viewport);    //初始化名称堆栈
        glSelectBuffer(SIZE, selectBuf);
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(1000);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();     // 定义一个以光标为中心的NxN拾取区域     // 必须对

        //鼠标y坐标求反，从屏幕坐标系转换成世界坐标系
        gluPickMatrix(x, viewport[3] - y, N, N, viewport);
        //gluOrtho2D(-2, 2, -2, 2);
        gluPerspective(60, (window_width - status_bar_width) / double(window_height), 0.2, 20000);
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        dsSetEye();
        //drawObjects(GL_SELECT);
        frame.display(true);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glFlush();
        hits = glRenderMode(GL_RENDER);
        printf(" hits = %d \n", hits);
        processHits(hits, selectBuf);
        glutPostRedisplay();
    }
    if (button == GLUT_RIGHT_BUTTON) {
        frame.actors.cancelSelect();
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
    glPushMatrix();
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
    glColor3f(1, 1, 1);
    glPopMatrix();

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
