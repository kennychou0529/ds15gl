#include "dsMessageBox.h"
#include "dsTextManager.h"
#include "dsTools.h"

static const size_t num_messages = 10;
static const GLfloat showing_alpha = 0.6f;
static const GLfloat fading_speed = 2.0f;
static const GLfloat default_lasting_time = 2.0f;

dsMessageBox::dsMessageBox(
    const std::wstring& _message,
    GLfloat width,
    GLfloat height
):
    message(_message),
    status(showing),
    width(width),
    height(height),
    lasting_time(default_lasting_time) {
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
    GLfloat alpha;

    switch (status) {
    case disappeared:
        return;
        break;
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
    }

    x = static_cast<GLfloat>(window_width) / 2.0f;
    y = window_height - height - 50.0f;
    extern dsTextManager dstext;

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

    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x - width / 2, y);
    glVertex2f(x - width / 2, y + height);
    glVertex2f(x - width / 2 + width, y + height);
    glVertex2f(x - width / 2 + width, y);
    glEnd();

    glColor4f(0.0f, 0.0f, 0.0f, alpha / showing_alpha);
    dstext.print(x  - width / 2 + 20.0f, y + 13.0f, message);

    glPopMatrix();
    glPopAttrib();
    pop_projection_matrix();


}

void dsMessageBoxManager::addMessage(
    const std::wstring& message,
    GLfloat width,
    GLfloat height
) {
    message_boxes.clear();
    message_boxes.push_back(
        dsMessageBox(message, width, height)
    );
}

void dsMessageBoxManager::render() {
    if (message_boxes.empty()) {
        return;
    }

    if (message_boxes.front().status == dsMessageBox::disappeared) {
        message_boxes.pop_front();
    }

    if (message_boxes.empty()) {
        return;
    }

    message_boxes.front().render();
}
