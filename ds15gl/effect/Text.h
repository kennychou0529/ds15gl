#pragma once
#include <string>
#include "../dsTextManager.h"
#include <ctime>
#include <GL/glut.h>
using std::string;

extern dsTextManager dstext_small;
class Text {
public:
    Text(string _text, float _life):  life(_life) {
        text = new char[_text.length() + 1];
        strcpy(text, _text.c_str());
    }

    void setPosition(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
        a = 1;
    }
    void setColor(float _r, float _g, float _b) {
        r = _r;
        g = _g;
        b = _b;
    }

    ~Text(void) {
        delete[] text;
    }

    void draw() {
        if (life <= 0) {
            return;
        }
        static clock_t lastTime = clock();
        clock_t nowTime = clock();
        float duration = (nowTime - lastTime) / 1000.0f;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();

        glLoadIdentity();
        glTranslatef(x, y, z);
        //dstext_small.print(10, 10, text);
        glColor4f(r, g, b, a);

		 glRasterPos2i(100, 100);
        for (char* s = text; *s; s++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *s);
        }

        glPopMatrix();
		glPopAttrib();
        life -= duration;
        lastTime = nowTime;
    }

    bool alive() {
        return life > 0;
    }

private:
    float life;
    char* text;
    //Î»ÖÃ
    float x, y, z;
    //ÑÕÉ«
    float r, g, b;
    float a;

    int e;
    //GL_FRONT font_style = GLUT_BITMAP_TIMES_ROMAN_24;
};

