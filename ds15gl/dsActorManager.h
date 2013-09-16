#ifndef _DSACTORMANAGER_H
#define _DSACTORMANAGER_H

#include <map>
#include <string>
#include "dsSoldier.h"
#include "dsScript.h"
#include <iostream>
#include <sstream>


typedef std::map<std::string, dsSoldier> SOLDIERS;

class DSActorManager {
public:
    DSActorManager();
    ~DSActorManager();

    void insertSoldier(const std::string& soldier_name);

    //加载人物
    void initialize();

    //渲染
    void render(bool selectMode);

    //每回合读脚本，更新人物信息，
    void update();

    int getCurrentRound() {
        return round;
    }

    void enterNextRound() {
        ++round;
        round_finished = false;
    }

    int script_playing;
    bool round_finished;
    bool all_finished;
    std::map<int, string> intToString;
    string selectSoldier;

    std::wstring selectInfo() {
        std::wostringstream info;
        if (selectSoldier.empty()) {
            return L"NOT SELECT";
        }
        info << selectSoldier.c_str() << ":\n";
        info << list[selectSoldier].getInfo();
        return info.str();
    }
private:
    SOLDIERS list;
    DSScript script;
    int round;
	//std::map<string,SoundIDandSource> soundGroups; 
};

#endif // !_DSACTORMANAGER_H