#ifndef _DSSOLDIER_H
#define _DSSOLDIER_H

#include "dsModel.h"
#include "dsTimer.h"

// 士兵
class dsSoldier {
public:
    enum Status {
        idle, running
    };

    dsSoldier();

    // 绘制帧 frame_index
    void renderFrame(size_t frame_index);

    // 带有线性插值的绘制，使得动画流畅。
    // 当 percentage = 0 时，绘制 frame1；
    // 当 percentage = 1 时，绘制 frame2；
    // 当 percentage 介于 0 和 1 之间时，绘制两者之间的情形。
    // 例如 percentage = 0.2 时，绘制的图形更接近 frame1
    void renderSmoothly(size_t frame1_index, size_t frame2_index, GLfloat percentage);

    // 带有线性插值的绘制，参数 progress 是一个小数化的“帧数”。
    // 例如当前 dsSoldier 私有参数 frame_beg 和 frame_end 分别为 0 和 39，
    // 则当 proress 为 0 时，绘制 帧 0；
    // 当 progress 为 5.7 时，绘制 帧 '5.7'，即 renderSmoothly(5, 6, 0.7)；
    // 当 progress 为 39.5 时，renderSmoothly(39, 0, 0.5)。
    // 
    // 再例如当前 dsSoldier 私有参数 frame_beg 和 frame_end 分别为 10 和 49,
    // 则当 proress 为 0 时，绘制 帧 10；
    // 当 progress 为 5.7 时，绘制 帧 '15.7'，即 renderSmoothly(15, 16, 0.7)；
    // 当 progress 为 39.5 时，renderSmoothly(49, 10, 0.5)。
    void renderSmoothly(GLfloat progress);

    void enterStatus(Status status_to_enter, int* script_playing = nullptr);

    void animate();

    // 载入士兵模型文件，只支持 bmp 类型的纹理图片
    void load(const char* person_model_file,
              const char* person_skin_file,
              const char* weapon_model_file,
              const char* weapon_skin_file);

    dsTimer timer;

    void setPosition(size_t x, size_t y);

    void setTarget(size_t x, size_t y);

private:
    size_t fps;
    Status status;
    MD2Model person;
    MD2Model weapon;
    size_t frame_beg;
    size_t frame_end;
    size_t current_position[2];
    size_t saved_position[2];
    size_t target_position[2];
    GLfloat move_speed;
    GLfloat scale;
    GLfloat angle;
    // bool* finished;
    int* playing;
};

#endif