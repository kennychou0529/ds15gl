#ifndef _DSSKYBOX_H
#define _DSSKYBOX_H

#include <GL/glut.h>
#include "dstexture.h"

// 天空盒类，已经将场景中绘制天空盒的部分都写在其中了

class DSSkybox {
public:
	DSSkybox(): skyBox(0) {}
	~DSSkybox() { del(); }

	// 载入天空盒 纹理 并分配 显示列表
	void load(GLuint index = 3);

	// 显示天空盒
	void show();

	// 删除天空盒 显示列表 和 纹理
	void del();

private:
	// 显示列表编号
	GLuint skyBox;

	// 天空盒纹理编号
	GLuint texture[6];

	GLdouble width, height;
};

#endif