#ifndef _VECTOR2f_H
#define _VECTOR2f_H

#include <GL/glut.h>

class dsVector2f {
public:
    dsVector2f(GLfloat _x = 0, GLfloat _y = 0);
    ~dsVector2f() {}
    GLfloat getLenth();
    dsVector2f operator+(const dsVector2f& rhs) const;
    dsVector2f operator-(const dsVector2f& rhs) const;

    // 点积
    GLfloat operator*(const dsVector2f& rhs) const;

    // 返回一个逆时针旋转 90 度的向量
    dsVector2f turnLeft() const;
	// 返回一个逆时针旋转 45 度的向量
	dsVector2f turn45d() const;


    GLfloat& operator[](size_t index);

    int normalise();

public:
    GLfloat x, y;
};

#endif