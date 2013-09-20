#include "dsMessageBox.h"
#include "dsTextManager.h"
#include "dsTools.h"

static const size_t num_messages = 10;
static const GLfloat showing_alpha = 0.4f;
static const GLfloat fading_speed = 1.0f;

dsMessageBox::dsMessageBox(const std::wstring& _message)
    : message(_message),
      status(appearing),
      width(250.0f),
      height(100.0f),
      x(400.0f),
      y(400.0f),
      lasting_time(4.0f) {
    message = L"Hello!";
    
    timer.recordTime();
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
    glPushAttrib(GL_TRANSFORM_BIT);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewport[2], 0, viewport[3]);
    glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

void dsMessageBox::render() {
    x = static_cast<GLfloat>(window_width - status_bar_width) / 2.0f;
    y = window_height - 50.0f;
    extern dsTextManager dstext;
    glPushMatrix();
    glLoadIdentity();
    
    glPopMatrix();

    pushScreenCoordinateMatrix();
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glLoadIdentity();
    GLfloat alpha;

    switch (status) {
    case appearing:
        alpha = fading_speed * timer.getDurationSecf();
        if (alpha > showing_alpha) {
            status = showing;
            timer.recordTime();
        }
        break;
    case showing:
        alpha = showing_alpha;
        if (timer.getDurationSecf() > lasting_time) {
            status = disappearing;
            timer.recordTime();
        }
        break;
    case disappearing:
        alpha = showing_alpha - fading_speed * timer.getDurationSecf();
        if (alpha < 0.0f) {
            status = disappeared;
        }
        break;
    case disappeared:
        glPopMatrix();
        glPopAttrib();
        pop_projection_matrix();
        return;
        break;
    }

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y);
    glVertex2f(x - width / 2, y + height);
    glVertex2f(x - width / 2 + width, y + height);
    glVertex2f(x - width / 2 + width, y);
    glEnd();

    glColor4f(0.0f, 0.0f, 1.0f, alpha / showing_alpha);
    dstext.print(x  - width / 2 + 10.0f, y + 10.0f, L"ROUND 1");

    glPopMatrix();
    glPopAttrib();
    pop_projection_matrix();

    
}