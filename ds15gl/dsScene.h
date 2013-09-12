﻿#ifndef _DSSCENE_H
#define _DSSCENE_H

#include <GL/glut.h>
#include <map>
#include "dsSkybox.h"
#include "dsMap.h"
#include "dsActorManager.h"
#include "dsObject.h"

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

    //显示列表
    GLuint groud;

	DSActorManager& actors;

    DSObjectManager object_manager;
};

#endif
