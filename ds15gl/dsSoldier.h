#ifndef _DSSOLDIER_H
#define _DSSOLDIER_H

#include <tuple>
#include <utility>
#include <string>
#include "dsModel.h"
#include "dsTimer.h"
#include <sstream>
#include "display_basic.h"

extern char kind[9][10];

struct SoundIDandSource {
    int _run;
    unsigned int _run_record;
    int _pain;
    int _fight;
    int _dying;
};

//unsigned int footsteps;     //脚步声

// 士兵
class dsSoldier {
public:
    enum Status {
        idle,       // 站着
        running,    // 跑动
        attacking,  // 攻击
        pain,       // 受到攻击
        dying,      // 倒地身亡
        died,       // 躺在地上
        disappear   // 不绘制
    };

    dsSoldier(int _idNumber  = 0);

    void setSounds(SoundIDandSource _soundsGroup) {
        sound = _soundsGroup;
    }

    // 绘制帧 frame_index
    void renderFrame(size_t frame_index) const;

    // 带有线性插值的绘制，使得动画流畅。
    // 当 percentage = 0 时，绘制 frame1；
    // 当 percentage = 1 时，绘制 frame2；
    // 当 percentage 介于 0 和 1 之间时，绘制两者之间的情形。
    // 例如 percentage = 0.2 时，绘制的图形更接近 frame1
    void renderSmoothly(
        size_t frame1_index,
        size_t frame2_index,
        GLfloat percentage
    ) const;

    // 带有线性插值的绘制，参数 progress 是一个小数化的“帧数”。
    // 例如当前 dsSoldier 私有参数 frame_beg 和 frame_end 分别为 0 和 39，
    // 则当 proress 为 0 时，绘制 帧 0；
    // 当 progress 为 5.7 时，绘制 帧 '5.7'，即 renderSmoothly(5, 6, 0.7)；
    // 当 progress 为 39.5 时，renderSmoothly(39, 0, 0.5)。
    //
    // 再例如当前 dsSoldier 私有参数 frame_beg 和 frame_end 分别为 10 和 49,
    // 则当 proress 为 0 时，绘制 帧 10；
    // 当 progress 为 5.7 时，绘制 帧 '15.7'，即 renderSmoothly(15, 16, 0.7)；
    // 当 progress 为 39.5 时，renderSmoothly(49, 10, 0.5)。
    void renderSmoothly(GLfloat progress) const;

    void enterStatus(Status status_to_enter, int* script_playing = nullptr);

    void animate(bool selectMode);

    void hpReduce(size_t reduce) {
        hp -= reduce;
        if (hp < 0) {
            hp = 0;
        }
    }

    // 载入士兵模型文件，只支持 bmp 类型的纹理图片
    void load(
        const std::string& person_model_file,
        const std::string& person_skin_file,
        const std::string& weapon_model_file,
        const std::string& weapon_skin_file
    );

    void load(
        const std::string& model_file,
        const std::string& skin_file
    );

    // 根据 XML 文件中的信息载入士兵模型文件
    void load(const std::string& soldier_name);

    dsTimer timer;

    void setPosition(size_t x, size_t y);

    void setTarget(size_t x, size_t y);

    void hpBar(GLfloat x, GLfloat y, GLfloat z);

    void hpBar2();

    std::wstring getInfo() {
        std::wostringstream os;
        os << L"HP     :" << hp << " / " << hp_max << L"\n";
        os << "Location:" << current_position[0] << "," << current_position[1] << "\n";
        return os.str();
    }

    void setSelected(bool _beSelected) {
        beSelected = _beSelected;
    }

    GLfloat setOrientation(size_t x, size_t y);

    //为被选中的人物画一个光环
    void drawAureole(GLfloat x, GLfloat y);
    friend class DSActorManager;

    void setHP(int max, int current) {
        hp_max = max;
        hp = current;
    }

	void setID(string _id){
		id=_id;
	}

private:
    int idNumber;               //为了启用选择模式，应该有一个编号
    string kind;
	string id;
    int hp;
    int hp_max;

    bool beSelected;            //是否被选中
    //static GLuint aureole;        //光环的贴图

    bool has_weapon;            // 是否含有武器模型
    Status status;              // 当前状态

    MD2Model person;            // 人物模型
    MD2Model weapon;            // 武器模型

    GLfloat move_speed;         // 移动速度
    GLfloat scale;              // 缩放倍数
    GLfloat angle;              // 当前旋转角
    GLfloat default_angle;      // 默认旋转角
    // 由于模型各不相同，需要一个修正角
    // 使得默认方向为 x 轴 正方向
    GLfloat translate;          // 高度移动值

    int* playing;               // 正在播放中的对象个数

    size_t frame_beg;           // 起始帧
    size_t frame_end;           // 末帧
    size_t fps;                 // 每秒播放的关键帧数

    size_t current_position[2]; // 当前地图坐标
    size_t saved_position[2];   // 储存的地图坐标
    size_t target_position[2];  // 正在走向的地图坐标 (在 running 状态中用)

    // 各个状态的起止帧号与播放速度 <frame_beg, frame_end, fps>
    std::tuple<size_t, size_t, size_t> frame_set[6];

    SoundIDandSource sound;

};


#endif