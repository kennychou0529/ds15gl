﻿#include "dsVector2f.h"
#include <cmath>
#include <stdexcept>

dsVector2f::dsVector2f(GLfloat _x, GLfloat _y): x(_x), y(_y) {}

GLfloat dsVector2f::getLenth() {
    return std::sqrt(x * x + y * y);
}

dsVector2f dsVector2f::operator+(const dsVector2f& rhs) const {
    return dsVector2f(x + rhs.x, y + rhs.y);
}

dsVector2f& dsVector2f::operator+=(const dsVector2f& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

dsVector2f dsVector2f::operator-(const dsVector2f& rhs) const {
    return dsVector2f(x - rhs.x, y - rhs.y);
}

dsVector2f& dsVector2f::operator-=(const dsVector2f& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

dsVector2f dsVector2f::operator*(const GLfloat multiplier) const {
    return dsVector2f(x * multiplier, y * multiplier);
}

dsVector2f operator*(const GLfloat multiplier, const dsVector2f& rhs) {
    return dsVector2f(rhs.x * multiplier, rhs.y * multiplier);
}

// 点积
GLfloat dsVector2f::operator*(const dsVector2f& rhs) const {
    return x * rhs.x + y * rhs.y;
}

// 返回一个逆时针旋转 90 度的向量
dsVector2f dsVector2f::turnLeft() const {
    return dsVector2f(-y, x);
}

dsVector2f dsVector2f::turn45d() const {
    static GLfloat temp = cos(45.0f);
    return dsVector2f((x - y) * temp, (x + y) * temp);
}

int dsVector2f::normalise() {
    if (std::fabs(x) < 1e-4 && std::fabs(y) < 1e-4) {
        return 0;
    }

    GLfloat len = getLenth();
    x /= len;
    y /= len;
    return 1;
}

GLfloat& dsVector2f::operator[](size_t index) {
    switch (index) {
    case 0:
        return x;

    case 1:
        return y;

    default:
        throw std::out_of_range("Error: Only 0 and 1 are valid index.");
    }
}
