#ifndef _DSMESSAGEBOX_H
#define _DSMESSAGEBOX_H

#include <string>
#include <GL/glut.h>
#include <list>

#include "dsTimer.h"

class dsMessageBoxManager;

class dsMessageBox {
    friend class dsMessageBoxManager;
public:
    enum Status {
        appearing,
        showing,
        disappearing,
        disappeared
    };

    dsMessageBox(
        const std::wstring& message,
        GLfloat width,
        GLfloat height
    );
    void render();
    std::wstring message;
    dsTimer timer;
private:
    GLfloat x, y;
    GLfloat width, height;
    GLfloat lasting_time;
    Status status;
};

class dsMessageBoxManager {
public:
    void addMessage(
        const std::wstring& message,
        GLfloat width = 200.0f,
        GLfloat height = 60.0f
    );

    // 每次绘制时调用
    void render();

private:
    std::list<dsMessageBox> message_boxes;
};

#endif