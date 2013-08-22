#ifndef _DSSCENE_H
#define _DSSCENE_H

#include "dsMap.h"
#include <GL/glut.h>
#include "dsSkybox.h"
#include "dsSoldier.h"

// 与场景有关的类
// 所有绘图操作都应该转移到这个类
class DSScene {
public:
    DSScene();
    ~DSScene();
    void initialize();
    void show();

private:
    DSMap map; // 以后改名
    DSSkybox dsSkyBox;
    // MD2Model model;
    // MD2Model weapon;

    dsSoldier soldier;
    dsSoldier soldier2;
    //显示列表
    GLuint groud;
};

#endif
