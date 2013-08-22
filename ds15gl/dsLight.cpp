#include "dsLight.h"
#include <GL/glut.h>

// 环境光，能保证光源找不到的地方也看得到，这个得设置得暗一些，我们设成暗淡的白色
GLfloat light_ambient[4]  = {0.2f, 0.2f, 0.2f, 1.0f};

// 漫射光
GLfloat light_diffuse[4]  = {0.5f, 250.0f / 255 / 2, 200.0f / 255 / 2, 1.0f};

// 镜面光，产生亮斑
GLfloat light_specular[4] = {0.5f, 250.0f / 255 / 2, 200.0f / 255 / 2, 2000.0f};
// GLfloat specular[4] = {1.0f, 1.0f, 0.0f, 1.0f};
// GLfloat specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// 光源位置
GLfloat light_position[4] = {5.0f, 0.0f, 5.0f, 1.0f};
// GLfloat position[4] = {5.0f, 0.0f, 5.0f, 1.0f};

void dsSetLight() {
    // 创建光源
    glEnable(GL_LIGHT0);
    // 将数据载入光源 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 聚光灯
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
    GLfloat spot_direction[] = { - light_position[0], - light_position[1], - light_position[2]};
    // GLfloat spot_direction[] = {0.0, 0.0, -1};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
}

void dsSetMaterial() {
    // 环境光
    GLfloat ambient[4]  = {1.0f, 1.0f, 1.0f, 1.0f};

    // 漫射光
    GLfloat diffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f};

    // 镜面光，产生亮斑
    GLfloat specular[4] = {0.0f, 0.0f, 0.0f, 1000.0f};

    GLfloat shinness = 5.0;

    // 将数据载入材质
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinness);
}

void dsShowLightSource() {
    glPushAttrib(GL_TEXTURE_2D | GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    /*glBegin(GL_POINTS);
    {
        glPointSize(5.0f);
        glVertex3fv(position);
    }
    glEnd();*/
    glPushMatrix();
    //glLoadIdentity();
    glColor3d(1.0, 1.0, 1.0);
    glTranslated(light_position[0], light_position[1], light_position[2]);

    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
    glPopAttrib();
}
