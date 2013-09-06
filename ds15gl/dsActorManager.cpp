#include "dsActorManager.h"


DSActorManager::DSActorManager() : round(0),
                                   script_playing(0),
                                   round_finished(false),
                                   all_finished(false) {}


DSActorManager::~DSActorManager() {}


//��������
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

//��Ⱦ
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

    // ֻ�е���ǰָ���Ѿ���ɲ���ʱ������Ҫ update
    // ����Ӧ�ü�������
    if (script_playing == 0) {

        // ���û���µ�ָ���ˣ�˵�����еĶ����������
        if (!script.notEmpty()) {
            all_finished = true;
            return;
        }

        // �����̽����������һ��ָ���Ӧ�Ļغ������ڵ�ǰ�غ�
        // ˵����ǰ�غϵ�����ָ��Ѳ������
        // ���Ǳ�� round_finished
        if (script.peekNextRecord().round > round) {
            round_finished = true;
        }

        // ֻ�е���ǰ�غϻ�û����ȫ������ɵ�ʱ�����Ҫ�Զ�������һ��ָ��
        if (!round_finished) {
            do {
                // ָ�����
                Record record = script.getNextRecord();

                // �´�ָ��
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
            // ����һ��ָ��Ҳ�Ǳ��غϵģ�����һ��ָ��Ҫ��ͬ�����ţ���ô����һ��ָ��Ҳ�´�

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