#include "particle.h"
#include <GL/glut.h>
#include <ctime>

float randF() {
    return (rand() % 512 - 256) / 256.0f;
}

Emitter::Emitter() {
    Vector _pos = { -10, -10, 0};
    Vector _speed = { 0, 0, 0};
    //    Vector _particleSpeed = {10, 10, 0};
    Vector _forcce = {0.1, -0.1, 0};
    Vector _center = {10, -10, 10};
    pos = _pos;
    speed = _speed;
    speedVar = 0;

    yaw = 1;
    yawVar = 3.14;

    pitch = 1;
    pitchVar = 3.14;
    particleSpeed = 5;
    //     particleSpeedVar = 15.f;
    life = 2;
    lifeVar = 0.3;
    emitsPerFrame = 500;
    emitVar = 5;
    for (int i = 0; i < colorIndexLength; i++) {
        colors[i].r = i / 255.0f;
        colors[i].g = i / 255.0f;
        colors[i].b = 1 - i / 255.0f;
        colors[i].a = 1;
    }
    srand(time(NULL));

    forceType = Gravity | Center;
    force = _forcce;
    center = _center;
    centripetal = 15;

    alive = true;

}

void Emitter::update(float duration) {
    if (!alive) {
        return;
    }

    pos = pos + speed;

    while (!particles.empty() && particles.front().life <= 0) {
        particles.pop_front();
    }
    for (Particle & it : particles) {
        it.colorIndex += (randF() + 1) * 128 * duration;
        if (it.colorIndex < 0) {
            it.colorIndex = 0;
        } else if (it.colorIndex >= colorIndexLength) {
            it.colorIndex = colorIndexLength - 1;
        }
        it.pos = it.pos + it.dir * duration;
        it.pos.x += randF() * it.randLevel * duration;
        it.pos.y += randF() * it.randLevel * duration;
        it.pos.z += randF() * it.randLevel * duration;
        if (forceType & Gravity) {
            it.dir = it.dir + force * duration;
        }
        if (forceType & Center) {
            Vector t = center - it.pos;
            t.unit();
            it.dir = it.dir + t * centripetal * duration;
        } else if (forceType & Z_Axle) {
            float l = sqrt(it.pos.x * it.pos.x + it.pos.y * it.pos.y);
            Vector t = {0, 0, 0};
            if (l > 0) {
                t.x = it.pos.x / l;
                t.y = it.pos.y / l;
            }
            it.dir = it.dir + t * centripetal * duration;
        }

        it.life -= duration;
    }
    for (int i = 0; i < emitsPerFrame; i++) {

        float _yaw, _pitch, _speed;
        //      Vector _speed = {0, 0, 0};
        Vector _particleSpeed =  Vector::RotationToDirection(yaw+yawVar*randF(), pitch+pitchVar*randF()) * particleSpeed ;

        Particle p = {pos, 4,_particleSpeed , life + lifeVar * randF(), 0, colors};

        particles.push_back(p);
    }
}

void Emitter::draw() {
    static clock_t lastTime = clock();
    clock_t nowTime = clock();
    float duration = (nowTime - lastTime) / 1000.0f;
    update(duration);
    if (!alive) {
        return;
    }
    glPointSize(1.f);
    glBegin(GL_POINTS);
    for (Particle & it : particles) {
        Color& c = it.colors[it.colorIndex];
        glColor3f(c.r, c.g, c.b);
        glVertex3f(it.pos.x, it.pos.y, it.pos.z);
    }
    glEnd();
    lastTime = nowTime;
}

