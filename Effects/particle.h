#ifndef _PARTICLE_H
#define _PARTICLE_H


#include <list>
#include <cmath>
using std::list;
const int colorIndexLength = 256;


struct Vector {
    float x, y, z;


    Vector operator +(Vector& a) {
        Vector sum;
        sum.x = x + a.x;
        sum.y = y + a.y;
        sum.z = z + a.z;
        return sum;
    }
    void unit() {
        float l = sqrt(x * x + y * y + z + z);
        if (l > 0) {
            x /= l;
            y /= l;
            z /= l;
        }
    }
    Vector operator -(Vector& a) {
        Vector sum;
        sum.x = x - a.x;
        sum.y = y - a.y;
        sum.z = z - a.z;
        return sum;
    }
    Vector operator *(float m) {
        Vector product;
        product.x = x * m;
        product.y = y * m;
        product.z = z * m;
        return product;
    }
    //���
    Vector operator *(Vector m) {
        Vector product;
        product.x = y * m.z - z * m.y;
        product.y = z * m.x - x * m.z;
        product.z = z * m.y + y * m.x;
        return product;
    }
    static Vector RotationToDirection(float yaw, float pitch) {
        Vector t;
        t.x = -sin(yaw) * cos(pitch);
        t.y = sin(pitch);
        t.z = cos(pitch) * cos(yaw);
        return t;
    }
};


struct Color {
    float r, g, b, a;
};

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
        float posx, float posy, float posz,
        //����Ƕ�
        float _yaw , float _yawVar,
        float _pich, float _pitchVar,
        //���ӳ����ٶ�
        float _particleSpeed,
        //��������
        float particleLife,
        //������ɫ�仯
        Color color[]
    );
    ~Emitter();

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

	void setCenter(float x, float y, float z,float a) {
		forceType |= Center;
		center.x = x;
		center.y = y;
		center.z = z;
		centripetal = a;
	}

	void setHoriForce(float a){
		forceType |= Z_Axle;
		centripetal = a;
	}


    void update(float duration);
    void draw() ;
private:
    //  long id; //EMITTER ID
    //
    //  char name[80];// EMITTER NAME
    //
    //  long flags; //EMITTER FLAGS

    float alive;
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

    float life, lifeVar; // LIFE COUNT AND VARIATION


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
};

#endif