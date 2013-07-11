#ifndef _DSVECTOR_H
#define _DSVECTOR_H

#include <GL/glut.h>
#include <cmath>

using std::sqrt;
using std::sin;
using std::cos;

// ���� ����������ĵ������� �� ���ĵ�ֱ�����꣬��� ���ֱ������
void dsSphereToOrtho3dv(const GLdouble sphere[3], const GLdouble center[3], GLdouble ortho[3]);

// ���� ����������ĵ������� �� ���ֱ�����꣬��� ���ĵ�ֱ������
void dsFindSphereCenter(const GLdouble sphere[3], const GLdouble ortho[3], GLdouble center[3]);

// ���������ĳ���
GLdouble dsLenth3dv(GLdouble vec[3]);

// �����ĵ�λ��
void dsNormalize3dv(GLdouble vec[3]);

// �����ļ���
void dsDiff3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// �����ļӷ�
void dsAdd3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// ����������
void dsScalarMulti3dv(const GLdouble v[3], const GLdouble multiplier, GLdouble ret[3]);

// �����ĵ��
GLdouble dsDot3dv(const GLdouble v1[3], const GLdouble v2[3]);

// �����Ĳ��
void dsCross3dv(const GLdouble v1[3], const GLdouble v2[3], GLdouble ret[3]);

// ���� �����ε��������㣬��� ��λ������
void dsNormalVectorOfTriangle3dv(const GLdouble p1[3], const GLdouble p2[3], const GLdouble p3[3], GLdouble ret[3]);


#endif
