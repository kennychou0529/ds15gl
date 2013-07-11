#ifndef _DSSKYBOX_H
#define _DSSKYBOX_H

#include <GL/glut.h>
#include "dsTexture.h"

// ��պ��࣬�Ѿ��������л�����պеĲ��ֶ�д��������

class DSSkybox {
public:
	DSSkybox(): skyBox(0) {}
	~DSSkybox() { del(); }

	// ������պ� ���� ������ ��ʾ�б�
	void load(GLuint index = 3);

	// ��ʾ��պ�
	void show();

	// ɾ����պ� ��ʾ�б� �� ����
	void del();

private:
	// ��ʾ�б���
	GLuint skyBox;

	// ��պ�������
	GLuint texture[6];

	GLdouble width; // ��պп��
	GLdouble height; // ��պи߶ȣ���������������ͼƬ�Ĵ�С����
	GLdouble depth; // Ϊ�˽��۾����ڴ�Լ xy ƽ�棬��Ҫ����պе������ - depth �߶���

};

#endif
