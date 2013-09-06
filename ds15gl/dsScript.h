#ifndef _DSDCRIPT_H
#define _DSDCRIPT_H

#include <string>
#include "dsVector2f.h"
#include <list>

using std::string;

enum ScriptType {
    //next_turn,
    soldier_move,
    soldier_fight,
    soldier_die,
};

struct Record {
    //回合数
    int round;
    // 是否与上一条指令同时发出
    bool sync;
    //对象id;
    string id;
    //动作
    ScriptType type;
    //目标
    size_t x, y;

    Record() {}
    Record(Record& r):
        round(r.round), sync(r.sync), id(r.id), type(r.type), x(r.x), y(r.y) {}

    Record(int r, bool s, string i, ScriptType t, size_t tx, size_t ty):
        round(r), sync(s), id(i), type(t), x(tx), y(ty) {}
};

class DSScript {
public:
    DSScript(): emptyRecord(0, false, "", soldier_die, 0, 0) {
        scriptQue.push_back(Record(0, false, "sword_man", soldier_move, 1, 1));
        scriptQue.push_back(Record(0, false, "mage", soldier_move, 0, 1));
        scriptQue.push_back(Record(1, false, "sword_man", soldier_move, 5, 1));
        scriptQue.push_back(Record(2, false, "mage", soldier_move, 1, 6));
        scriptQue.push_back(Record(2, true, "sword_man", soldier_move, 2, 9));
        scriptQue.push_back(Record(3, false, "sword_man", soldier_move, 1, 1));
        scriptQue.push_back(Record(3, false, "mage", soldier_move, 0, 1));
        scriptQue.push_back(Record(4, false, "sword_man", soldier_move, 5, 1));
        scriptQue.push_back(Record(4, false, "mage", soldier_move, 1, 6));
        scriptQue.push_back(Record(5, false, "sword_man", soldier_move, 2, 9));
    }



    // 窥探接下来的一条记录，但是不将该记录挤出队列
    Record& peekNextRecord() {
        //no need
        ////去掉坏的记录
        //while (!scriptQue.empty() && scriptQue.front_element() == NULL) {
        //    scriptQue.pop();
        //}
		if (scriptQue.empty()) {
			return emptyRecord;
		}
        auto r = scriptQue.front();
        
        /*以前好好的怎么现在不行了？
        Record record(*r);
        delete r;*/
        return r;
    }

    Record getNextRecord() {
		if (scriptQue.empty()) {
			return emptyRecord;
		}
        Record r = scriptQue.front();
        scriptQue.pop_front();
                
        //no need
        ////去掉坏的记录
        //while (!scriptQue.empty() && scriptQue.front() == NULL) {
        //    scriptQue.pop_front();
        //}
        return r;
    }

    bool notEmpty() {
        return !scriptQue.empty();
    }

    int nextRound() {
        if (scriptQue.empty()) {
            return 0;
        }
        return scriptQue.front().round;
    }

private:
    //脚本队列
    std::list<Record> scriptQue;

    //有一个空 Record
    Record emptyRecord;
};

#endif
