#ifndef _DSMESSAGEBOX_H
#define _DSMESSAGEBOX_H

#include <string>
#include <GL/glut.h>
#include "dsTimer.h"

class dsMessageBox {
public:
    enum Status {
        appearing,
        showing,
        disappearing,
        disappeared
    };

    dsMessageBox(const std::wstring& message = L"");
    void render();
    std::wstring message;
    dsTimer timer;
private:
    GLfloat x, y;
    GLfloat width, height;
    GLfloat lasting_time;
    Status status;
};

#endif