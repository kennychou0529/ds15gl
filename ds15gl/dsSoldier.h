#ifndef _DSSOLDIER_H
#define _DSSOLDIER_H

#include "dsModel.h"
#include "dsTimer.h"

// ʿ��
class dsSoldier {
public:
    enum Status {
        idle, running
    };

    dsSoldier();

    // ����֡ frame_index
    void renderFrame(size_t frame_index);

    // �������Բ�ֵ�Ļ��ƣ�ʹ�ö���������
    // �� percentage = 0 ʱ������ frame1��
    // �� percentage = 1 ʱ������ frame2��
    // �� percentage ���� 0 �� 1 ֮��ʱ����������֮������Ρ�
    // ���� percentage = 0.2 ʱ�����Ƶ�ͼ�θ��ӽ� frame1
    void renderSmoothly(size_t frame1_index, size_t frame2_index, GLfloat percentage);

    // �������Բ�ֵ�Ļ��ƣ����� progress ��һ��С�����ġ�֡������
    // ���統ǰ dsSoldier ˽�в��� frame_beg �� frame_end �ֱ�Ϊ 0 �� 39��
    // �� proress Ϊ 0 ʱ������ ֡ 0��
    // �� progress Ϊ 5.7 ʱ������ ֡ '5.7'���� renderSmoothly(5, 6, 0.7)��
    // �� progress Ϊ 39.5 ʱ��renderSmoothly(39, 0, 0.5)��
    // 
    // �����統ǰ dsSoldier ˽�в��� frame_beg �� frame_end �ֱ�Ϊ 10 �� 49,
    // �� proress Ϊ 0 ʱ������ ֡ 10��
    // �� progress Ϊ 5.7 ʱ������ ֡ '15.7'���� renderSmoothly(15, 16, 0.7)��
    // �� progress Ϊ 39.5 ʱ��renderSmoothly(49, 10, 0.5)��
    void renderSmoothly(GLfloat progress);

    void enterStatus(Status status_to_enter);

    void animate();

    // ����ʿ��ģ���ļ���ֻ֧�� bmp ���͵�����ͼƬ
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
};

#endif