#include "dsSoldier.h"
#include "dsFrame.h"
#include "dsMap.h"
#include "dsVector2f.h"
#include "tinyxml2.h"
#include <cmath>
#include <functional>

static const GLfloat pi = 3.1415926f;

extern DSFrame frame;

dsSoldier::dsSoldier() :
    move_speed(20.0f),
    scale(0.2f),
    angle(0.0f),
    playing(nullptr) {
    enterStatus(idle);
}

void dsSoldier::renderFrame(size_t frame_index) {
    person.renderFrame(frame_index);
    weapon.renderFrame(frame_index);
}

void dsSoldier::renderSmoothly(
    size_t frame1_index,
    size_t frame2_index,
    GLfloat percentage
) {
    person.renderSmoothly(frame1_index, frame2_index, percentage);

    // 测试表明，武器模型的死亡动画有问题，所以不画了
    if (status == dying || status == died || !has_weapon) {
        return;
    }
    weapon.renderSmoothly(frame1_index, frame2_index, percentage);
}

void dsSoldier::renderSmoothly(GLfloat progress) {
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

}

void dsSoldier::animate() {
    glPushMatrix();
    {
        GLfloat x, y;
        GLfloat duration = timer.getDurationSecf();

        auto render = [&]() {
            glTranslatef(x, y, translate);
            glScaled(scale, scale, scale);
            glRotatef(angle + default_angle, 0.0f, 0.0f, 1.0f);
            renderSmoothly(duration * fps);
        };

        // 这里没有使用 case 语句，是因为会出现对象初始化问题
        if (status == idle || status == died) {
            frame.scene.map.getCoords(
                current_position[0], current_position[1], &x, &y
            );
            render();

        } else if (status == running) {
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
            if (move_speed * duration > length) {
                pos = target;
                setPosition(target_position[0], target_position[1]);
                enterStatus(idle, playing);
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

void dsSoldier::load(const std::string& soldier_name) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("soldiers.xml");
    auto root = doc.FirstChildElement();
    auto soldier = root->FirstChildElement("soldier");
    for (; soldier != nullptr; soldier = soldier->NextSiblingElement()) {
        if (soldier_name == soldier->Attribute("name")) {
            if (soldier->FirstChildElement("weapon") == nullptr) {
                // 该士兵没有武器模型
                load(
                    soldier->FirstChildElement("tris")->GetText(),
                    soldier->FirstChildElement("tris_skin")->GetText()
                );
            } else {
                // 该士兵有武器模型
                load(
                    soldier->FirstChildElement("tris")->GetText(),
                    soldier->FirstChildElement("tris_skin")->GetText(),
                    soldier->FirstChildElement("weapon")->GetText(),
                    soldier->FirstChildElement("weapon_skin")->GetText()
                );
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