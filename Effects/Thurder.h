#pragma once
#include "vector.h"
#include <GL/glut.h>
#include <ctime>


//一种类似雷电的直线攻击效果
class Thurder {
public:
    Thurder(float sx, float sy, float sz,
            float ex, float ey, float ez,
            unsigned int seg, unsigned int ln,
            float r,float life
           ) {
        Vector s = {sx, sy, sz};
        Vector e = {ex, ey, ez};

        start = s;
        end = e;
        division = (e - s) * (float(1) / seg);

        Color cc = {0, 0, 1, 1};
        c = cc;

        this->seg = seg;
        this->ln=ln;
        this->r = r;
		this->life = life;

    }
    void draw() {
        if (life <= 0) {
            return;
        }
        static clock_t lastTime = clock();
        clock_t nowTime = clock();
        float duration = (nowTime - lastTime) / 1000.0f;
        for (int i = 0; i < ln; i++) {
            //float length = (start)
            glColor4f(c.r, c.g, c.b, c.a);
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < seg; j++) {
                Vector v;
				v.x= start.x + division.x * j +  r * randF();
				v.y= start.y + division.y * j +  r * randF();
				v.z= start.z + division.z * j +  r * randF();
                glVertex3f(v.x, v.y, v.z);
            }
            glEnd();
        }
		life -= duration;

        lastTime = nowTime;
    }
    bool alive() {
        return life > 0;
    }
    ~Thurder(void);
private:
    Vector start;
    Vector end;

    Vector division;
    //分段数
    unsigned int seg;
    //线条数
    unsigned int ln;
    float r;
    float life;

    Color c;
};

