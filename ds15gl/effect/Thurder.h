#pragma once
#include "vector.h"
#include <GL/glut.h>
#include <ctime>
#include <iostream>

//一种类似雷电的直线攻击效果
class Thurder {
public:
    Thurder(float sx, float sy, float sz,
            float ex, float ey, float ez,
            unsigned int seg, unsigned int ln,
            float r, float life
           ) {
        Vector s = {sx, sy, sz};
        Vector e = {ex, ey, ez};

        start = s;
        end = e;
        division = (e - s) * (float(1) / seg);

        Color cc = {0, 0, 1, 1};
        c = cc;

        this->seg = seg;
        this->ln = ln;
        this->r = r;
        this->life = life;
        lastTime = clock();

    }
    void draw() {
        if (life <= 0) {
            return;
        }
        //static clock_t lastTime = clock();
        clock_t nowTime = clock();
        float duration = (nowTime - lastTime) / 1000.0f;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		glDisable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 2 / 255.0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        if (ln == 1 && seg == 2) {
            glColor4f(c.r, c.g, c.b,c.a);
			glLineWidth(r);
            glBegin(GL_LINE_STRIP);
            glVertex3f(start.x,start.y, start.z);
            glVertex3f(end.x, end.y, end.z);
			glEnd();

        } else
            for (int i = 0; i < ln; i++) {
                //float length = (start)
                glColor4f(c.r, c.g, c.b,c.a);
				glLineWidth(1);
                glBegin(GL_LINE_STRIP);
                for (int j = 0; j < seg; j++) {
                    Vector v;
                    v.x = start.x + division.x * j +  r * randF();
                    v.y = start.y + division.y * j +  r * randF();
                    v.z = start.z + division.z * j +  r * randF();
                    glVertex3f(v.x, v.y, v.z);
                }
                glEnd();
            }



        glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);

		glPopAttrib();
        life -= duration;

        lastTime = nowTime;

    }
    void setColor(float r, float g, float b,float a=1) {
        Color cc = {r, g, b, a};
        c = cc;
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

    clock_t lastTime ;
    Color c;
};

