#include "dsScene.h"
#include "dsTexture.h"
#include "dsTextManager.h"
#include "dsModel.h"
#include <sstream>

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
    //model.renderFrame((int)(model.time_manager.getDurationSecd() * 20) % 40);
    //weapon.renderFrame((int)(model.time_manager.getDurationSecd() * 20) % 40);

    // soldier.renderFrame((int)(soldier.time_manager.getDurationSecd() * 20) % 40);
    glPushMatrix();
    // glTranslatef(0.0, 0, 0);
    soldier.renderSmoothly(soldier.time_manager.getDurationSecf() * 5);
    glTranslatef(60.0, 0, 0);
    soldier2.renderSmoothly(soldier2.time_manager.getDurationSecf() * 5);
    glPopMatrix();
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

    //GLdouble len = 20.0;
    //GLuint n = 100;

    //GLdouble delta_x = len / n;

    //for (GLdouble x = - len; x < len - 0.5 * delta_x; x += delta_x) {
    //    for (GLdouble y = - len; y < len - 0.5 * delta_x; y += delta_x) {
    //        glBegin(GL_QUADS);
    //        {
    //            glNormal3d(0.0, 0.0, 1.0);
    //            glVertex3d(x, y, 0);
    //            glVertex3d(x + delta_x, y, 0);
    //            glVertex3d(x + delta_x, y + delta_x, 0);
    //            glVertex3d(x, y + delta_x, 0);
    //        }
    //        glEnd();
    //    }
    //}
    map.renderGrid();
    glDisable(GL_LIGHTING);

    //std::wstringstream os;
    //os << L"FPS: " << fps;
    //// std::wstring my_str = L"FPS:";
    //// time_manager.getDurationSecf();
    //glPushMatrix();
    //glLoadIdentity();
    //dstext.print(5, 400, os.str());
    //
    //glPopMatrix();

    glEnable(GL_LIGHTING);

    //dsShowLightSource();

    dsShowAxes();
}

void DSScene::initialize() {
    //model.load("data/sword_man/tris.md2", "data/sword_man/knight_white.bmp");
    //weapon.load("data/sword_man/weapon.md2", "data/sword_man/weapon.bmp");
    //model.time_manager.recordTime();
    soldier.load("data/sword_man/tris.md2",
                 "data/sword_man/knight_white.bmp",
                 "data/sword_man/weapon.md2",
                 "data/sword_man/weapon.bmp");
    soldier.time_manager.recordTime();
    soldier2.load("data/bat/tris.md2",
                 "data/bat/tris.bmp",
                 "data/bat/weapon.md2",
                 "data/bat/weapon.bmp");
    soldier2.time_manager.recordTime();
    dsSkyBox.load(3);
}
