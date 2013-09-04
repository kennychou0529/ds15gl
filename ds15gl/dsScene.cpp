#include "dsScene.h"
#include "dsTexture.h"
#include "dsTextManager.h"
#include "dsModel.h"
#include "dsTools.h"
#include <sstream>
#include <utility>
#include "dsFrame.h"

extern dsTextManager dstext;
extern DSFrame frame;
DSScene::DSScene() :actors(frame.actors){}

DSScene::~DSScene() {
    if (glIsList(groud)) {
        glDeleteLists(groud, 1);
    }
}

void DSScene::show() {
	
    glViewport(0, 0, window_width - status_bar_width, window_height);
    dsSkyBox.show();
    
    glEnable(GL_LIGHTING);
    
    /*包含在ActorManager中
	glPushMatrix();
    {
        soldiers["sword_man"].animate();
        soldiers["mage"].animate();
    }
    glPopMatrix();*/

	actors.render();

    map.renderGrid();

    //dsShowLightSource();

    dsShowAxes();
}

void DSScene::initialize() {

   /* 由Manager管理
   soldiers.insert(std::make_pair("sword_man", dsSoldier()));
    soldiers["sword_man"].load("data/sword_man/tris.md2",
                               "data/sword_man/tris.bmp",
                               "data/sword_man/weapon.md2",
                               "data/sword_man/weapon.bmp");

    soldiers["sword_man"].setPosition(6, 6);
    soldiers["sword_man"].setTarget(8, 9);
    soldiers["sword_man"].enterStatus(dsSoldier::Status::running);

    soldiers.insert(std::make_pair("mage", dsSoldier()));
    soldiers["mage"].load("data/mage/tris.md2",
                          "data/mage/tris.bmp",
                          "data/mage/weapon.md2",
                          "data/mage/weapon.bmp");

    soldiers["mage"].setPosition(1, 1);
    soldiers["mage"].enterStatus(dsSoldier::Status::idle);*/

	actors.initialize();

    dsSkyBox.load(3);
}
