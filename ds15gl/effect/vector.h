#ifndef EFFECT_VECTOR_H
#define EFFECT_VECTOR_H

#include <cmath>

extern float randF();

struct Color {
	float r, g, b, a;
};


struct Vector {
	float x, y, z;


	Vector operator +(Vector& a) {
		Vector sum;
		sum.x = x + a.x;
		sum.y = y + a.y;
		sum.z = z + a.z;
		return sum;
	}
	void unit() {
		float l = sqrt(x * x + y * y + z + z);
		if (l > 0) {
			x /= l;
			y /= l;
			z /= l;
		}
	}
	Vector operator -(Vector& a) {
		Vector sum;
		sum.x = x - a.x;
		sum.y = y - a.y;
		sum.z = z - a.z;
		return sum;
	}
	Vector operator *(float m) {
		Vector product;
		product.x = x * m;
		product.y = y * m;
		product.z = z * m;
		return product;
	}
	//²æ³Ë
	Vector operator *(Vector m) {
		Vector product;
		product.x = y * m.z - z * m.y;
		product.y = z * m.x - x * m.z;
		product.z = z * m.y + y * m.x;
		return product;
	}
	static Vector RotationToDirection(float yaw, float pitch) {
		Vector t;
		t.x = -sin(yaw) * cos(pitch);
		t.y = sin(pitch);
		t.z = cos(pitch) * cos(yaw);
		return t;
	}

	static Vector I(){
		static Vector i = {1,1,1};
		return i;
	}
};
#endif //EFFECT_VECTOR_H