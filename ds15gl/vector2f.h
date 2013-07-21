#ifndef _VECTOR2f_H
#define _VECTOR2f_H

class vector2f
{
public:
	vector2f(float xx = 0, float yy = 0);
	~vector2f();
	const float lenth();
	const vector2f operator+(const vector2f vec);
	const vector2f operator-(const vector2f vec);
	const vector2f operator*(const vector2f vec); // 点积
	const vector2f turnLeft(); // 返回一个逆时针旋转 90 度的向量
	const float operator[](int index);
	int normalLise();

public :
	float vec[2];
	float &x, &y;
};

#endif