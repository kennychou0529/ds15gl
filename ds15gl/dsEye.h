#ifndef _DSEYE_H
#define _DSEYE_H

#include <GL/glut.h>
#include <chrono>

// �۾�λ�ã��������� (r, phi, theta) ��ʾ
// ���У�phi ��ʾ�� z ��ļн�
// theta ��ʾ�� xy ƽ���ͶӰ����ת�� 
extern GLdouble eye_sphere[3];

// �������ϵķ���
extern GLdouble up[3];

// �������ĵ㣬�������ԭ��
extern GLdouble center[3];

// ���λ�ã���ֱ���޸ģ�ͨ���������Ӳ���
extern GLdouble eye[3];

// ���ַ���
enum Direction {
    STOP = 0,
    UP = 1,
    DOWN = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3,
};

// �۾���ת����
extern int rdir;

// ��������λ��ƽ�Ʒ���
extern int idir;

extern int mdir;

// ���浱ǰ�ӽ���Ϣ���� dsTools �еļ��̲����������õ�
void saveEyeInfo();

// �����ӽǣ�ÿ�λ�ͼʱ�õ�
void dsSetEye();

#endif