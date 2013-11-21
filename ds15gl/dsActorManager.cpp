#include "dsActorManager.h"
#include "dsSoundManager.h"
#include "dsEye.h"
#include "dsFrame.h"

DSActorManager::DSActorManager() :
    round(0),
    script_playing(0),
    round_finished(false),
    all_finished(false) {}


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
    dsSoldier* sol = new dsSoldier(counter);

    sol->load(soldier_type);
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
    //list["SABER"].setPosition(4, 2);
    insertSoldier("SABER2", "SABER");
    //list["SABER"].setPosition(4, 2);

    insertSoldier("SOLDIER1", "SOLDIER");
    insertSoldier("SOLDIER2", "SOLDIER");

    //     list["SOLDIER"].setPosition(1, 1);
    //     list["SOLDIER"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("ARCHER1", "ARCHER");
    insertSoldier("ARCHER2", "ARCHER");
    //     list["ARCHER"].setPosition(1, 5);
    //     list["ARCHER"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("AIRPLANE1", "AIRPLANE");
    insertSoldier("AIRPLANE2", "AIRPLANE");
    /*  list["AIRPLANE"].setPosition(2, 4);*/
    //list["AIRPLANE"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("TANK1", "TANK");
    insertSoldier("TANK2", "TANK");
    //list["TANK"].setPosition(0, 2);
    //list["TANK"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("WIZARD1", "WIZARD");
    insertSoldier("WIZARD2", "WIZARD");
    //     list["WIZARD"].setPosition(5, 5);
    //     list["WIZARD"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("BERSERKER1", "BERSERKER");
    insertSoldier("BERSERKER2", "BERSERKER");
    //    list["BERSERKER"].setPosition(2, 2);
    //list["BERSERKER"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("BERSERKER1", "BERSERKER");
    insertSoldier("BERSERKER2", "BERSERKER");
    //     list["BERSERKER"].setPosition(0, 2);
    //     list["BERSERKER"].enterStatus(dsSoldier::Status::disappear);

    insertSoldier("BERSERKER1", "BERSERKER");
    insertSoldier("BERSERKER2", "BERSERKER");
    //list["BERSERKER"].setPosition(2, 0);
    //list["BERSERKER"].enterStatus(dsSoldier::Status::disappear);
    //mvAlltoPool();
}

//渲染
void DSActorManager::render(bool selectMode) {
    update();

    for (auto & soldier : list) {
        glEnable(GL_LIGHTING);
        soldier.second->animate(selectMode);
        //soldier.second->hpBar2();
    }
    effects.render();
    //list["SOLDIER"].hpBar2();
}

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
                        if (record.x > 0) {
                            std::wostringstream os;
                            os << "-" << record.x;
                            Text t(os.str(), 2);
                            t.setPosition(x, y, 10);
                            t.setColor(1, 0, 0);
                            effects.addText(t);
                        } else if (record.x < 0) {
                            std::wostringstream os;
                            os << "+" << -record.x;
                            Text t(os.str(), 2);
                            t.setPosition(x, y, 10);
                            t.setColor(0, 1, 0);
                            effects.addText(t);
                        } else {
                            std::wostringstream os;
                            os << "MISS";
                            Text t(os.str(), 2);
                            t.setPosition(x, y, 10);
                            t.setColor(0, 0, 1);
                            effects.addText(t);
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
    frame.scene.mboxes.addMessage(os.str().c_str(), 230.0f);
}