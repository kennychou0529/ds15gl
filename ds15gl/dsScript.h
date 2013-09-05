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
    //对象id;
    string id;
    //动作
    ScriptType type;
    //目标
    size_t x, y;

	Record(void){}
	Record(Record &r):
		round(r.round), id(r.id), type(r.type), x(r.x), y(r.y){}

    Record(int r, string i, ScriptType t, size_t tx, size_t ty):
        round(r), id(i), type(t), x(tx), y(ty) {}
};

class DSScript {
public:
    DSScript(void): emptyRecord(0, "", soldier_die, 0, 0) {
        scriptQue.push_back(new Record(0, "sword_man", soldier_move, 1, 1));
		scriptQue.push_back(new Record(0, "mage", soldier_move, 0, 1));
        scriptQue.push_back(new Record(1, "sword_man", soldier_move, 5, 1));
        scriptQue.push_back(new Record(2, "mage", soldier_move, 1, 6));
        scriptQue.push_back(new Record(2, "sword_man", soldier_move, 2, 9));
		scriptQue.push_back(new Record(3, "sword_man", soldier_move, 1, 1));
		scriptQue.push_back(new Record(3, "mage", soldier_move, 0, 1));
		scriptQue.push_back(new Record(4, "sword_man", soldier_move, 5, 1));
		scriptQue.push_back(new Record(4, "mage", soldier_move, 1, 6));
		scriptQue.push_back(new Record(5, "sword_man", soldier_move, 2, 9));
    }

	

    // 窥探接下来的一条记录，但是不将该记录挤出队列
    Record peekNextRecord() {
        //no need
		////去掉坏的记录
        //while (!scriptQue.empty() && scriptQue.front_element() == NULL) {
        //    scriptQue.pop();
        //}
        auto r = scriptQue.front();
        if (r == nullptr) {
            return emptyRecord;
        }
        /*以前好好的怎么现在不行了？
		Record record(*r);
        delete r;*/
        return *r;
    }

    Record getNextRecord() {
        Record* r = scriptQue.front();
		scriptQue.pop_front();
        //这几乎不可能
        if (r == NULL) {
            return emptyRecord;
        }
		//no need
        ////去掉坏的记录
        //while (!scriptQue.empty() && scriptQue.front() == NULL) {
        //    scriptQue.pop_front();
        //}

        Record rd = *r;
        delete r;
        return rd;
    }

    bool notEmpty() {
        return !scriptQue.empty();
    }

    int nextRound() {
        if (scriptQue.empty()) {
            return 0;
        }
        return scriptQue.front()->round;
    }

private:
    //脚本队列
    std::list<Record*> scriptQue;

    //有一个空 Record
    Record emptyRecord;
};

#endif
