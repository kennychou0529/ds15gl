#include "particle.h"
#include <GL/glut.h>
#include <ctime>

float randF() {
    return (rand() % 512 - 256) / 256.0f;
}

Emitter::Emitter() {
    Vector _pos = { -10, -10, 0};
    Vector _speed = { 0, 0, 0};
    Vector _particleSpeed = {10, 10, 0};
    Vector _forcce = {0.1, -0.1, 0};
    pos = _pos;
    speed = _speed;
    speedVar = 0;
    particleSpeed = _particleSpeed;
    particleSpeedVar = 5.f;
    life = 1;
    lifeVar = 0.3;
    emitsPerFrame = 100;
    emitVar = 5;
    for (int i = 0; i < colorIndexLength; i++) {
        colors[i].r = i / 255.0f;
        colors[i].g = i / 255.0f;
        colors[i].b = 1 - i / 255.0f;
        colors[i].a = 1;
    }
    srand(time(NULL));

    force = _forcce;

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
        it.colorIndex += (randF()+1) * 256 * duration;
        if (it.colorIndex < 0) {
            it.colorIndex = 0;
        } else if (it.colorIndex >= colorIndexLength) {
            it.colorIndex = colorIndexLength - 1;
        }
        it.pos = it.pos + it.dir * duration;
        it.pos.x += randF() * it.randLevel * duration;
        it.pos.y += randF() * it.randLevel * duration;
        it.pos.z += randF() * it.randLevel * duration;

        it.dir = it.dir + force * duration;

        it.life -= duration;
    }
    for (int i = 0; i < emitsPerFrame; i++) {
        //      Vector _speed = {0, 0, 0};
        Vector _particleSpeed = particleSpeed;
        _particleSpeed.x += particleSpeedVar * randF();
        _particleSpeed.y += particleSpeedVar * randF();
        _particleSpeed.z += particleSpeedVar * randF();

        Particle p = {pos, 4, particleSpeed, life + lifeVar * randF(), 0, colors};
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