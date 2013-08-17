#include "dsScene.h"
#include "dsTexture.h"
#include "dsTextManager.h"
#include "dsModel.h"

extern dsTextManager dstext;

DSScene::DSScene() {}

DSScene::~DSScene() {
    if (glIsList(groud)) {
        glDeleteLists(groud, 1);
    }
}

void DSScene::show() {
    glViewport(0, 0, window_width - status_bar_width, window_height);
    dsSkyBox.show();
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslated(0, 0, 2.0);
    glScaled(0.1, 0.1, 0.1);
    model.renderFrame((int)(model.time_manager.getDurationSecd() * 20) % 40);
    //glScaled(10.0, 10.0, 10.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //glBegin(GL_TRIANGLES);
    //{
    //  glNormal3d(0.0, 0.0, 1.0);
    //  glVertex3d(10.0, 0.0, 0.0);
    //  glVertex3d(0.0, 10.0, 0.0);
    //  glVertex3d(-10.0, 0.0, 0.0);
    //}
    //glEnd();



    GLdouble len = 20.0;
    GLuint n = 100;

    GLdouble delta_x = len / n;

    for (GLdouble x = - len; x < len - 0.5 * delta_x; x += delta_x) {
        for (GLdouble y = - len; y < len - 0.5 * delta_x; y += delta_x) {
            glBegin(GL_QUADS);
            {
                glNormal3d(0.0, 0.0, 1.0);
                glVertex3d(x, y, 0);
                glVertex3d(x + delta_x, y, 0);
                glVertex3d(x + delta_x, y + delta_x, 0);
                glVertex3d(x, y + delta_x, 0);
            }
            glEnd();
        }
    }

    glDisable(GL_LIGHTING);

    std::wstring my_str = L"(测试文字显示)\n您好，伟大的谈至勋。\n众所周知，您是无所不能的，但是：\n您能不能告诉我们，为什么您这么厉害呢？";
    glPushMatrix();
    glLoadIdentity();
    dstext.print(5, 400, my_str);
    glPopMatrix();

    glEnable(GL_LIGHTING);

    dsShowLightSource();

    dsShowAxes();
}

void DSScene::initialize() {
    model.load("data/sword_man/tris.md2", "data/sword_man/knight_white.bmp");
    model.time_manager.recordTime();
    dsSkyBox.load(3);
}
