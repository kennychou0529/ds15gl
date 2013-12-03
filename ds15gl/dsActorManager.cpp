#include "dsActorManager.h"
#include "dsSoundManager.h"
#include "dsEye.h"
#include "dsFrame.h"

bool gameOver;

DSActorManager::DSActorManager() :
    round(0),
    script_playing(0),
    round_finished(false),
    all_finished(false) {
    gameOver = true;
}


DSActorManager::~DSActorManager() {
    //list.clear();
    //两个map保存了同一个对象，必然有一个析构出问题
    //list = *(new SOLDIERS);
    clearAll();
    for (auto i : pool) {
        delete i.second;
    }

    pool.clear();
}

extern DSFrame frame;

void DSActorManager::insertSoldier(const std::string& soldier_id, std::string soldier_type) {
    static int counter = 1;
    if (soldier_type.empty()) {
        soldier_type = soldier_id;
    }
    size_t team;
    if (soldier_id[soldier_id.length() - 1] == '1') {
        team = 1;
    } else {
        team = 2;
    }
    //size_t team = soldier_id[soldier_id.length() - 1] - '0';
    dsSoldier* sol = new dsSoldier(counter);

    sol->load(soldier_type, team);
    sol->setID(soldier_id);
    sol->enterStatus(dsSoldier::Status::idle);
    intToString[counter++] = soldier_id;

    if (frame.sounds.soundgroups.find(soldier_type) != frame.sounds.soundgroups.end()) {
        sol->setSounds(frame.sounds.soundgroups[soldier_type]);
    } else {
        sol->setSounds(DSSoundManager::defaultSoundGroup);
    }

    pool.insert(std::make_pair(soldier_type, sol));
}

//加载人物
void DSActorManager::initialize() {

    insertSoldier("SABER1", "SABER");
    insertSoldier("SABER2", "SABER");
	insertSoldier("SABER3", "SABER");
	insertSoldier("SABER4", "SABER");
	insertSoldier("SABER5", "SABER");
	insertSoldier("SABER6", "SABER");
	insertSoldier("SABER7", "SABER");
	insertSoldier("SABER8", "SABER");
	insertSoldier("SABER9", "SABER");

    insertSoldier("SOLDIER1", "SOLDIER");
    insertSoldier("SOLDIER2", "SOLDIER");
	insertSoldier("SOLDIER3", "SOLDIER");
	insertSoldier("SOLDIER4", "SOLDIER");
	insertSoldier("SOLDIER5", "SOLDIER");
	insertSoldier("SOLDIER6", "SOLDIER");
	insertSoldier("SOLDIER7", "SOLDIER");
	insertSoldier("SOLDIER8", "SOLDIER");
	insertSoldier("SOLDIER9", "SOLDIER");

    //     list["SOLDIER"].setPosition(1, 1);
    //     list["SOLDIER"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("ARCHER1", "ARCHER");
    insertSoldier("ARCHER2", "ARCHER");
	insertSoldier("ARCHER3", "ARCHER");
	insertSoldier("ARCHER4", "ARCHER");
	insertSoldier("ARCHER5", "ARCHER");
	insertSoldier("ARCHER6", "ARCHER");
	insertSoldier("ARCHER7", "ARCHER");
	insertSoldier("ARCHER8", "ARCHER");
	insertSoldier("ARCHER9", "ARCHER");
	
    //     list["ARCHER"].setPosition(1, 5);
    //     list["ARCHER"].enterStatus(dsSoldier::Status::disappear);

   
	insertSoldier("AIRPLANE1", "AIRPLANE");
	insertSoldier("AIRPLANE2", "AIRPLANE");
	insertSoldier("AIRPLANE3", "AIRPLANE");
	insertSoldier("AIRPLANE4", "AIRPLANE");
	insertSoldier("AIRPLANE5", "AIRPLANE");
	insertSoldier("AIRPLANE6", "AIRPLANE");
	insertSoldier("AIRPLANE7", "AIRPLANE");
	insertSoldier("AIRPLANE8", "AIRPLANE");
	insertSoldier("AIRPLANE9", "AIRPLANE");
	
    /*  list["AIRPLANE"].setPosition(2, 4);*/
    //list["AIRPLANE"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("TANK1", "TANK");
    insertSoldier("TANK2", "TANK");
	insertSoldier("TANK3", "TANK");
	insertSoldier("TANK4", "TANK");
	insertSoldier("TANK5", "TANK");
	insertSoldier("TANK6", "TANK");
	insertSoldier("TANK7", "TANK");
	insertSoldier("TANK8", "TANK");
	insertSoldier("TANK9", "TANK");
	
    //list["TANK"].setPosition(0, 2);
    //list["TANK"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("WIZARD1", "WIZARD");
    insertSoldier("WIZARD2", "WIZARD");
	insertSoldier("WIZARD3", "WIZARD");
	insertSoldier("WIZARD4", "WIZARD");
	insertSoldier("WIZARD5", "WIZARD");
	insertSoldier("WIZARD6", "WIZARD");
	insertSoldier("WIZARD7", "WIZARD");
	insertSoldier("WIZARD8", "WIZARD");
	insertSoldier("WIZARD9", "WIZARD");
	
    //     list["WIZARD"].setPosition(5, 5);
    //     list["WIZARD"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("BERSERKER1", "BERSERKER");
    insertSoldier("BERSERKER2", "BERSERKER");

	insertSoldier("ASSASSIN1", "ASSASSIN");
	insertSoldier("ASSASSIN2", "ASSASSIN");

	insertSoldier("ARCHMAGE1", "ARCHMAGE");
	insertSoldier("ARCHMAGE2", "ARCHMAGE");
    

}

//渲染
dsTimer timer;
float delay_time = 0;
void DSActorManager::render(bool selectMode) {
    update();

    for (auto & soldier : list) {
        glEnable(GL_LIGHTING);
        soldier.second->animate(selectMode);
        //soldier.second->hpBar2();
    }

    effects.render();

    if (delay_time > 0) {
        if (delay_time > timer.getDurationSecf()) {
            float duration =  timer.getDurationSecf();
            if (gameOver) {
                showEndInfo(duration, delay_time);
            } else {
                showBeginInfo(duration, delay_time);
            }
        } else {
            delay_time = 0;
            script_playing --;
        }
    }


    //list["SOLDIER"].hpBar2();
}

//#include "dsTimer.h"

void DSActorManager::update() {

    // 只有当当前指令已经完成播放时，才需要 update
    // 否则应该继续播放
    if (script_playing == 0) {

        // 如果没有新的指令了，说明所有的都播放完成了
        if (!script.notEmpty()) {
            all_finished = true;
            return;
        }

        // 如果窥探到接下来的一条指令对应的回合数大于当前回合
        // 说明当前回合的所有指令都已播放完成
        // 于是标记 round_finished
        if (script.nextRound() > round) {
            round_finished = true;
        }

        // 只有当当前回合还没有完全播放完成的时候才需要自动进入下一波指令
        if (!round_finished) {
            do {
                // 指令出队
                Record record = script.getNextRecord();
                if (record.type == game_over) {
                    gameOver = true;
                    delay_time = record.x;
                    timer.recordTime();
                    script_playing++;
                    return;
                } else if (record.type == game_begin) {
                    delay_time = record.x;
                    timer.recordTime();
                    script_playing++;
					frame.sounds.replay();
					glutShowWindow();
                    return;
                }
                // 下达指令
                auto iter_soldier = list.find(record.id);
                if (iter_soldier != list.end()) {

                    switch (record.type) {
                    case soldier_move:
                        iter_soldier->second->setTarget(record.x, record.y);
                        iter_soldier->second->enterStatus(
                            dsSoldier::Status::running, &script_playing
                        );
                        break;
                    case soldier_fight:
                        iter_soldier->second->enterStatus(
                            dsSoldier::Status::attacking, &script_playing
                        );
                        iter_soldier->second->setOrientation(record.x, record.y);

                        float x1, y1, x2, y2;
                        frame.scene.map.getCoords(
                            iter_soldier->second->current_position[0],
                            iter_soldier->second->current_position[1],
                            &x1, &y1
                        );
                        frame.scene.map.getCoords(
                            record.x,
                            record.y,
                            &x2, &y2
                        );

                        if (iter_soldier->second->kind == "AIRPLANE") {
                            Thurder* t = new Thurder(x1, y1, 6, x2, y2, 2, 10, 3, 1, 1);
                            effects.addThurder(*t);

                            Color* colors = new Color[256];
                            for (int i = 0; i < 200; i++) {
                                colors[i].r = 1;
                                colors[i].g = 1;
                                colors[i].b = 0;
                                colors[i].a = 1 -  1.3f * (i / 200.0f) * (i / 200.0f);
                            }

                            Emitter emm(0, 6.28f, 0, 0.5, 2, 2, colors, 0.2);
                            emm.setPosition(x2, y2, 0);
                            //emm.setSpeed(0,0,-4);
                            //Vector mag = {0, 0.01, 0};
                            emm.setGravity(0, 0, 1);
                            emm.setCenter(0, 0, 10, 1);

                            //emm.setMagnetic(0, 0.5, 0);
                            effects.addEmitter(emm);
                        }
                        break;
                    case soldier_die:
                        iter_soldier->second->enterStatus(
                            dsSoldier::Status::dying, &script_playing
                        );
                        break;
                    case soldier_pain:
                        iter_soldier->second->enterStatus(
                            dsSoldier::Status::pain, &script_playing
                        );
                        iter_soldier->second->hpReduce(record.y);

                        //转化为实际坐标
                        float x, y;
                        frame.scene.map.getCoords(
                            iter_soldier->second->current_position[0],
                            iter_soldier->second->current_position[1],
                            &x, &y
                        );
                        //文字画不出来
                        //                         if (record.x > 0) {
                        //                             std::stringstream os;
                        //                             os << "-" << record.x;
                        //                             Text *t=new Text(os.str(), 2);
                        //                             t->setPosition(x, y, 10);
                        //                             t->setColor(1, 0, 0);
                        //                             effects.addText(*t);
                        //                         } else if (record.x < 0) {
                        //                             std::stringstream os;
                        //                             os << "+" << -record.x;
                        //                             Text *t=new Text(os.str(), 2);
                        //                             t->setPosition(x, y, 10);
                        //                             t->setColor(0, 1, 0);
                        //                             effects.addText(*t);
                        //                         } else {
                        //                             std::stringstream os;
                        //                             os << "MISS";
                        //                             Text* t=new Text(os.str(), 2);
                        //                             t->setPosition(x, y, 10);
                        //                             t->setColor(0, 0, 1);
                        //                             effects.addText(*t);
                        //                         }
                        break;
                    case soldier_trans: {
                        float x3, y3;
                        frame.scene.map.getCoords(
                            iter_soldier->second->current_position[0],
                            iter_soldier->second->current_position[1],
                            &x3, &y3
                        );
                        iter_soldier->second->setPosition(record.x, record.y);

                        Color* colors = new Color[256];
                        for (int i = 0; i < 200; i++) {
                            colors[i].r = 1;
                            colors[i].g = 1;
                            colors[i].b = 0;
                            colors[i].a = 1 -  1.3f * (i / 200.0f) * (i / 200.0f);
                        }

                        Emitter emm(0, 6.28f, 0, 0.5, 2, 2, colors, 0.3);
                        emm.setPosition(x3, y3, 0);
                        //emm.setSpeed(0,0,-4);
                        //Vector mag = {0, 0.01, 0};
                        emm.setGravity(0, 0, 1);
                        //emm.setCenter(0, 0, 10, 1);

                        //emm.setMagnetic(0, 0.5, 0);
                        effects.addEmitter(emm);
                    }
                    break;
                    default:
                        break;
                    }
                }

                if (record.type == view_move) {
                    centerMoveToi(record.x, record.y, &script_playing);
                }

            } while (
                (script.nextRound() == round)
                && (script.peekNextRecord().sync)
            );
            // 当下一条指令也是本回合的，且下一条指令要求同步播放，
            // 那么将下一条指令也下达

        } else {
            enterNextRound();
        }
    }
}

void DSActorManager::enterNextRound() {
    ++round;
    round_finished = false;
    std::wostringstream os;
    os.str(L"");
    os << L"第 " << round << L" 回合";
    frame.scene.mboxes.addMessage(os.str().c_str(), 250.0f);
}