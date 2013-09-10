#ifndef _DSDCRIPT_H
#define _DSDCRIPT_H

#include <string>
#include <list>
#include "dsVector2f.h"

using std::string;

enum ScriptType {
    //next_turn,
    soldier_move,
    soldier_fight,
    soldier_die,
    soldier_pain,
    empty
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
    DSScript(): emptyRecord(0, false, "", empty, 0, 0) {
        scriptQue.push_back(Record(1, false, "chastit", soldier_die, 1, 2));
        scriptQue.push_back(Record(1, false, "sword_man", soldier_die, 1, 1));
        scriptQue.push_back(Record(1, false, "mage", soldier_die, 0, 1));
        scriptQue.push_back(Record(1, false, "gunman", soldier_die, 0, 1));
        scriptQue.push_back(Record(1, false, "bauul", soldier_die, 5, 1));
        scriptQue.push_back(Record(1, false, "fighter", soldier_die, 5, 1));


        scriptQue.push_back(Record(2, false, "mage", soldier_pain, 1, 6));
        scriptQue.push_back(Record(2, false, "sword_man", soldier_pain, 2, 9));
        scriptQue.push_back(Record(2, false, "sword_man", soldier_die, 2, 9));
        
        scriptQue.push_back(Record(3, false, "sword_man", soldier_move, 1, 1));
        scriptQue.push_back(Record(3, false, "mage", soldier_die, 0, 1));
        scriptQue.push_back(Record(4, false, "sword_man", soldier_move, 5, 1));
        scriptQue.push_back(Record(4, false, "mage", soldier_move, 1, 6));
        scriptQue.push_back(Record(4, false, "fighter", soldier_die, 2, 3));
        scriptQue.push_back(Record(5, false, "sword_man", soldier_move, 2, 9));
    }



    // 窥探接下来的一条记录，但是不将该记录挤出队列
    Record peekNextRecord() {
		if (scriptQue.empty()) {
			return emptyRecord;
		}
        return scriptQue.front();
    }

    // 获取接下来的一条记录，同时将该记录挤出队列
    Record getNextRecord() {
		if (scriptQue.empty()) {
			return emptyRecord;
		}
        Record r = scriptQue.front();
        scriptQue.pop_front();
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
