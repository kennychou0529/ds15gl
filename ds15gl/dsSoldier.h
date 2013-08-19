#ifndef _DSSOLDIER_H
#define _DSSOLDIER_H

#include "dsModel.h"
#include "dsTimeManager.h"

class dsSoldier {
public:
    dsSoldier();
    void renderFrame(size_t frame_index);
    void renderSmoothly(size_t frame1_index, size_t frame2_index, GLfloat percentage);
    void renderSmoothly(GLfloat progress);
    void load(const char* person_model_file,
              const char* person_skin_file,
              const char* weapon_model_file,
              const char* weapon_skin_file);
    dsTimeManager time_manager;
private:
    MD2Model person;
    MD2Model weapon;
    size_t frame_beg;
    size_t frame_end;
};

#endif