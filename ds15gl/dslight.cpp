#include "dslight.h"

void dsSetLight() {
	// 创建光源

	glEnable(GL_LIGHT0);

	// 环境光
	GLfloat ambient[4]  = {0.5f, 250.0f / 255 / 2, 200.0f / 255 / 2, 1.0f};

	// 漫射光
	GLfloat diffuse[4]  = {0.5f, 0.5f, 0.5f, 1.0f};

	// 镜面光，产生亮斑
	GLfloat specular[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	
	// 光源位置
	GLfloat position[4] = {1.0f, 1.0f, 1.0f, 0.2f};
	
	// 将数据载入光源 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

}

void dsSetMaterial() {
	// 环境光
	GLfloat ambient[4]  = {1.0f, 1.0f, 1.0f, 1.0f};

	// 漫射光
	GLfloat diffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f};

	// 镜面光，产生亮斑
	GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	GLfloat shinness = 5.0;
	
	// 将数据载入材质
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinness);
	//glMaterialfv(GL_FRONT, GL_POSITION, position);
}