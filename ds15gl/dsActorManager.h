#ifndef _DSACTORMANAGER_H
#define _DSACTORMANAGER_H

#include <map>
#include <string>
#include "dsSoldier.h"
#include "dsScript.h"
#include <iostream>
#include <sstream>
#include "effect/EffectManager.h"

typedef std::map<std::string, dsSoldier*> SOLDIERS;

class DSActorManager {
public:
    DSActorManager();
    ~DSActorManager();

    void insertSoldier(const std::string& soldier_name, std::string soldier_type = "");

    //加载人物
    void initialize();

    //渲染
    void render(bool selectMode);

    //每回合读脚本，更新人物信息，
    void update();

	void showBeginInfo(float time,float totalTime);
	void showEndInfo(float time,float totalTime);

    int getCurrentRound() {
        return round;
    }

    void enterNextRound();

    int script_playing;
    bool round_finished;
    bool all_finished;
    std::map<int, string> intToString;
    string selectSoldierId;

    std::wstring selectInfo() {
        std::wostringstream info;
        if (selectSoldierId.empty()) {
            return L"NOT SELECT";
        }
        info << selectSoldierId.c_str() << ":\n";
        info << list[selectSoldierId]->getInfo();
        return info.str();
    }

    void selectSoldier(int IdNum) {
        if (selectSoldierId.length() > 0) {
            list[selectSoldierId]->setSelected(false);
        }
        selectSoldierId = intToString[IdNum];
        list[selectSoldierId]->setSelected(true);
    }

    void cancelSelect() {
        if (selectSoldierId.length() > 0) {
            list[selectSoldierId]->setSelected(false);
        }
        selectSoldierId = "";
    }
    friend class DSFrame;
    
    void clearAll() {
        list.clear();
        auto s = pool.begin();
        while (s != pool.end()) {
            s->second->enterStatus(dsSoldier::Status::disappear);
            s++;
        }

    }

    string getSoldierByKind(int _kind) {
        auto s = pool.find(kind[_kind]);
        while (s != pool.end()) {
            if (s->second->status == dsSoldier::Status::disappear) {
                dsSoldier* solder = s->second;
                solder->enterStatus(dsSoldier::Status::idle);
                list[solder->id] = solder;
                //pool.erase(s);
                return solder->id;
            }
            s++;
        }
        throw ("NO SUCH A SOLDIER");
    }
private:
    //游戏中的士兵
    SOLDIERS list;
    //士兵池，程序开始，初始化的士兵放在这里，读回放后加入list
    multimap <string, dsSoldier*> pool;
    //脚本
    DSScript script;
    //回合数
    int round;
    //std::map<string,SoundIDandSource> soundGroups;
	EffectManager effects;
	//
};

extern bool gameOver;

#endif // !_DSACTORMANAGER_H