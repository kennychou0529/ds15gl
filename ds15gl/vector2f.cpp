#include "vector2f.h"
#include <cmath>

vector2f::vector2f(float xx,float yy): x(vec[0]), y(vec[1]) {
	x = xx;
	y = yy;
}

const vector2f vector2f::operator+(const vector2f vec) {
	vector2f tem;
	tem.x = vec.x + x;
	tem.y = vec.y + y;
	return tem;
}

const vector2f vector2f::operator-(const vector2f vec) {
	vector2f tem;
	tem.x = x - vec.x;
	tem.y = y - vec.y;
	return tem;
}

// 点积
const vector2f vector2f::operator*(const vector2f vec) { 
	vector2f tem;
	tem.x = vec.x * x;
	tem.y = vec.y * y;
	return tem;
}

// 返回一个逆时针旋转90度的向量
const vector2f vector2f::turnLeft() {
	vector2f tem;
	tem.x = -y;
	tem.y = x;
	return tem;
}

const float vector2f::lenth() {
	return sqrt(x * x + y * y);
}

int vector2f::normalLise() {
	if (std::fabs(x) < 1e-4 && std::fabs(y) < 1e-4) {
		return 0;
	}
	float len = lenth();
	x /= len;
	y /= len;
	return 1;
}

const float vector2f::operator[](int index) {
	if (index < 0 || index > 1) {
		return 0;
	}
	return vec[index];
}

vector2f::~vector2f() {}
