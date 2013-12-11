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
    //Particle*prev,*next; // ˫������ ָ��ǰǰ�����ͺ�̽��

    Vector pos; // ��ǰλ��
    //
    //  Vector prevPos;//֮ǰλ��

    float randLevel; //����仯������̶�

    Vector dir; // ��ǰ������ٶ�

    float life; // ������ڵ�ʱ��

    unsigned int colorIndex;

    //Color* colors;

    //  Color color; //���ӵ�ǰ��ɫ
    //
    //  Color prevColor; //����֮ǰ��ɫ
    //
    //  Color deltaColor; //��ɫ�ı���
    void init(Vector _pos, float _randLevel, Vector _dir, float _life) {
        pos = _pos;
        randLevel = _randLevel;
        dir = _dir;
        life = _life;
        colorIndex = 0;
        //colors = _colors;
    }
};



//���ӳ�
class ParticlePool {
public:
    ParticlePool();
    ~ParticlePool();
    void apend();
    Particle* getParticle();
    void recycle(Particle*);
private:
    //����
    int particleMax;
    //ʣ������
    int particles;

    list<Particle*> pool;
};

//������
enum ForceType {
    Gravity = 1, //ƽ����
    Center = 2,  //������
    Z_Axle = 4, //ˮƽ��,ָ��z��
    Lorentz  = 8 //��������
};

//������
class  Emitter

{
public:
    Emitter(//λ����Ϣ
        /*float posx, float posy, float posz,*/
        //����Ƕ�
        float _yaw , float _yawVar,
        float _pich, float _pitchVar,
        //���ӳ����ٶ�
        float _particleSpeed,
        //��������
        float particleLife,
        //������ɫ�仯
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
    //  ���lifeΪ0���򲻷������ӵ���������������Ϊ0ʱ���ķ���������
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
    Vector center; //����������
    Vector mag;//�ų�
    float centripetal;//���ļ��ٶȴ�С
    clock_t lastTime ;
    bool g2e; //�����Ƿ������ڷ�����
};

#endif