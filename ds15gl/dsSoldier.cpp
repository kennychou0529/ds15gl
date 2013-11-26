#include "dsSoldier.h"
#include "dsFrame.h"
#include "dsMap.h"
#include "dsVector2f.h"
#include "tinyxml2.h"
#include <cmath>
#include <functional>
#include "dsVector.h"
#include <iostream>
#include "dsTexture.h"

/*
const int SABER = 0;              // 剑士
const int SOLDIER = 1;            // 突击兵
const int ARCHER = 2;             // 狙击手
const int AIRPLANE = 3;           // 战机
const int TANK = 4;               // 坦克
const int WIZARD = 5;             // 治疗师
const int BERSERKER = 6;          // 狂战士
const int ASSASSIN = 7;           // 暗杀者
const int ARCHMAGE = 8;           // 大法师
*/

//这里需要排一下
char kind[9][10]= {"SABER","SOLDIER","ARCHER","AIRPLANE",
	"TANK","WIZARD","BERSERKER","ASSASSIN","ARCHMAGE"};


static const GLfloat pi = 3.1415926f;

extern DSFrame frame;

//GLuint dsSoldier::aureole = dsLoadTextureBMP2D("data/images/aureole.bmp");

dsSoldier::dsSoldier(int _idNumber) :
    move_speed(20.0f),
    scale(0.2f),
    angle(0.0f),
    playing(nullptr),
    idNumber(_idNumber) {
    enterStatus(idle);
    hp = 15;
    hp_max = 15;
    beSelected = false;
}

void dsSoldier::renderFrame(size_t frame_index) const {
    person.renderFrame(frame_index);
    weapon.renderFrame(frame_index);
}

void dsSoldier::renderSmoothly(
    size_t frame1_index,
    size_t frame2_index,
    GLfloat percentage
) const {
    person.renderSmoothly(frame1_index, frame2_index, percentage);

    // 测试表明，武器模型的死亡动画有问题，所以不画了
    if (status == dying || status == died || !has_weapon) {
        return;
    }
    weapon.renderSmoothly(frame1_index, frame2_index, percentage);
}

void dsSoldier::renderSmoothly(GLfloat progress) const {
    if (frame_beg == frame_end) {
        renderSmoothly(frame_beg, frame_beg, 0);
    } else {
        size_t frame1_index = static_cast<size_t>(std::floor(progress));
        GLfloat percentage = progress - frame1_index;
        frame1_index %= (frame_end - frame_beg + 1);
        size_t frame2_index = (frame1_index + 1) % (frame_end - frame_beg + 1);
        frame1_index += frame_beg;
        frame2_index += frame_beg;
        renderSmoothly(frame1_index, frame2_index, percentage);
    }
}

void dsSoldier::enterStatus(Status status_to_enter, int* script_playing) {
    playing = script_playing;

    // Status 中：
    // idle, died, disappear 不属于过程
    // running, attacking, pain, dying 属于过程
    bool prev_playing = status != idle && status != died && status != disappear;

    status = status_to_enter;
    bool cur_playing = status != idle && status != died && status != disappear;

    timer.recordTime();

    frame_beg = std::get<0>(frame_set[status]);
    frame_end = std::get<1>(frame_set[status]);
    fps = std::get<2>(frame_set[status]);

    // 从非过程变成过程，需要 playing 增
    // 从过程变成非过程，需要 playing 减
    if (!prev_playing && cur_playing) {
        if (playing != nullptr) {
            ++(*playing);
        }
    } else if (prev_playing && !cur_playing) {
        if (playing != nullptr) {
            --(*playing);
        }
    }
    ALfloat x, y;

    frame.scene.map.getCoords(current_position[0], current_position[1], &x, &y);

    //需要播放声音
    switch (status_to_enter) {
    case dsSoldier::idle:
        break;
    case dsSoldier::running:
        sound._run_record = frame.sounds.playSound(sound._run, x, y, 0);
        break;
    case dsSoldier::attacking:
        break;
    case dsSoldier::pain:
        frame.sounds.playSound(sound._pain, x, y, 1);
        break;
    case dsSoldier::dying:
        frame.sounds.playSound(sound._dying, x, y, 1);
        break;
    case dsSoldier::died:
        break;
    case dsSoldier::disappear:
        break;
    default:
        break;
    }

}

void dsSoldier::animate(bool selectMode) {

    if (selectMode) {
        glPushName(idNumber + 1000);
    }
    glPushMatrix();
    {
        GLfloat x, y;
        GLfloat duration = timer.getDurationSecf();

        auto render = [&]() {
            glPushMatrix();
            glTranslatef(x, y, translate);
            glScaled(scale, scale, scale);
            glRotatef(angle + default_angle, 0.0f, 0.0f, 1.0f);
            renderSmoothly(duration * fps);
            glPopMatrix();
			//hpBar(x, y, 12);
			hpBar2(x,y);

            drawAureole(x, y);
        };

        // 这里没有使用 case 语句，是因为会出现对象初始化问题
        if (status == idle || status == died) {
            frame.scene.map.getCoords(
                current_position[0], current_position[1], &x, &y
            );
            render();

        } else if (status == running) {
            frame.scene.map.renderArrow(
                target_position[0],
                target_position[1],
                duration
            );

            dsVector2f saved;
            frame.scene.map.getCoords(
                saved_position[0], saved_position[1], &(saved.x), &(saved.y)
            );

            dsVector2f target;
            frame.scene.map.getCoords(
                target_position[0], target_position[1], &(target.x), &(target.y)
            );

            dsVector2f dir = target - saved;
            GLfloat length = dir.getLenth();
            dir.normalise();

            dsVector2f pos = saved + move_speed * duration * dir;

            angle = std::acos(dir.x) * 180.0f / pi;
            if (std::abs(std::sin(angle / 180.0f * pi) - dir.y) > 0.01f) {
                angle = -angle;
            }

            x = pos.x;
            y = pos.y;

            render();
			//声源跟随人物
            DSSoundManager::changePosition(sound._run_record, x, y);
            if (move_speed * duration > length) {
                pos = target;
                setPosition(target_position[0], target_position[1]);
                enterStatus(idle, playing);
                frame.sounds.stop(sound._run_record); //结束脚步声
            }
            

        } else if (status == attacking || status == pain) {
            frame.scene.map.getCoords(
                current_position[0], current_position[1], &x, &y
            );
            render();
            if (fps * duration > (frame_end - frame_beg)) {
                enterStatus(idle, playing);
            }

        } else if (status == dying) {
            frame.scene.map.getCoords(
                current_position[0], current_position[1], &x, &y
            );
            render();
            if (fps * duration > (frame_end - frame_beg)) {
                enterStatus(died, playing);
            }

        } // endif
    }
    glPopMatrix();
    if (selectMode) {
        glPopName();
    }
}

void dsSoldier::load(
    const std::string& person_model_file,
    const std::string& person_skin_file,
    const std::string& weapon_model_file,
    const std::string& weapon_skin_file
) {
    has_weapon = true;
    person.load(person_model_file, person_skin_file);
    weapon.load(weapon_model_file, weapon_skin_file);
}

void dsSoldier::load(
    const std::string& model_file,
    const std::string& skin_file
) {
    has_weapon = false;
    person.load(model_file, skin_file);
}

void dsSoldier::load(const std::string& soldier_name, size_t team) {
    tinyxml2::XMLDocument doc;
	kind = soldier_name;
    doc.LoadFile("soldiers.xml");
    auto root = doc.FirstChildElement();
    auto soldier = root->FirstChildElement("soldier");
    for (; soldier != nullptr; soldier = soldier->NextSiblingElement("soldier")) {
        if (soldier_name == soldier->Attribute("name")) {
            if (soldier->FirstChildElement("weapon") == nullptr) {
                // 该士兵没有武器模型
                if (team == 1) {
                    load(
                        soldier->FirstChildElement("tris")->GetText(),
                        soldier->FirstChildElement("tris_skin")->GetText()
                        );
                } else {
                    load(
                        soldier->FirstChildElement("tris")->GetText(),
                        soldier->FirstChildElement("tris_skin2")->GetText()
                        );
                }
            } else {
                // 该士兵有武器模型
                if (team == 1) {
                    load(
                        soldier->FirstChildElement("tris")->GetText(),
                        soldier->FirstChildElement("tris_skin")->GetText(),
                        soldier->FirstChildElement("weapon")->GetText(),
                        soldier->FirstChildElement("weapon_skin")->GetText()
                        );
                } else {
                    load(
                        soldier->FirstChildElement("tris")->GetText(),
                        soldier->FirstChildElement("tris_skin2")->GetText(),
                        soldier->FirstChildElement("weapon")->GetText(),
                        soldier->FirstChildElement("weapon_skin")->GetText()
                        );
                }
            }
            auto xml_frame_set = soldier->FirstChildElement("frame_set");
            if (xml_frame_set == nullptr) {
                throw "Frame_set invalid";
            }

            frame_set[idle]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("idle")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("idle")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("idle")->UnsignedAttribute("fps")
                  );
            frame_set[running]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("running")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("running")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("running")->UnsignedAttribute("fps")
                  );
            frame_set[attacking]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("attacking")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("attacking")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("attacking")->UnsignedAttribute("fps")
                  );
            frame_set[pain]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("pain")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("pain")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("pain")->UnsignedAttribute("fps")
                  );
            frame_set[dying]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("dying")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("dying")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("dying")->UnsignedAttribute("fps")
                  );
            frame_set[died]
                = std::make_tuple(
                      xml_frame_set->FirstChildElement("died")->UnsignedAttribute("beg"),
                      xml_frame_set->FirstChildElement("died")->UnsignedAttribute("end"),
                      xml_frame_set->FirstChildElement("died")->UnsignedAttribute("fps")

                  );
            soldier->FirstChildElement("default_angle")->QueryFloatText(&default_angle);
            soldier->FirstChildElement("scale")->QueryFloatText(&scale);
            soldier->FirstChildElement("translate")->QueryFloatText(&translate);

            return;
        }
    }
}

void dsSoldier::setPosition(size_t x, size_t y) {
    current_position[0] = x;
    current_position[1] = y;
}

void dsSoldier::setTarget(size_t x, size_t y) {
    saved_position[0] = current_position[0];
    saved_position[1] = current_position[1];

    target_position[0] = x;
    target_position[1] = y;
}

extern GLdouble up[3];
extern GLdouble center[3];
extern GLdouble eye[3];

//hpBar需要画在人物头顶正上方，方向总与视线垂直
void dsSoldier::hpBar(GLfloat x, GLfloat y, GLfloat z) {

    static double hpBarWidth = 0.5;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    //glLoadIdentity();
    glPushMatrix();

    glTranslatef(x , y, z);

    GLdouble direction[3];
    dsDiff3dv(center, eye, direction);

    static GLdouble xline[] = {1.0, 0, 0};
    static GLdouble yline[] = {0, 1.0, 0};
    GLdouble direction_t[] = {direction[0], direction[1], 0};
    //计算旋转角度
    GLdouble rr = dsIncludedAngle2dv(yline, direction_t);
    GLdouble n[3];
    dsCross3dv(yline, direction, n);
    if (n[2] < 0) {
        rr = -rr;
    }
    //printf("%f %f ",n[2],rr);
    glRotated(rr, 0, 0, 1.0);

    glTranslatef(-hp_max / 2 * (hpBarWidth + 0.2), 0, 0);
    for (int i = 0; i < hp_max; i++) {
        glColor3f(0, 0, 1.0f);
        glutWireCube(hpBarWidth);
        if (i < hp) {
            glColor3f(0, 1.0, 0);
            glutSolidCube(hpBarWidth - 0.01);
        }
        glTranslatef(hpBarWidth + 0.2, 0, 0);
    }

    //auxSolidCylinder()
    //auxSolidCylinder(hpBarWidth,hpBarLenth,3,1);

    glPopMatrix();
    glPopAttrib();

}

void dsSoldier::hpBar2(float x,float y) {
    GLdouble winx, winy, winz;
    GLdouble point[3];
	point[0]=x;
	point[1]=y;
    point[2] = 11;
    /*frame.scene.map.getCoords(
        current_position[0], current_position[1], point, point + 1
    );*/

    GLdouble dir1[3];
    GLdouble dir2[3];
    dsDiff3dv(center, eye, dir1);
    dsDiff3dv(point, eye, dir2);
    if (dsDot3dv(dir1, dir2) < 0) {
        return;
    }

    GLdouble projection_matrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

    GLdouble modelview_matrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject(
        point[0], point[1], point[2],
        modelview_matrix,
        projection_matrix,
        viewport,
        &winx, &winy, &winz
    );
    // 经测试，返回的 winx, winy 是像素值
    if (winz < 0) {
        return;
    }

    // 储存并替换投影矩阵
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewport[2], 0, viewport[3]);

    glMatrixMode(GL_MODELVIEW);

    glPushAttrib(GL_ENABLE_BIT);
    glPushMatrix();
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glLoadIdentity();
        glBegin(GL_POLYGON);
        {
            glVertex2d(winx - 40, winy);
            glVertex2d(winx + 40, winy);
            glVertex2d(winx + 40, winy + 5);
            glVertex2d(winx - 40, winy + 5);
        }
        glEnd();
    }
    glPopMatrix();
    glPopAttrib();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

GLfloat dsSoldier::setOrientation(size_t x, size_t y) {
    dsVector2f dir(
        (int)x - (int)current_position[0],
        (int)y - (int)current_position[1]
    );
    dir.normalise();

    if (dir[0] == 0.0 && dir[1] == 0.0) {
        return 0.0;
    }

    angle = std::acos(dir.x) * 180.0f / pi;
    if (std::abs(std::sin(angle / 180.0f * pi) - dir.y) > 0.01f) {
        angle = -angle;
    }

    

    return angle;
}

void dsSoldier::drawAureole(GLfloat x, GLfloat y) {
    if (!beSelected) {
        return;
    }
    static GLfloat angle = 0;
    angle += 0.5f;
    if (angle >= 360) {
        angle = 0;
    }
    glPushMatrix();
    //glLoadIdentity();
    glTranslatef(x, y, 0.1f);
    glRotatef(angle, 0, 0, 1);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glBindTexture(GL_TEXTURE_2D,aureole);
    glColor4f(1.f, 1.f, 0.f,0.5f);
	glLineWidth(6);
    glBegin(GL_LINE_LOOP);
    {

        glVertex2d(0.f, 6.f);
        glVertex2d(-5.2f, -3.f);
        glVertex2d(5.2, -3.f);

    }
    glEnd();
	//glRotatef(-2*angle, 0, 0, 1);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2d(0.f, -6.f);
        glVertex2d(5.2f, 3.f);
        glVertex2d(-5.2, 3.f);
    }
    glEnd();
	glRotatef(-2*angle, 0, 0, 1);
	glRectf(-2.f,-2.f,2.f,2.f);
    glPopAttrib();
    //  glEnable(GL_TEXTURE_2D);
    //  glEnable(GL_LIGHTING);
    glPopMatrix();
}