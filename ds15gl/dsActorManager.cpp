#include "dsActorManager.h"


DSActorManager::DSActorManager() : round(0),
    script_finished(true),
    round_finished(true) {}


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
    //list["sword_man"].setTarget(8, 9);
    //list["sword_man"].enterStatus(dsSoldier::Status::running);

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
        for (auto & soldier : list) {
            soldier.second.animate();
        }
    }
    glPopMatrix();
}

void DSActorManager::update() {
    if ((script_finished == true) && script.notEmpty()) {
        script_finished = false;
        Record record = script.getNextRecord();
		//记录一下当前回合数，用于显示
		round = record.round;

        auto iter_soldier = list.find(record.id);
        if (iter_soldier != list.end()) {
            switch (record.type) {
            case soldier_move:
                iter_soldier->second.setTarget(record.x, record.y);
                iter_soldier->second.enterStatus(dsSoldier::Status::running, &script_finished);
            default:
                break;
            }
        }

    }

    /*if (timer.getDurationMiliseci() > ROUNDTIME) {
        round++;

        while (script.notEmpty() && script.nextRound() == round) {
            Record& record = script.getNextRecord();

            auto iter_soldier = list.find(record.id);
            if (iter_soldier == list.end()) {
                continue;
            }
            switch (record.type) {
            case soldier_move:
                iter_soldier->second.setTarget(record.x, record.y);
                iter_soldier->second.enterStatus(dsSoldier::Status::running, &script_finished);
            default:
                break;
            }
        }
        timer.recordTime();
    }*/

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