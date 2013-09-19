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

    void insertSoldier(const std::string& soldier_name,std::string soldier_type="");

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
    string selectSoldierId;

    std::wstring selectInfo() {
        std::wostringstream info;
        if (selectSoldierId.empty()) {
            return L"NOT SELECT";
        }
        info << selectSoldierId.c_str() << ":\n";
        info << list[selectSoldierId].getInfo();
        return info.str();
    }

	void selectSoldier(int IdNum){
		if(selectSoldierId.length()>0){
			list[selectSoldierId].setSelected(false);
		}
		selectSoldierId = intToString[IdNum];
		list[selectSoldierId].setSelected(true);
	}

	void cancelSelect(){
		if(selectSoldierId.length()>0){
			list[selectSoldierId].setSelected(false);
		}
		selectSoldierId = "";
	}

private:
    SOLDIERS list;
    DSScript script;
    int round;
	//std::map<string,SoundIDandSource> soundGroups; 
};

#endif // !_DSACTORMANAGER_H