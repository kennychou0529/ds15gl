#ifndef _T_PARTIVCLE_H
#define _T_PARTIVCLE_H


#include <cmath>
#include <iostream>
#include <ctime>

struct tVector {
    float x, y, z;
};
struct tColor {
    float r, g, b;
};

struct tParticle{

    tParticle* prev, *next; // 双向链表， 指向前前驱结点和后继结点

    tVector pos; // 当前位置

    tVector prevPos;//之前位置

    tVector dir; // 当前方向和速度

    int life; // 还会存在的时间

    tColor color; //粒子当前颜色

    tColor prevColor; //粒子之前颜色

    tColor deltaColor; //颜色改变量

};

struct tEmitter{

    long id; //EMITTER ID

    char name[80];// EMITTER NAME

    long flags; //EMITTER FLAGS

    //TRANSFORMATION INFO

    tVector pos; //XYZ POSITION

    float yaw, yawVar;// YAW AND VARIATION

    float pitch, pitchVar; // PITCH AND VARIATION

    float speed, speedVar;

    // Particle

    tParticle* particle; // NULL TERMINATED LINKED LIST

    int totalParticles; // TOTAL EMITTED AT ANY TIME

    int particleCount; // TOTAL EMITTED RIGHT NOW

    int emitsPerFrame, emitVar; // EMITS PER FRAME AND VARIATION

    int life, lifeVar; // LIFE COUNT AND VARIATION

    tColor startColor, startColorVar; // CURRENT COLOR OF PARTICLE

    tColor endColor, endColorVar; // CURRENT COLOR OF PARTICLE

    // Physics

    tVector force; //GLOBAL GRAVITY, WIND, ETC.

};


extern void RotationToDirection(float pitch, float yaw, tVector* direction) ;
extern bool addParticle(tEmitter* emitter);
extern bool updateParticle(tParticle* particle, tEmitter* emitter);
//extern void renderParticles(tEmitter* emitter);
extern void initParticlePool();

#endif