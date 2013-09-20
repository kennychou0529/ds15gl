#include "dsMessageBox.h"

static const size_t num_messages = 10;
static const GLfloat showing_alpha = 0.4f;
static const GLfloat fading_speed = 1.0f;

dsMessageBox::dsMessageBox(const std::wstring& message)
    : message(message),
      status(appearing),
      width(400.0f),
      height(400.0f),
      x(400.0f),
      y(400.0f),
      lasting_time(4.0f) {
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
    glVertex2f(x, y);
    glVertex2f(x, y + height);
    glVertex2f(x + width, y + height);
    glVertex2f(x + width, y);
    glEnd();
    glPopMatrix();
    glPopAttrib();
    pop_projection_matrix();
}