#include "dsSoldier.h"
#include <cmath>

dsSoldier::dsSoldier() : frame_beg(0), frame_end(39) {}

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

void dsSoldier::load(const char* person_model_file,
                     const char* person_skin_file,
                     const char* weapon_model_file,
                     const char* weapon_skin_file)
{
    person.load(person_model_file, person_skin_file);
    weapon.load(weapon_model_file, weapon_skin_file);
}
