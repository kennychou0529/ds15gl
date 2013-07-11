#ifndef _DSSCENE_H
#define _DSSCENE_H

#include "dsMap.h"
#include <GL/glut.h>
#include "dsSkybox.h"
#include "dstools.h"

// �볡���йص���
// ���л�ͼ������Ӧ��ת�Ƶ������
class DSScene {
public:
	DSScene();
	~DSScene();
	void initialize();
	void show();

private:
	DSMap map; // �Ժ����	
	DSSkybox dsSkyBox;

	//��ʾ�б�
	GLuint groud;
};

#endif