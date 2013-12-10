#include "particle.h"
#include <GL/glut.h>
#include <ctime>

float randF() {
    return (rand() % 512 - 256) / 256.0f;
}

ParticlePool::ParticlePool() {
    //最开始有20000个粒子
    particles = particleMax = 20000;
    for (int i = 0; i < particleMax; i++) {
        Particle* p = new Particle;
        pool.push_front(p);
    }
}
ParticlePool::~ParticlePool() {
    for (Particle * p : pool) {
        delete p;
    }
    pool.clear();
}
void ParticlePool::apend() {
    for (int i = 0; i <  particleMax; i++) {
        Particle* p = new Particle;
        pool.push_front(p);
    }
    particles += particleMax;
    particleMax *= 2;
}
Particle*  ParticlePool::getParticle() {
    //如果粒子没有了，就扩大池子容量
    if (particles <= 0) {
        apend();
    }
    Particle* p = pool.front();
    pool.pop_front();
    particles--;
    return p;
}
void ParticlePool::recycle(Particle* p) {
    pool.push_front(p);
    particles++;
}



Emitter::Emitter(//位置信息
    //float posx, float posy, float posz,

    //发射角度
    float _yaw , float _yawVar,
    float _pich, float _pitchVar,
    //粒子初设速度
    float _particleSpeed,
    //粒子寿命
    float particleLife,
    //粒子颜色变化
    Color _color[],
    float _life
) {
    this->life = life;
    Vector _pos = {0, 0, 0};
    Vector _speed = { 0, 0, 0};
    //    Vector _particleSpeed = {10, 10, 0};
    //     Vector _forcce = {forcex, forcey, forcez};
    //     Vector _center = {centerx, centery, centerz};
    pos = _pos;
    speed = _speed;
    speedVar = 0;

    yaw = _yaw;
    yawVar = _yawVar;

    pitch = _pich;
    pitchVar = _pitchVar;
    particleSpeed = _particleSpeed;
    //     particleSpeedVar = 15.f;
    plife = particleLife;
    life = _life;
    plifeVar = 0.3;
    emitsPerFrame = 30;
    emitVar = 5;
    //dlc = 256 / life;
    colors = _color;
    //     for (int i = 0; i < colorIndexLength; i++) {
    //         colors[i].r = i / 255.0f;
    //         colors[i].g = i / 255.0f;
    //         colors[i].b = 1 - i / 255.0f;
    //         colors[i].a = 1 -  1.3f * (i / 256.0f);
    //     }
    srand(time(NULL));

    forceType = 0;
    //     force = _forcce;
    //     center = _center;
    //centripetal = _centripetal;

    alive = true;
    lastTime = clock();

}

Emitter::~Emitter() {
    for (Particle*& p : particles) {
        pPool->recycle(p);
    }
    particles.clear();
}

void Emitter::update(float duration) {
    if (!alive) {
        return;
    }

    pos = pos + speed * duration;

    while (!particles.empty() && particles.front()->life <= 0) {
        pPool->recycle(particles.front());
        particles.pop_front();
    }

    for (Particle*& it : particles) {
        it->colorIndex = (1 - it->life / plife) * 256;
        if (it->colorIndex < 0) {
            it->colorIndex = 0;
        } else if (it->colorIndex >= colorIndexLength) {
            it->colorIndex = colorIndexLength - 1;
        }
        it->pos = it->pos + it->dir * duration;
        it->pos.x += randF() * it->randLevel * duration;
        it->pos.y += randF() * it->randLevel * duration;
        it->pos.z += randF() * it->randLevel * duration;
        if (forceType & Lorentz) {
            Vector v = it->dir * mag;
            it->pos = it->pos + v;
        }
        if (forceType & Gravity) {
            it->dir = it->dir + force * duration;
        }
        if (forceType & Center) {
            Vector t = center - it->pos;
            t.unit();
            it->dir = it->dir + t * centripetal * duration;
        } else if (forceType & Z_Axle) {
            float l = sqrt(it->pos.x * it->pos.x + it->pos.y * it->pos.y);
            Vector t = {0, 0, 0};
            if (l > 0) {
                t.x = it->pos.x / l;
                t.y = it->pos.y / l;
            }
            it->dir = it->dir + t * centripetal * duration;
        }


        it->life -= duration;
    }

    if (life > 0) {
        for (int i = 0; i < emitsPerFrame; i++) {

            float _yaw, _pitch, _speed;
            //      Vector _speed = {0, 0, 0};
            Vector _particleSpeed =  Vector::RotationToDirection(yaw + yawVar * randF(), pitch + pitchVar * randF()) * particleSpeed ;

            Particle* p = pPool->getParticle();
            p->init(pos, 4, _particleSpeed , plife + plifeVar * randF());

            particles.push_back(p);
        }
        life -= duration;
    } else {
        if (particles.empty()) {
            alive = false;
        }
    }
}

void Emitter::draw() {
    //static clock_t lastTime = clock();
    clock_t nowTime = clock();
    float duration = (nowTime - lastTime) / 1000.0f;
    update(duration);
    if (!alive) {
        return;
    }

    glLineWidth(2.0);
    glPointSize(1.0);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 2 / 255.0);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_POINT_SPRITE_ARB);

    /* glBegin(GL_POINTS);
     for (Particle & it : particles) {
         Color& c = it.colors[it.colorIndex];
         glColor4f(c.r, c.g, c.b,c.a);
         glVertex3f(it.pos.x, it.pos.y, it.pos.z);
     }
     glEnd();*/
    glPushMatrix();
    glBegin(GL_LINES);
    int s = 20;
    for (Particle * it : particles) {
        int i = it->colorIndex;
        Color& c = colors[i];

        glColor4f(c.r, c.g, c.b, c.a);
        glVertex3f(it->pos.x, it->pos.y, it->pos.z);
        glVertex3f(it->pos.x + it->dir.x / s, it->pos.y + it->dir.y / s, it->pos.z + it->dir.z / s);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);

    lastTime = nowTime;
}

