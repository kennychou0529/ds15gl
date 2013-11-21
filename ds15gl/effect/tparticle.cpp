#include "tparticle.h"
#include <gl/glut.h>


float RandomNum() {
    return (rand() % 512 - 256) / 256.0f;
}

///////////////////////////////////////////////////////////////////////////////

// Function:RotationToDirection

// Purpose:Convert a Yaw and Pitch to a direction vector

///////////////////////////////////////////////////////////////////////////////

void RotationToDirection(float pitch, float yaw, tVector* direction) {

    direction->x = -sin(yaw) * cos(pitch);

    direction->y = sin(pitch);

    direction->z = cos(pitch) * cos(yaw);

}

/// initParticleSystem////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

// Function:addParticle

// Purpose: adda particle to an emitter

// Arguments:The emitter to add to

///////////////////////////////////////////////////////////////////////////////

tParticle* m_ParticlePool;

void initParticlePool() {
    m_ParticlePool = new tParticle;
    m_ParticlePool->next = NULL;
    for (int i = 0; i < 20000; i++) {
        tParticle* p = new tParticle;

        p->prev = NULL;
        p->next = m_ParticlePool;

        m_ParticlePool ->prev = p;
        m_ParticlePool = p;
    }
}

bool addParticle(tEmitter* emitter) {

    /// LocalVariables ///////////////////////////////////////////////////////////

    tParticle* particle;

    tColor start, end;

    float yaw, pitch, speed;

    ///////////////////////////////////////////////////////////////////////////////

    // IF THERE ISAN EMITTER AND A PARTICLE IN THE POOL

    // AND I HAVEN'TEMITTED MY MAX

    if (emitter != NULL && m_ParticlePool != NULL &&

            emitter->particleCount < emitter->totalParticles)

    {

        particle = m_ParticlePool; // THE CURRENT PARTICLE

        m_ParticlePool = m_ParticlePool->next; // FIX THE POOL POINTERS

        if (emitter->particle != NULL)

        {
            emitter->particle->prev = particle;    // SET BACK LINK
        }

        particle->next = emitter->particle; // SET ITS NEXT POINTER

        particle->prev = NULL; // IT HAS NO BACK POINTER

        emitter->particle = particle; // SET IT IN THE EMITTER

        particle->pos.x = 0.0f; // RELATIVE TO EMITTER BASE

        particle->pos.y = 0.0f;

        particle->pos.z = 0.0f;

        particle->prevPos.x = 0.0f; // USED FOR ANTI ALIAS

        particle->prevPos.y = 0.0f;

        particle->prevPos.z = 0.0f;

        // CALCULATE THESTARTING DIRECTION VECTOR

        yaw = emitter->yaw + (emitter->yawVar * RandomNum());

        pitch = emitter->pitch + (emitter->pitchVar * RandomNum());

        // CONVERT THEROTATIONS TO A VECTOR

        RotationToDirection(pitch, yaw, &particle->dir);

        // MULTIPLY INTHE SPEED FACTOR

        speed = emitter->speed + (emitter->speedVar * RandomNum());

        particle->dir.x *= speed;

        particle->dir.y *= speed;

        particle->dir.z *= speed;

        // CALCULATE THECOLORS

        start.r = emitter->startColor.r + (emitter->startColorVar.r * RandomNum());

        start.g = emitter->startColor.g + (emitter->startColorVar.g * RandomNum());

        start.b = emitter->startColor.b + (emitter->startColorVar.b * RandomNum());

        end.r = emitter->endColor.r + (emitter->endColorVar.r * RandomNum());

        end.g = emitter->endColor.g + (emitter->endColorVar.g * RandomNum());

        end.b = emitter->endColor.b + (emitter->endColorVar.b * RandomNum());

        particle->color.r = start.r;

        particle->color.g = start.g;

        particle->color.b = start.b;

        // CALCULATE THELIFE SPAN

        particle->life = emitter->life + (int)((float)emitter->lifeVar * RandomNum());

        // CREATE THECOLOR DELTA

        particle->deltaColor.r = (end.r - start.r) / particle->life;

        particle->deltaColor.g = (end.g - start.g) / particle->life;

        particle->deltaColor.b = (end.b - start.b) / particle->life;

        emitter->particleCount++;// A NEW PARTICLE IS BORN

        return true;

    }

    return false;

}

/// addParticle///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

// Function:updateParticle

// Purpose:updateParticle settings

// Arguments:The particle to update and the emitter it came from

///////////////////////////////////////////////////////////////////////////////

bool updateParticle(tParticle* particle, tEmitter* emitter) {

    // IF THIS IS AVALID PARTICLE

    if (particle != NULL && particle->life > 0)

    {

        // SAVE ITS OLDPOS FOR ANTI ALIASING

        particle->prevPos.x = particle->pos.x;

        particle->prevPos.y = particle->pos.y;

        particle->prevPos.z = particle->pos.z;

        // CALCULATE THENEW

        particle->pos.x += particle->dir.x;

        particle->pos.y += particle->dir.y;

        particle->pos.z += particle->dir.z;

        // APPLY GLOBALFORCE TO DIRECTION

        particle->dir.x += emitter->force.x;

        particle->dir.y += emitter->force.y;

        particle->dir.z += emitter->force.z;

        // SAVE THE OLDCOLOR

        particle->prevColor.r = particle->color.r;

        particle->prevColor.g = particle->color.g;

        particle->prevColor.b = particle->color.b;

        // GET THE NEWCOLOR

        particle->color.r += particle->deltaColor.r;

        particle->color.g += particle->deltaColor.g;

        particle->color.b += particle->deltaColor.b;

        particle->life --; //IT IS A CYCLE OLDER

        return true;

    }

    else if (particle != NULL && particle->life == 0)

    {

        // FREE THISSUCKER UP BACK TO THE MAIN POOL

        if (particle->prev != NULL)

        {
            particle->prev->next = particle->next;
        }

        else

        {
            emitter->particle = particle->next;
        }

        // FIX UP THENEXTÕS PREV POINTER IF THERE IS A NEXT

        if (particle->next != NULL)

        {
            particle->next->prev = particle->prev;
        }

        particle->next = m_ParticlePool;

        m_ParticlePool = particle; // NEW POOL POINTER

        emitter->particleCount--;// ADD ONE TO POOL

    }

    return false;

}

/// updateParticle///////////////////////////////////////////////////////////////


