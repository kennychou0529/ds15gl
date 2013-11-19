#include <fstream>
#include "dsFrame.h"
#include "display_basic.h"

DSFrame::DSFrame() {}


void DSFrame::initialize(const std::string& file_name) {


    num_frames = 0;
    fps = 0;
    timer.recordTime();
    statusBar.init();
    //scene.map.init(15, 10);
    sounds.loadSounds();
    //把这些初始化工作集中在这里，以备Socket使用
    //
    //     Game_Info info;
    //     Round_Begin_Info begin_info;
    //     Command cmd;
    //     Round_End_Info end_info;
    //
    //     // 回放文件读入
    //     std::ifstream is(file_name);
    //
    //     // 读入地图信息
    //     is >> info.map_size[0] >> info.map_size[1];
    //     for (size_t i = 0; i < info.map_size[0]; i++)
    //         for (size_t j = 0; j < info.map_size[1]; j++) {
    //             is >> info.map[i][j];
    //         }
    //
    //     is >> info.team_name[0] >> info.team_name[1];
    //     for (size_t i = 0; i < 2; ++i) {
    //         is >> info.soldier_number[i];
    //         for (size_t j = 0; j < info.soldier_number[i]; ++j)
    //             is >> info.soldier[j][i].kind
    //                >> info.soldier[j][i].life
    //                >> info.soldier[j][i].strength
    //                >> info.soldier[j][i].defence
    //                >> info.soldier[j][i].move_range
    //                >> info.soldier[j][i].attack_range[0]
    //                >> info.soldier[j][i].attack_range[1]
    //                >> info.soldier[j][i].duration
    //                >> info.soldier[j][i].pos.x
    //                >> info.soldier[j][i].pos.y;
    //     }

    // 初始信息读入完成

    scene.map.init(10, 10);
    //scene.map.init(&info);
    scene.map.load();
    scene.actors.initialize();
    scene.initialize();
}

extern bool isReady;
bool canRefresh = false;
void DSFrame::initialize2(const std::string& rep_file_name) {
    isReady = false;
	//必须等frame停止渲染才能重设场景
	while(!canRefresh){
		Sleep(100);
	}	
    Game_Info info;
    Round_Begin_Info begin_info;
    Command cmd;
    Round_End_Info end_info;

    // 回放文件读入
    std::ifstream is(rep_file_name);

    // 读入地图信息
    is >> info.map_size[0] >> info.map_size[1];
    for (size_t i = 0; i < info.map_size[0]; i++)
        for (size_t j = 0; j < info.map_size[1]; j++) {
            is >> info.map[i][j];
        }

    is >> info.team_name[0] >> info.team_name[1];
    for (size_t i = 0; i < 2; ++i) {
        is >> info.soldier_number[i];
        for (size_t j = 0; j < info.soldier_number[i]; ++j)
            is >> info.soldier[j][i].kind
               >> info.soldier[j][i].life
               >> info.soldier[j][i].strength
               >> info.soldier[j][i].defence
               >> info.soldier[j][i].move_range
               >> info.soldier[j][i].attack_range[0]
               >> info.soldier[j][i].attack_range[1]
               >> info.soldier[j][i].duration
               >> info.soldier[j][i].pos.x
               >> info.soldier[j][i].pos.y;
    }

    // 初始信息读入完成

    //处理人物
    //全部隐藏
    actors.clearAll();

    //依次载入
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < info.soldier_number[i]; ++j) {
            try {
                string soldier = actors.getSoldierByKind(info.soldier[j][i].kind);
                actors.list[soldier]->setHP(info.soldier[j][i].life, info.soldier[j][i].life);
                actors.list[soldier]->setPosition(info.soldier[j][i].pos.x, info.soldier[j][i].pos.y) ;
            } catch (char* err) {
                cerr << err;
            }
        }
    }

    //scene.map.init(8, 8);
    scene.map.init(&info);
	is.close();
}


void DSFrame::display(bool selectMode) {
    // 计算 FPS，每秒统计一次
    calculateFPS();
    scene.show(selectMode);
    statusBar.show();
}

void DSFrame::calculateFPS() {
    ++num_frames;
    if (timer.getDurationSecf() > 1.0f) {
        fps = static_cast<size_t>(
                  static_cast<GLfloat>(num_frames) / timer.getDurationSecf()
              );
        num_frames = 0;
        timer.recordTime();
    }
}
