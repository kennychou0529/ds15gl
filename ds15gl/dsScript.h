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
    //�غ���
    int round;
    // �Ƿ�����һ��ָ��ͬʱ����
    bool sync;
    //����id;
    string id;
    //����
    ScriptType type;
    //Ŀ��
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



    // ��̽��������һ����¼�����ǲ����ü�¼��������
    Record& peekNextRecord() {
        //no need
        ////ȥ�����ļ�¼
        //while (!scriptQue.empty() && scriptQue.front_element() == NULL) {
        //    scriptQue.pop();
        //}
		if (scriptQue.empty()) {
			return emptyRecord;
		}
        auto r = scriptQue.front();
        
        /*��ǰ�úõ���ô���ڲ����ˣ�
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
        ////ȥ�����ļ�¼
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
    //�ű�����
    std::list<Record> scriptQue;

    //��һ���� Record
    Record emptyRecord;
};

#endif
