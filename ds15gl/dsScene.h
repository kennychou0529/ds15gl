#ifndef _DSSCENE_H
#define _DSSCENE_H

#include "dsMap.h"
#include <GL/glut.h>
#include "dsSkybox.h"
#include "dsSoldier.h"
#include <map>
#include "dsActorManager.h"

// 与场景有关的类
// 所有绘图操作都应该转移到这个类
class DSScene {
public:
    DSScene();
    ~DSScene();
    void initialize();
    void show();
    DSMap map;
private:
    DSSkybox dsSkyBox;
	//移入cctors
    //std::map<std::string, dsSoldier> soldiers;
    //显示列表
    GLuint groud;
	DSActorManager& actors;
};

#endif
