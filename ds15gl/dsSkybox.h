#ifndef _DSSKYBOX_H
#define _DSSKYBOX_H

#include <GL/glut.h>
#include "dsTexture.h"

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

	GLdouble width; // 天空盒宽度
	GLdouble height; // 天空盒高度，这个将会根据纹理图片的大小计算
	GLdouble depth; // 为了将眼睛放在大约 xy 平面，需要把天空盒底面放在 - depth 高度上

};

#endif
