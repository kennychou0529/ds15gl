#pragma once
#include "dsMap.h"
#include <GL/glut.h>
#include "dsSkybox.h"

// 与场景有关的类
// 所有绘图操作都应该转移到这个类
class dsScene
{
public:
	dsScene();
	~dsScene();
	void initScene();
	void initSkyBox();
	void showScene();

private:
	dsMap dsmap; // 以后改名	
	//显示列表
	GLuint skyBox;
	GLuint groud;
	
	////记录显示列表数量，析构时使用
	////int listCount;
	//天空和地面可能是万年不变的，
	//因此用显示列表	

	//贴图
	GLuint tex[6]; //east,west,south,north,top

	dsSkybox dsskybox;
};

