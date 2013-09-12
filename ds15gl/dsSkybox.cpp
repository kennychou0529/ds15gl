#include "dsSkybox.h"
#include "dsTexture.h"
#include "tinyxml2.h"

void DSSkybox::del() {
    // 如果 显示列表 已存在，则删除之
    if (glIsList(display_list)) {
        glDeleteLists(display_list, 1);
    }

    // 清空原来的天空盒 纹理
    for (GLuint i = 0; i < 6; ++i) {
        if (glIsTexture(texture[i])) {
            glDeleteTextures(1, texture + i);
        }
    }
}

void DSSkybox::load(GLuint index) {

    // 原来可能已经存在天空盒，需要清空
    del();

    GLuint texture_height, texture_width;

    // 先载入纹理，将纹理编号存入 texture[]
    tinyxml2::XMLDocument doc;
    doc.LoadFile("skyboxes.xml");
    auto root = doc.FirstChildElement();
    auto item = root->FirstChildElement("skybox");
    for (; item != nullptr; item = item->NextSiblingElement()) {

        // 找到了该序号的天空盒信息
        if (item->UnsignedAttribute("id") == index) {

            texture[0] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t0")->GetText(),
                             &texture_height,
                             &texture_width
                         );
            texture[1] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t1")->GetText()
                         );
            texture[2] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t2")->GetText()
                         );
            texture[3] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t3")->GetText()
                         );
            texture[4] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t4")->GetText()
                         );
            texture[5] = dsLoadTextureBMP2D(
                             item->FirstChildElement("t5")->GetText()
                         );
            item->FirstChildElement("depth")->QueryDoubleText(&depth);

            break;
        }
    }

    // 再创建显示列表
    width = 2000;
    height = width / (GLdouble)texture_width * (GLdouble)texture_height;

    GLdouble x = width / 2;

    display_list = glGenLists(1);

    glNewList(display_list, GL_COMPILE); // GL_COMPILE_AND_EXECUTE
    {
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);

        // 地面
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(1, 0);
            glVertex3d(- x, - x, - depth);
            glTexCoord2d(1, 1);
            glVertex3d(x, - x, - depth);
            glTexCoord2d(0, 1);
            glVertex3d(x, x, - depth);
            glTexCoord2d(0, 0);
            glVertex3d(- x, x, - depth);
        }
        glEnd();

        // 东面
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0, 0);
            glVertex3d(x, x, - depth);
            glTexCoord2d(0, 1);
            glVertex3d(x, x, height - depth);
            glTexCoord2d(1, 1);
            glVertex3d(x, - x, height - depth);
            glTexCoord2d(1, 0);
            glVertex3d(x, - x, - depth);
        }
        glEnd();

        // 西面
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0, 0);
            glVertex3d(- x, - x, - depth);
            glTexCoord2d(0, 1);
            glVertex3d(- x, - x, height - depth);
            glTexCoord2d(1, 1);
            glVertex3d(- x, x, height - depth);
            glTexCoord2d(1, 0);
            glVertex3d(- x, x, - depth);
        }
        glEnd();

        // 南面
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0, 0);
            glVertex3d(x, - x, - depth);
            glTexCoord2d(0, 1);
            glVertex3d(x, - x, height - depth);
            glTexCoord2d(1, 1);
            glVertex3d(- x, - x, height - depth);
            glTexCoord2d(1, 0);
            glVertex3d(- x, - x, - depth);
        }
        glEnd();

        // 北面
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0, 0);
            glVertex3d(- x, x, - depth);
            glTexCoord2d(0, 1);
            glVertex3d(- x, x, height - depth);
            glTexCoord2d(1, 1);
            glVertex3d(x, x, height - depth);
            glTexCoord2d(1, 0);
            glVertex3d(x, x, - depth);
        }
        glEnd();

        // 顶面
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBegin(GL_QUADS);
        {
            glTexCoord2d(1, 1);
            glVertex3d(- x, -x, height - depth);
            glTexCoord2d(1, 0);
            glVertex3d(x, -x, height - depth);
            glTexCoord2d(0, 0);
            glVertex3d(x, x, height - depth);
            glTexCoord2d(0, 1);
            glVertex3d(- x, x, height - depth);
        }
        glEnd();

        //glEnable(GL_LIGHTING);
        glPopAttrib();
    }
    glEndList();
}

void DSSkybox::show() {
    glCallList(display_list);
}