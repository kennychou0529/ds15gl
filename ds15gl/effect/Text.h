#pragma once
#include <string>
#include "../dsTextManager.h"
#include <ctime>
#include <GL/glut.h>

extern dsTextManager dstext_small;
class Text {
public:
    Text(std::wstring _text,float _life): text(_text),life(_life) {}
    void setPosition(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    void setColor(float _r, float _g, float _b) {
        r = _r;
        g = _g;
        b = _b;
    }

    ~Text(void){}
    void draw() {
        if (life <= 0) {
            return;
        }
        static clock_t lastTime = clock();
        clock_t nowTime = clock();
        float duration = (nowTime - lastTime) / 1000.0f;

        glPushMatrix();

		glLoadIdentity();
        //glTranslatef(x, y, z);
        dstext_small.print(10, 10, text);

        glPopMatrix();

        life -= duration;
        lastTime = nowTime;
    }

    bool alive() {
        return life > 0;
    }

private:
    float life;
    std::wstring text;
    //Î»ÖÃ
    float x, y, z;
    //ÑÕÉ«
    float r, g, b;
    float a;

    int e;
};

