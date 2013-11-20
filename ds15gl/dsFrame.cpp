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
    while (!canRefresh) {
        Sleep(100);
    }
    int roundNum = 0;
    Game_Info info;
    Round_Begin_Info begin_info;
    Command cmd;
    Round_End_Info end_info;
    //从数字索引到stringID的映射
    map<int, string> index;

    // 回放文件读入
    std::ifstream is(rep_file_name);

    // 读入地图信息
    is >> info.map_size[0] >> info.map_size[1];
    for (size_t i = 0; i < info.map_size[0]; i++)
        for (size_t j = 0; j < info.map_size[1]; j++) {
            is >> info.map[i][j];
        }

    is >> info.team_name[0];
    is >> info.team_name[1];
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
    //初始化地图
    scene.map.init(&info);
    //scene.initialize();

    //处理人物
    //全部隐藏
    actors.clearAll();

    //依次载入
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < info.soldier_number[i]; ++j) {
            try {
                string soldier = actors.getSoldierByKind(info.soldier[j][i].kind);
                index[(i << 4) + j] = soldier;
                actors.list[soldier]->setHP(info.soldier[j][i].life, info.soldier[j][i].life);
                actors.list[soldier]->setPosition(info.soldier[j][i].pos.x, info.soldier[j][i].pos.y) ;
            } catch (char* err) {
                fprintf(stderr, "%s", err);
            }
        }
    }
    // 初始信息读入完成

    //begin_info.temple.resize(info.temple_number);
    //清除原有
    actors.script.clear();

    end_info.over = false;
    while (!end_info.over) {
        is >> begin_info.move_team >> begin_info.move_id;
        is >> begin_info.range_num;
        begin_info.range.resize(begin_info.range_num);
        for (int i = 0; i < begin_info.range_num; i++) {
            is >> begin_info.range[i].x >> begin_info.range[i].y;
        }
        for (int i = 0; i < 2; i++) {
            //is >> begin_info.soldier_number[i];
            for (int j = 0; j < info.soldier_number[i]; j++)
                is >> begin_info.soldier[j][i].kind >> begin_info.soldier[j][i].life
                   >> begin_info.soldier[j][i].strength >> begin_info.soldier[j][i].defence
                   >> begin_info.soldier[j][i].move_range >> begin_info.soldier[j][i].attack_range[0]
                   >> begin_info.soldier[j][i].attack_range[1] >> begin_info.soldier[j][i].duration
                   >> begin_info.soldier[j][i].pos.x >> begin_info.soldier[j][i].pos.y;
        }
        is >> info.temple_number;
        begin_info.temple.resize(info.temple_number);
        for (int i = 0; i < info.temple_number; i++) {
            is >> begin_info.temple[i].pos.x >> begin_info.temple[i].pos.y >> begin_info.temple[i].state;
        }

        ////////////////展示组操作

        is >> cmd.destination.x >> cmd.destination.y >> cmd.order
           >> cmd.target_team >> cmd.target_id;

        ////////////////展示组操作

        for (int i = 0; i < 2; i++) {
            //is >> end_info.soldier_number[i];
            for (int j = 0; j < info.soldier_number[i]; j++)
                is >> end_info.soldier[j][i].kind >> end_info.soldier[j][i].life
                   >> end_info.soldier[j][i].strength >> end_info.soldier[j][i].defence
                   >> end_info.soldier[j][i].move_range >> end_info.soldier[j][i].attack_range[0]
                   >> end_info.soldier[j][i].attack_range[1] >> end_info.soldier[j][i].duration
                   >> end_info.soldier[j][i].pos.x >> end_info.soldier[j][i].pos.y;
        }
        is >> end_info.route_len;
        end_info.route.resize(end_info.route_len);
        //end_info.route.clear();
        for (int i = 0; i < end_info.route_len; i++) {
            is >> end_info.route[i].x >> end_info.route[i].y;
        }
        is >> end_info.score[0] >> end_info.score[1];
        is >> end_info.attack_effect[0] >> end_info.attack_effect[1];
        is >> end_info.trans >> end_info.over;

        ///////////////展示操作
        roundNum++;
        dsSoldier* soldier = actors.list[index[begin_info.move_id + (begin_info.move_team << 4)]];
		
		if(begin_info.soldier[begin_info.move_id ][begin_info.move_team].life <=0){
			continue;
		}
		//是否没动
        if (!(begin_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x
                == end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x
                && begin_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y
                == end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y)){
			//移动
            for (Position p : end_info.route) {
                actors.script.add(roundNum, false, soldier->getID(), soldier_move, p.x, p.y);
			}
		}
        if (cmd.order == 1||cmd.order == 2) {
            dsSoldier* target_soldier = actors.list[index[cmd.target_id + (cmd.target_team << 4)]];
            //攻击动作

            actors.script.add(roundNum, false, soldier->getID(), soldier_fight, target_soldier->x(), target_soldier->y());
            //伤害
            if (begin_info.soldier[cmd.target_id][cmd.target_team].life > 0) {
                int hurt = begin_info.soldier[cmd.target_id][cmd.target_team].life - end_info.soldier[cmd.target_id][cmd.target_team].life;
                actors.script.add(roundNum, true, target_soldier->getID(), soldier_pain, hurt, 0);
                //死亡
                if (end_info.soldier[cmd.target_id][cmd.target_team].life <= 0) {
                    actors.script.add(roundNum, true, target_soldier->getID(), soldier_die, 0, 0);
                }

                //反击
                int hurt2 = begin_info.soldier[begin_info.move_id ][begin_info.move_team].life - end_info.soldier[begin_info.move_id ][begin_info.move_team].life;
                if (hurt2 > 0) {
                    actors.script.add(roundNum, false, soldier->getID(), soldier_pain, hurt, 0);
                    //死亡
                    if (end_info.soldier[begin_info.move_id ][begin_info.move_team].life <= 0) {
                        actors.script.add(roundNum, false, soldier->getID(), soldier_die, 0, 0);
                    }

                }
            }
        } else if (cmd.order == 2) {
            //待添加
        }

    }
    //
    //
    //     std::string soldier_to_move;
    //
    //     while (1) {
    //         is >> begin_info.move_team >> begin_info.move_id;
    //
    //         soldier_to_move = kind[info.soldier[begin_info.move_id][begin_info.move_team].kind];
    //         soldier_to_move += char(begin_info.move_team + 1 + '0');
    //
    //         is >> begin_info.range_num;
    //         begin_info.range.resize(begin_info.range_num);
    //         for (int i = 0; i < begin_info.range_num; i++) {
    //             is >> begin_info.range[i].x >> begin_info.range[i].y;
    //         }
    //         for (int i = 0; i < 2; i++) {
    //             for (int j = 0; j < info.soldier_number[i]; j++)
    //                 is >> begin_info.soldier[j][i].kind
    //                    >> begin_info.soldier[j][i].life
    //                    >> begin_info.soldier[j][i].strength
    //                    >> begin_info.soldier[j][i].defence
    //                    >> begin_info.soldier[j][i].move_range
    //                    >> begin_info.soldier[j][i].attack_range[0]
    //                    >> begin_info.soldier[j][i].attack_range[1]
    //                    >> begin_info.soldier[j][i].duration
    //                    >> begin_info.soldier[j][i].pos.x
    //                    >> begin_info.soldier[j][i].pos.y;
    //         }
    //         is >> info.temple_number;
    //         begin_info.temple.resize(info.temple_number);
    //         for (int i = 0; i < info.temple_number; i++) {
    //             is >> begin_info.temple[i].pos.x
    //                >> begin_info.temple[i].pos.y
    //                >> begin_info.temple[i].state;
    //         }
    //
    //         //scene.actors.script.scriptQue.push_back(Record(1, false, "sword_man1", soldier_fight, 2, 2));
    //         ////////////////展示组操作
    //
    //         is >> cmd.destination.x
    //            >> cmd.destination.y
    //            >> cmd.order
    //            >> cmd.target_team
    //            >> cmd.target_id;
    //
    //
    //
    //         ////////////////展示组操作
    //
    //         for (int i = 0; i < 2; i++) {
    //             for (int j = 0; j < info.soldier_number[i]; j++)
    //                 is >> end_info.soldier[j][i].kind
    //                    >> end_info.soldier[j][i].life
    //                    >> end_info.soldier[j][i].strength
    //                    >> end_info.soldier[j][i].defence
    //                    >> end_info.soldier[j][i].move_range
    //                    >> end_info.soldier[j][i].attack_range[0]
    //                    >> end_info.soldier[j][i].attack_range[1]
    //                    >> end_info.soldier[j][i].duration
    //                    >> end_info.soldier[j][i].pos.x
    //                    >> end_info.soldier[j][i].pos.y;
    //         }
    //         is >> end_info.route_len;
    //         end_info.route.resize(end_info.route_len);
    //         for (int i = 0; i < end_info.route_len; i++) {
    //             is >> end_info.route[i].x >> end_info.route[i].y;
    //         }
    //         is >> end_info.score[0]
    //            >> end_info.score[1];
    //         is >> end_info.attack_effect[0]
    //            >> end_info.attack_effect[1];
    //         is >> end_info.trans
    //            >> end_info.over;
    //         if (end_info.over == true) {
    //             break;
    //         }
    //         ///////////////展示操作
    //     }

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
