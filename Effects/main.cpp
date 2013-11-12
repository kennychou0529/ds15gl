#include <GL/glut.h>
#include <iostream>
//#include "tparticle.h"
#include "particle.h"
#include "EffectManager.h"

using namespace std;
//tEmitter* em;

EffectManager em;
Vector center;
Vector eye;
//tParticle* m_ParticlePool;

// void renderParticles(tEmitter* emitter) {
//     if (emitter == NULL) {
//         return;
//     }
//     addParticle(emitter);
//     tParticle* p = emitter->particle->next;
//  glPointSize(2.0f);
//     glBegin(GL_POINTS);
//     while (p != NULL) {
//         updateParticle(p, emitter);
//         glColor3f(p->color.r, p->color.g, p->color.b);
//         glVertex3f(p->pos.x, p->pos.y, p->pos.z);
//         p = p->next;
//     }
//     glEnd();
// }


void dsDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    Vector eyeP;
    eyeP.x = eye.z * sin(eye.x) * cos(eye.y);
    eyeP.y = eye.z * sin(eye.x) * sin(eye.y);
    eyeP.z = eye.z * cos(eye.x);
    gluLookAt(eyeP.x, eyeP.y , eyeP.z, center.x, center.y, center.z, 0, 1, 0);

    //extern void renderParticles(tEmitter* emitter);
    //updateParticle()
    //renderParticles(em);
    em.render();

    glutSwapBuffers();

    // 打印 GL 错误
    GLenum errCode;
    const GLubyte* errString;

    while ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        std::cerr << errString << std::endl;
    }
}

void init() {
    center.x = 0, center.y = 0, center.z = 0;
    eye.x = 1, eye.y = 1, eye.z = 30;

    Color* colors = new Color[256];
    for (int i = 0; i < colorIndexLength; i++) {
        colors[i].r = i / 255.0f;
        colors[i].g = i / 255.0f;
        colors[i].b = 1 - i / 255.0f;
        colors[i].a = 1 -  1.3f * (i / 256.0f)*(i / 256.0f);
    }
    Emitter emm( 1, 0.1, 1,0.5, 4, 2, colors,2);
	emm.setPosition(0,0,1);
	emm.setSpeed(0,0,-1);
    //Vector mag = {0, 0.01, 0};
	emm.setGravity(0,0,-1);
	emm.setCenter(10,10,-1,3);

    emm.setMagnetic(0,0.5,0);
	em.addEmitter(emm);

	Thurder t(0,0,0,3,0,3,10,2,0.2,100);
	em.addThurder(t);

    //     em = new tEmitter;
    //     em->emitsPerFrame = 1000;
    //     em->emitVar = 50;
    //     em->endColor.r = 0;
    //     em->endColor.g = 0;
    //     em->endColor.b = 1;
    //     em->flags  = 11;
    //     em->force.x = 0;
    //     em->force.y = 0;
    //     em->force.z = 0;
    //     em->id  = 11;
    //     em->life = 100;
    //     em->lifeVar = 50;
    //     //em->name="test";
    //     em->particleCount = 0;
    //     em->pitch = 1;
    //     em->pitchVar = 0.5f;
    //     em->yaw = 1;
    //     em->yaw = 0.5f;
    //     em->pos.x = 0;
    //     em->pos.y = 0;
    //     em->pos.z = 0;
    //     em->speed = 0.1;
    //     em->speedVar  = 0.01;
    //     em->totalParticles = 100000;
    //     em->startColor.r = 1;
    //     em->startColor.g = 1;
    //     em->startColor.b = 0;
    //     em->startColorVar.r = 0;
    //     em->startColorVar.g = 0;
    //     em->startColorVar.b = 0;
    //  em->endColorVar.r = 0;
    //  em->endColorVar.g = 0;
    //  em->endColorVar.b = 0;
    //     em->particle = NULL;
    //     initParticlePool();
}

// 当窗口大小被修改时自动调用此函数
void dsReshape(int w, int h) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 4 / 3.0f, 2, 200);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void dsIdle() {
    glutPostRedisplay();
}

void keyFunc(unsigned char c,int x,int y){
	switch (c)
	{
	case 'a':
		eye.x+=0.1;
		break;
	case 'd':
		eye.x-=0.1;
		break;
	case 'w':
		eye.y+=0.1;
		break;
	case 's':
		eye.y-=0.1;
		break;
	case 'q':
		eye.z+=1;
		break;
	case 'e':
		eye.z-=1;
		break;
	default:
		break;
	}

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    //glClearColor(255,1,1,1);
    glutInitWindowSize(800, 600);
    glutCreateWindow("DS 15th Effect Test");

    glutDisplayFunc(dsDisplay);
	glutKeyboardFunc(keyFunc);
    glutReshapeFunc(dsReshape);
    glutIdleFunc(dsIdle);
    init();
    glutMainLoop();
   // delete emm;
    return 0;
}