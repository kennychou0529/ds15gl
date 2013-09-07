#include "dsSoldier.h"
#include "dsFrame.h"
#include "dsMap.h"
#include "dsVector2f.h"
#include <cmath>

static const GLfloat pi = 3.1415926f;

extern DSFrame frame;

dsSoldier::dsSoldier() : frame_beg(40), frame_end(45), fps(5), move_speed(20.0f), scale(0.2f), angle(0.0f), playing(nullptr) {}

void dsSoldier::renderFrame(size_t frame_index) {
    person.renderFrame(frame_index);
    weapon.renderFrame(frame_index);
}

void dsSoldier::renderSmoothly(size_t frame1_index, size_t frame2_index, GLfloat percentage) {
    person.renderSmoothly(frame1_index, frame2_index, percentage);
    weapon.renderSmoothly(frame1_index, frame2_index, percentage);
}

void dsSoldier::renderSmoothly(GLfloat progress) {
    size_t frame1_index = static_cast<size_t>(std::floor(progress));
    GLfloat percentage = progress - frame1_index;
    frame1_index %= (frame_end - frame_beg + 1);
    size_t frame2_index = (frame1_index + 1) % (frame_end - frame_beg + 1);
    frame1_index += frame_beg;
    frame2_index += frame_beg;
    renderSmoothly(frame1_index, frame2_index, percentage);
}

void dsSoldier::enterStatus(Status status_to_enter, int* script_playing) {
    playing = script_playing;
    status = status_to_enter;
    timer.recordTime();

    switch (status) {
    case idle:
        frame_beg = 0;
        frame_end = 39;
        fps = 10;
        break;
    case running:
        frame_beg = 40;
        frame_end = 45;
        fps = 5;
        if (playing != nullptr) {
            ++(*playing);
        }
        break;
    case attacking:
        frame_beg = 46;
        frame_end = 53;
        fps = 10;
        if (playing != nullptr) {
            ++(*playing);
        }
        break;
    }
}

void dsSoldier::animate() {
    glPushMatrix();
    {
        // 这里没有使用 case 语句，是因为会出现对象初始化问题
        if (status == idle) {
            GLfloat x, y;
            frame.scene.map.getCoords(current_position[0], current_position[1], &x, &y);

            glTranslatef(x, y, 4.0f);
            glScaled(scale, scale, scale);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            renderSmoothly(timer.getDurationSecf() * fps);
        } else if (status == running) {
            dsVector2f saved;
            frame.scene.map.getCoords(saved_position[0], saved_position[1], &(saved.x), &(saved.y));

            dsVector2f target;
            frame.scene.map.getCoords(target_position[0], target_position[1], &(target.x), &(target.y));

            dsVector2f dir = target - saved;
            GLfloat length = dir.getLenth();
            dir.normalise();

            GLfloat duration = timer.getDurationSecf();
            dsVector2f pos = saved + move_speed * duration * dir;

            angle = std::acos(dir.x) * 180.0f / pi;
            if (std::abs(std::sin(angle / 180.0f * pi) - dir.y) > 0.01f) {
                angle = -angle;
            }

            glTranslatef(pos.x, pos.y, 4.0f);
            glScaled(scale, scale, scale);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            renderSmoothly(timer.getDurationSecf() * fps);
            if (move_speed * duration > length) {
                pos = target;
                setPosition(target_position[0], target_position[1]);
                enterStatus(idle, playing);
                if (playing != nullptr) {
                    --(*playing);
                }
            }
        } else if (status == attacking) {
            GLfloat x, y;
            frame.scene.map.getCoords(current_position[0], current_position[1], &x, &y);

            glTranslatef(x, y, 4.0f);
            glScaled(scale, scale, scale);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);

            GLfloat duration = timer.getDurationSecf();
            renderSmoothly(duration * fps);
            if (fps * duration > (frame_end - frame_beg + 1)) {
                enterStatus(idle, playing);
                if (playing != nullptr) {
                    --(*playing);
                }
            }
        } // endif
    }
    glPopMatrix();
}

void dsSoldier::load(const char* person_model_file,
                     const char* person_skin_file,
                     const char* weapon_model_file,
                     const char* weapon_skin_file)
{
    person.load(person_model_file, person_skin_file);
    weapon.load(weapon_model_file, weapon_skin_file);
}

void dsSoldier::setPosition(size_t x, size_t y) {
    current_position[0] = x;
    current_position[1] = y;
}

void dsSoldier::setTarget(size_t x, size_t y) {
    saved_position[0] = current_position[0];
    saved_position[1] = current_position[1];

    target_position[0] = x;
    target_position[1] = y;
}