#include "dsActorManager.h"
#include "dsSoundManager.h"
#include "dsEye.h"
#include "dsFrame.h"

DSActorManager::DSActorManager() :
    round(0),
    script_playing(0),
    round_finished(false),
    all_finished(false) {}


DSActorManager::~DSActorManager() {}

extern DSFrame frame;

void DSActorManager::insertSoldier(const std::string& soldier_id, std::string soldier_type) {
    static int counter = 1;
    if (soldier_type.empty()) {
        soldier_type = soldier_id;
    }
    list.insert(std::make_pair(soldier_id, dsSoldier(counter)));
    list[soldier_id].load(soldier_type);
    list[soldier_id].enterStatus(dsSoldier::Status::idle);
    intToString[counter++] = soldier_id;

    if (frame.sounds.soundgroups.find(soldier_type) != frame.sounds.soundgroups.end()) {
        list[soldier_id].setSounds(frame.sounds.soundgroups[soldier_type]);
    } else {
        list[soldier_id].setSounds(DSSoundManager::defaultSoundGroup);
    }
}

//加载人物
void DSActorManager::initialize() {

    insertSoldier("sword_man");
    list["sword_man"].setPosition(0, 0);

    insertSoldier("mage");
    list["mage"].setPosition(1, 1);

    insertSoldier("chastit");
    list["chastit"].setPosition(1, 5);

    insertSoldier("gunman");
    list["gunman"].setPosition(1, 6);

    insertSoldier("bauul");
    list["bauul"].setPosition(3, 6);

    insertSoldier("fighter");
    list["fighter"].setPosition(5, 6);

    insertSoldier("tank");
    list["tank"].setPosition(6, 7);

    insertSoldier("disgustin");
    list["disgustin"].setPosition(5, 7);

    insertSoldier("monkey");
    list["monkey"].setPosition(5, 8);
}

//渲染
void DSActorManager::render(bool selectMode) {
    update();

    for (auto & soldier : list) {
        glEnable(GL_LIGHTING);
        soldier.second.animate(selectMode);
        //soldier.second.hpBar2();
    }
    //list["mage"].hpBar2();
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
                        iter_soldier->second.setTarget(record.x, record.y);
                        iter_soldier->second.enterStatus(
                            dsSoldier::Status::running, &script_playing
                        );
                        break;
                    case soldier_fight:
                        iter_soldier->second.enterStatus(
                            dsSoldier::Status::attacking, &script_playing
                        );
                        break;
                    case soldier_die:
                        iter_soldier->second.enterStatus(
                            dsSoldier::Status::dying, &script_playing
                        );
                        break;
                    case soldier_pain:
                        iter_soldier->second.enterStatus(
                            dsSoldier::Status::pain, &script_playing
                        );
                        iter_soldier->second.hpReduce(record.x);
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

        }
    }
}