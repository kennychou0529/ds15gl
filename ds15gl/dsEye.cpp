#include "dsEye.h"
#include "dsTimeManager.h"
#include "dsVector2f.h"
#include "dsVector.h"
#include "dsSoundManager.h"

// �ƶ��������ĵ�ʱ��ÿ���ƶ�����
static const double viewMoveSpeed = 40;

static const GLdouble pi = 3.1415926;

// ��ת�۾�ʱ��ÿ��ת������
static GLdouble rotateSpeed = 2;


// static GLdouble scaleSpeed = 0.5;

static dsTimeManager time_manager_for_special;
static dsTimeManager time_manager_for_eye;

extern DSSoundManager* soundManager;

// �۾���ת����
int rdir = STOP;

// ��������λ��ƽ�Ʒ���
int idir = STOP;

// ����ǰ�� / �������ӣ�ֻȡ UP DOWN STOP
int mdir = STOP;

bool moving = false;
static float targetX, targetY;

// �۾�λ�ã��������� (r, phi, theta) ��ʾ
// ���У�phi ��ʾ�� z ��ļн�
// theta ��ʾ�� xy ƽ���ͶӰ����ת�� 
GLdouble eye_sphere[3] = { 10.0, pi / 4, -pi / 2 };

static GLdouble eye_sphere_saved[3];

// �������ϵķ���
GLdouble up[3] = { 0.0, 0.0, 1.0 };

// �������ĵ㣬�������ԭ��
GLdouble center[3] = { 0.0, 0.0, 0.0 };

static GLdouble center_saved[3] = { 0.0, 0.0, 0.0 };

// ���λ�ã���ֱ���޸ģ�ͨ���������Ӳ���
GLdouble eye[3];

GLdouble direction[3];

static void saveEyeSphere() {
    time_manager_for_special.recordTime();
    eye_sphere_saved[0] = eye_sphere[0];
    eye_sphere_saved[1] = eye_sphere[1];
    eye_sphere_saved[2] = eye_sphere[2];
}

static void saveCenter() {
    time_manager_for_eye.recordTime();
    center_saved[0] = center[0];
    center_saved[1] = center[1];
    center_saved[2] = center[2];
}

static void saveDirection() {
    dsDiff3dv(center, eye, direction);
    dsNormalize3dv(direction);
    time_manager_for_eye.recordTime();
}

// ���浱ǰ�ӽ���Ϣ���� dsTools �еļ��̲����������õ�
void saveEyeInfo() {
    saveEyeSphere();
    saveCenter();
    saveDirection();
}

void centerMoveTo(float x, float y) {
    targetX = x;
    targetY = y;
    moving = true;
}

static void moveTo() {
    dsVector2f dir = dsVector2f(GLfloat(targetX - center[0]),
        GLfloat(targetY - center[1]));

    if (dir.getLenth() <= viewMoveSpeed){
        center[0] += dir.x;
        center[1] += dir.y;
        moving = false;
        return;
    }

    dir.normalise();
    center[0] += dir.x;
    center[1] += dir.y;
}

// ��ʱ�������������ĵ㣬�� dsSetEye �е���
static void dsCenterMove() {
    if (moving == true) {
        moveTo();
        return;
    }

    dsVector2f dir_up(GLfloat(center[0] - eye[0]), GLfloat(center[1] - eye[1]));
    dsVector2f dir_left = dir_up.turnLeft();

    dsVector2f dir_move(0.0f, 0.0f); // Moving direction
    if (idir & UP)
        dir_move += dir_up;
    if (idir & DOWN)
        dir_move -= dir_up;
    if (idir & LEFT)
        dir_move += dir_left;
    if (idir & RIGHT)
        dir_move -= dir_left;
    dir_move.normalise();

    int move_factor = 0;
    if (mdir & UP)
        ++move_factor;
    if (mdir & DOWN)
        --move_factor;

    double duration = time_manager_for_eye.getDurationSecd();
    center[0] = center_saved[0] + (direction[0] * move_factor + dir_move.x) * viewMoveSpeed * duration;
    center[1] = center_saved[1] + (direction[1] * move_factor + dir_move.y) * viewMoveSpeed * duration;
    center[2] = center_saved[2] + direction[2] * move_factor * viewMoveSpeed * duration;
}

// ��ʱ�������۾������꣬�� dsSetEye �е���
static void dsEyeRotate() {
    if (rdir == 0) {
        return;
    }

    int factor1 = 0, factor2 = 0;
    if (rdir & UP)
        ++factor1;
    if (rdir & DOWN)
        --factor1;
    if (rdir & LEFT)
        ++factor2;
    if (rdir & RIGHT)
        --factor2;

    double duration = time_manager_for_special.getDurationSecd();
    eye_sphere[1] = eye_sphere_saved[1] + factor1 * rotateSpeed * duration;
    if (eye_sphere[1] > pi - 0.01) {
        eye_sphere[1] = pi - 0.01;
    }
    if (eye_sphere[1] < 0.01) {
        eye_sphere[1] = 0.01;
    }
    eye_sphere[2] = eye_sphere_saved[2] + factor2 * rotateSpeed * duration;
}

void dsSetEye() {
    dsEyeRotate();
    dsCenterMove();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ��������ת��Ϊֱ������
    dsSphereToOrtho3dv(eye_sphere, center, eye);

    // ��������ͷλ��
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    // ������λ��
    soundManager->setListenerPosition(eye[0], eye[1], eye[2]);
}
