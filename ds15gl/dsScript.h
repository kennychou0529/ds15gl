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
    //����id;
    string id;
    //����
    ScriptType type;
    //Ŀ��
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

	

    // ��̽��������һ����¼�����ǲ����ü�¼��������
    Record peekNextRecord() {
        //no need
		////ȥ�����ļ�¼
        //while (!scriptQue.empty() && scriptQue.front_element() == NULL) {
        //    scriptQue.pop();
        //}
        auto r = scriptQue.front();
        if (r == nullptr) {
            return emptyRecord;
        }
        /*��ǰ�úõ���ô���ڲ����ˣ�
		Record record(*r);
        delete r;*/
        return *r;
    }

    Record getNextRecord() {
        Record* r = scriptQue.front();
		scriptQue.pop_front();
        //�⼸��������
        if (r == NULL) {
            return emptyRecord;
        }
		//no need
        ////ȥ�����ļ�¼
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
    //�ű�����
    std::list<Record*> scriptQue;

    //��һ���� Record
    Record emptyRecord;
};

#endif
