#ifndef _DSSTATUSBAR_H
#define _DSSTATUSBAR_H

#include <GL/glut.h>
#include "dsTimer.h"

class DSStatusBar {
public:
    DSStatusBar();
    ~DSStatusBar();
    void show();
    void init();
private:
    GLuint logo_tex;
    GLdouble ratio;
    GLuint logo_display_list;

    GLuint bar_tex;
    //GLuint bar_display_list;
    GLuint bar_height;
    void renderBackground();
};

#endif
