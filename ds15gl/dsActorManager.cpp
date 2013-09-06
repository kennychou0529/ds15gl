#include "dsActorManager.h"


DSActorManager::DSActorManager() : round(0),
                                   script_playing(0),
                                   round_finished(false),
                                   all_finished(false) {}


DSActorManager::~DSActorManager() {}


//加载人物
void DSActorManager::initialize() {
    list.insert(std::make_pair("sword_man", dsSoldier()));
    list["sword_man"].load("data/sword_man/tris.md2",
                           "data/sword_man/tris.bmp",
                           "data/sword_man/weapon.md2",
                           "data/sword_man/weapon.bmp");

    list["sword_man"].setPosition(0, 0);
    list["sword_mam"].enterStatus(dsSoldier::Status::idle);

    list.insert(std::make_pair("mage", dsSoldier()));
    list["mage"].load("data/mage/tris.md2",
                      "data/mage/tris.bmp",
                      "data/mage/weapon.md2",
                      "data/mage/weapon.bmp");

    list["mage"].setPosition(1, 1);
    list["mage"].enterStatus(dsSoldier::Status::idle);
}

//渲染
void DSActorManager::render() {
    update();
    //if (list.empty()) {
    //    return;
    //}

    glPushMatrix();
    {
        for (auto& soldier : list) {
            soldier.second.animate();
        }
    }
    glPopMatrix();
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
        if (script.peekNextRecord().round > round) {
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
                        iter_soldier->second.enterStatus(dsSoldier::Status::running, &script_playing);
                    default:
                        break;
                    }
                }
            } while ((script.peekNextRecord().round == round) && (script.peekNextRecord().sync == true));
            // 当下一条指令也是本回合的，且下一条指令要求同步播放，那么将下一条指令也下达

        }
    }

    //if (timer.getDurationMiliseci() > ROUNDTIME) {
    //    round++;

    //    while (script.notEmpty() && script.nextRound() == round) {
    //        Record& record = script.getNextRecord();

    //        auto iter_soldier = list.find(record.id);
    //        if (iter_soldier == list.end()) {
    //            continue;
    //        }
    //        switch (record.type) {
    //        case soldier_move:
    //            iter_soldier->second.setTarget(record.x, record.y);
    //            iter_soldier->second.enterStatus(dsSoldier::Status::running, &script_finished);
    //        default:
    //            break;
    //        }
    //    }
    //    timer.recordTime();
    //}
}