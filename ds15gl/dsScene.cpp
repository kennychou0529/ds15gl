#include <sstream>
#include <utility>
#include "dsMessageBox.h"

#include "dsScene.h"
#include "dsTools.h"
#include "dsFrame.h"

//extern dsTextManager dstext;
extern DSFrame frame;

DSScene::DSScene() : actors(frame.actors) {}

DSScene::~DSScene() {
    if (glIsList(groud)) {
        glDeleteLists(groud, 1);
    }
}


void DSScene::show(bool selectMode) {
	
    glViewport(0, 0, window_width - status_bar_width, window_height);
    if (!selectMode) {
        dsSkyBox.show();

        glEnable(GL_LIGHTING);
        //object_manager.objects["banana"].render();
       
    }
	actors.render(selectMode);
    map.render(selectMode);
    mboxes.render();
    //dsShowLightSource();    
	dsShowAxes();
}

void DSScene::initialize() {
    dsSkyBox.load(0);
}
