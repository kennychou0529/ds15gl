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
	actors.initialize();

    dsSkyBox.load(3);
}
