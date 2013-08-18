#ifndef _DSSOLDIER_H
#define _DSSOLDIER_H

#include "dsModel.h"
#include "dsTimeManager.h"

class dsSoldier {
public:
    dsSoldier();
    void renderFrame(unsigned int frame_index);
    void renderSmoothly(int frame1_index, int frame2_index, GLfloat percentage);
    void renderSmoothly(float progress);
    void load(const char* person_model_file, const char* person_skin_file, const char* weapon_model_file, const char* weapon_skin_file);
    dsTimeManager time_manager;
private:
    MD2Model person;
    MD2Model weapon;
    unsigned int frame_beg;
    unsigned int frame_end;
};

#endif