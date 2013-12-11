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


Game_Info info;
Round_End_Info end_info;
Round_End_Info end_infos[700];
Round_Begin_Info begin_infos[700];
//从数字索引到stringID的映射
map<int, string> index;

void DSFrame::initialize2(const std::string& rep_file_name) {
    isReady = false;
    //必须等frame停止渲染才能重设场景
    while (!canRefresh) {
        Sleep(100);
    }
    gameOver = false;
    int roundNum = 0;
    Round_Begin_Info begin_info;
    Command cmd;



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
    
    //scene.initialize();

    //处理人物
    //全部隐藏
    actors.clearAll();

    //依次载入
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < info.soldier_number[i]; ++j) {
            try {
                string soldier = actors.getSoldierByKind(info.soldier[j][i].kind);
                index[(i << 5) + j] = soldier;
                actors.list[soldier]->setHP(info.soldier[j][i].life, info.soldier[j][i].life);
                actors.list[soldier]->setPosition(info.soldier[j][i].pos.x, info.soldier[j][i].pos.y) ;
                actors.list[soldier]->setTeam(i + 1);
            } catch (char* err) {
                fprintf(stderr, "%s", err);
            }
        }
    }

    actors.script.clear();
    //actors.script.add(0, false, index[0], soldier_move, info.soldier[0][0].pos.x, info.soldier[0][0].pos.y);
    actors.script.add(0, false, "BEGIN", game_begin, 3, 0);

    // 初始信息读入完成

    //begin_info.temple.resize(info.temple_number);
    //清除原有


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
        std::cout << "TEMPLE" << info.temple_number << std::endl;
        scene.map.data[0] = temple;
        info.map[0][0] = TEMPLE;
        for (int i = 0; i < info.temple_number; i++) {
            is >> begin_info.temple[i].pos.x >> begin_info.temple[i].pos.y >> begin_info.temple[i].state;
            size_t x = begin_info.temple[i].pos.x;
            size_t y = begin_info.temple[i].pos.y;
            size_t x_max = scene.map.x_max;
            //scene.map.data[y * x_max + x] = temple;
            info.map[x][y] = TEMPLE;
            
            //scene.map.listLoaded = false;
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
        if (roundNum == 1) {
            end_infos[0] = end_info;
            begin_infos[0] = begin_info;
        }


        end_infos[roundNum] = end_info;
        begin_infos[roundNum] = begin_info;
        dsSoldier* soldier = actors.list[index[begin_info.move_id + (begin_info.move_team << 5)]];

        if (begin_info.soldier[begin_info.move_id ][begin_info.move_team].life <= 0) {
            continue;
        }
        //是否没动
        if (!(begin_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x
                == end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x
                && begin_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y
                == end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y)) {

            //移动
            for (Position p : end_info.route) {
                actors.script.add(roundNum, false, soldier->getID(), soldier_move, p.x, p.y);

            }
            // 这里
            if (end_info.trans) {
                //Record r = actors.script.pop();
                actors.script.add(roundNum, false, soldier->getID(), soldier_trans,
                                  end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x,
                                  end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y);
            }

        }
        if (cmd.order == 1) {
            dsSoldier* target_soldier = actors.list[index[cmd.target_id + (cmd.target_team << 5)]];


            //禁止鞭尸
            if (end_info.attack_effect[0] != -1 && begin_info.soldier[cmd.target_id][cmd.target_team].life > 0) {
                //攻击动作
                actors.script.add(roundNum, false, soldier->getID(), soldier_fight,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.x,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.y);


                //攻击命中
                if (end_info.attack_effect[0] == 1) {
                    //伤害
                    int hurt = begin_info.soldier[cmd.target_id][cmd.target_team].life - end_info.soldier[cmd.target_id][cmd.target_team].life;

                    actors.script.add(roundNum, true, target_soldier->getID(), soldier_pain, hurt, end_info.soldier[cmd.target_id][cmd.target_team].life);
                    //死亡
                    if (end_info.soldier[cmd.target_id][cmd.target_team].life <= 0) {
                        actors.script.add(roundNum, true, target_soldier->getID(), soldier_die, 0, 0);
                    }
                }

                if (end_info.attack_effect[1] != -1) {
                    actors.script.add(roundNum, false, target_soldier->getID(), soldier_fight,
                                      end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x,
                                      end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y);
                    //反击
                    int hurt2 = begin_info.soldier[begin_info.move_id ][begin_info.move_team].life - end_info.soldier[begin_info.move_id ][begin_info.move_team].life;

                    if (end_info.attack_effect[1] == 1) {

                        actors.script.add(roundNum, false, soldier->getID(), soldier_pain, hurt2, end_info.soldier[begin_info.move_id ][begin_info.move_team].life);
                        //死亡
                        if (end_info.soldier[begin_info.move_id ][begin_info.move_team].life <= 0) {
                            actors.script.add(roundNum, false, soldier->getID(), soldier_die, 0, 0);
                        }

                    }
                }
            }
        } else if (cmd.order == 2) {

            dsSoldier* target_soldier = actors.list[index[cmd.target_id + (cmd.target_team << 5)]];
            switch (begin_info.soldier[begin_info.move_id][begin_info.move_team].kind) {
            case BERSERKER: {

                actors.script.add(roundNum, false, soldier->getID(), soldier_skill,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.x,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.y);
                int recover = begin_info.soldier[cmd.target_id][cmd.target_team].life - end_info.soldier[cmd.target_id][cmd.target_team].life;
                if (recover < 0) {
                    actors.script.add(roundNum, false, target_soldier->getID(), soldier_pain,
                                      recover,
                                      end_info.soldier[cmd.target_id][cmd.target_team].life);
                }
            }
            break;
            case ASSASSIN: {
                actors.script.add(roundNum, false, soldier->getID(), soldier_skill,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.x,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.y);

                //伤害
                int hurt = begin_info.soldier[cmd.target_id][cmd.target_team].life - end_info.soldier[cmd.target_id][cmd.target_team].life;

                actors.script.add(roundNum, true, target_soldier->getID(), soldier_pain, hurt, end_info.soldier[cmd.target_id][cmd.target_team].life);
                //死亡
                if (end_info.soldier[cmd.target_id][cmd.target_team].life <= 0) {
                    actors.script.add(roundNum, true, target_soldier->getID(), soldier_die, 0, 0);
                }



                actors.script.add(roundNum, false, target_soldier->getID(), soldier_fight,
                                  end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.x,
                                  end_info.soldier[begin_info.move_id ][begin_info.move_team].pos.y);
                //反击
                int hurt2 = begin_info.soldier[begin_info.move_id ][begin_info.move_team].life - end_info.soldier[begin_info.move_id ][begin_info.move_team].life;



                actors.script.add(roundNum, false, soldier->getID(), soldier_pain, hurt2, end_info.soldier[begin_info.move_id ][begin_info.move_team].life);
                //死亡
                if (end_info.soldier[begin_info.move_id ][begin_info.move_team].life <= 0) {
                    actors.script.add(roundNum, false, soldier->getID(), soldier_die, 0, 0);
                }
            }

            break;
            case ARCHMAGE:
                actors.script.add(roundNum, false, soldier->getID(), soldier_skill,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.x,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.y);
                break;
            case WIZARD: {
                actors.script.add(roundNum, false, soldier->getID(), soldier_fight,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.x,
                                  end_info.soldier[cmd.target_id][cmd.target_team].pos.y);
                int hurt = begin_info.soldier[cmd.target_id][cmd.target_team].life - end_info.soldier[cmd.target_id][cmd.target_team].life;

                actors.script.add(roundNum, true, target_soldier->getID(), soldier_pain, hurt, end_info.soldier[cmd.target_id][cmd.target_team].life);


            }

            default:
                break;
            }
        }
        if (is.eof() || roundNum > 600) {
            break;
        }
    }

    //初始化地图
    scene.map.init(&info);
    //scene.map.loadDisplayLists();
    actors.script.add(roundNum, false, "END", game_over, 10, 0);

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
