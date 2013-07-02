#pragma once
class vector2f
{
public:
	vector2f(float xx=0,float yy=0);
	~vector2f(void);
	const float lenth();
	const vector2f operator +(const vector2f vec);
	const vector2f operator -(const vector2f vec);
	const vector2f operator *(const vector2f vec);//点积
	const vector2f turnLeft();//返回一个逆时针旋转90度的向量
	const float operator[](int index);
	int normalLise();

public :
	float vec[2];
	float &x,&y;
};

