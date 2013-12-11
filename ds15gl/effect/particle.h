#ifndef _PARTICLE_H
#define _PARTICLE_H


#include <list>
#include <cmath>
#include "vector.h"
#include <ctime>

using std::list;
const int colorIndexLength = 256;


struct Particle

{
    //Particle*prev,*next; // 双向链表， 指向前前驱结点和后继结点

    Vector pos; // 当前位置
    //
    //  Vector prevPos;//之前位置

    float randLevel; //方向变化的随机程度

    Vector dir; // 当前方向和速度

    float life; // 还会存在的时间

    unsigned int colorIndex;

    //Color* colors;

    //  Color color; //粒子当前颜色
    //
    //  Color prevColor; //粒子之前颜色
    //
    //  Color deltaColor; //颜色改变量
    void init(Vector _pos, float _randLevel, Vector _dir, float _life) {
        pos = _pos;
        randLevel = _randLevel;
        dir = _dir;
        life = _life;
        colorIndex = 0;
        //colors = _colors;
    }
};



//粒子池
class ParticlePool {
public:
    ParticlePool();
    ~ParticlePool();
    void apend();
    Particle* getParticle();
    void recycle(Particle*);
private:
    //总数
    int particleMax;
    //剩余数量
    int particles;

    list<Particle*> pool;
};

//三种力
enum ForceType {
    Gravity = 1, //平行力
    Center = 2,  //有心力
    Z_Axle = 4, //水平力,指向z轴
    Lorentz  = 8 //洛伦兹力
};

//发射器
class  Emitter

{
public:
    Emitter(//位置信息
        /*float posx, float posy, float posz,*/
        //发射角度
        float _yaw , float _yawVar,
        float _pich, float _pitchVar,
        //粒子初设速度
        float _particleSpeed,
        //粒子寿命
        float particleLife,
        //粒子颜色变化
        Color color[],
        float life
    );
    ~Emitter();

    void setSpeed(float vx, float vy,  float vz) {
        speed.x = vx;
        speed.y = vy;
        speed.z = vz;
    }

    void setPosition(float posx, float posy, float posz) {
        pos.x = posx;
        pos.y = posy;
        pos.z = posz;
    }

    void setMagnetic(float x, float y, float z) {
        forceType |= Lorentz;
        mag.x = x;
        mag.y = y;
        mag.z = z;
    }

    void setGravity(float x, float y, float z) {
        forceType |= Gravity;
        force.x = x;
        force.y = y;
        force.z = z;
    }

    void setCenter(float x, float y, float z, float a) {
        forceType |= Center;
        center.x = x;
        center.y = y;
        center.z = z;
        centripetal = a;
    }

    void setHoriForce(float a) {
        forceType |= Z_Axle;
        centripetal = a;
    }


    void update(float duration);
    void draw() ;
    bool isalive() {
        return alive;
    }

    void setPool(ParticlePool* pool) {
        pPool = pool;
    }

    void setG2e(bool flag) {
        g2e = flag;
    }

private:
    ParticlePool* pPool;
    //  如果life为0，则不发射粒子但不死亡，当粒子为0时，改发射器死亡
    float life;

    bool alive;
    //TRANSFORMATION INFO

    Vector pos; //XYZ POSITION

    //     float yaw, yawVar;// YAW AND VARIATION
    //
    //     float pitch, pitchVar; // PITCH AND VARIATION

    Vector speed;
    float speedVar;

    // Particle

    //  Particle* particle; // NULL TERMINATED LINKED LIST

    list<Particle*> particles;

    //int totalParticles; // TOTAL EMITTED AT ANY TIME

    //int particleCount; // TOTAL EMITTED RIGHT NOW

    int emitsPerFrame, emitVar; // EMITS PER FRAME AND VARIATION

    float plife, plifeVar; // LIFE COUNT AND VARIATION


    //   Vector particleDir;
    float particleSpeed;
    float yaw, yawVar;// YAW AND VARIATION
    float pitch, pitchVar; // PITCH AND VARIATION
    //Color startColor, startColorVar; // CURRENT COLOR OF PARTICLE

    //Color endColor,endColorVar; // CURRENT COLOR OF PARTICLE

    Color* colors;

    // Physics

    int forceType;

    Vector force; //GLOBAL GRAVITY, WIND, ETC.
    Vector center; //有心力中心
    Vector mag;//磁场
    float centripetal;//向心加速度大小
    clock_t lastTime ;
    bool g2e; //重力是否作用于发射器
};

#endif