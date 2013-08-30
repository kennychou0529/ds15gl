#include "dsScene.h"
#include "dsTexture.h"
#include "dsTextManager.h"
#include "dsModel.h"
#include "dsTools.h"
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
    {
        //soldier.renderSmoothly(soldier.time_manager.getDurationSecf() * 5);
        soldier.animate();
        //glTranslatef(10.0f, 0, 0);
        soldier2.animate();
    }
    glPopMatrix();

    map.renderGrid();

    //dsShowLightSource();

    dsShowAxes();
}

void DSScene::initialize() {
    soldier.load("data/sword_man/tris.md2",
                 "data/sword_man/tris.bmp",
                 "data/sword_man/weapon.md2",
                 "data/sword_man/weapon.bmp");
    soldier.setPosition(6, 6);
    soldier.setTarget(8, 9);
    soldier.enterStatus(dsSoldier::Status::running);
    soldier2.load("data/mage/tris.md2",
                 "data/mage/tris.bmp",
                 "data/mage/weapon.md2",
                 "data/mage/weapon.bmp");
    //soldier2.time_manager.recordTime();
    soldier.setPosition(1, 1);
    soldier2.enterStatus(dsSoldier::Status::idle);
    dsSkyBox.load(3);
}
