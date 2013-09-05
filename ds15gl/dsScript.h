#ifndef _DSDCRIPT_H
#define _DSDCRIPT_H

#include <string>
#include "dsVector2f.h"
#include "dsQueue.h"

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

    Record(void);
    Record(int r, string i, ScriptType t, size_t tx, size_t ty):
        round(r), id(i), type(t), x(tx), y(ty) {}
};

class DSScript {
public:
    DSScript(void): emptyRecord(0, "", soldier_die, 0, 0) {

        scriptQue.push(new Record(3, "sword_man", soldier_move, 1, 1));
        scriptQue.push(new Record(6, "sword_man", soldier_move, 5, 1));
        scriptQue.push(new Record(9, "sword_man", soldier_move, 1, 6));
        scriptQue.push(new Record(12, "sword_man", soldier_move, 2, 1));
    }

    Record getNextRecord() {
        Record* r = scriptQue.pop();
        //�⼸��������
        if (r == NULL) {
            return emptyRecord;
        }
        //ȥ�����ļ�¼
        while (!scriptQue.empty() && scriptQue.front_element() == NULL) {
            scriptQue.pop();
        }

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
        return scriptQue.front_element()->round;
    }

private:
    //�ű�����
    DSQueue<Record*> scriptQue;

    //��һ���� Record
    Record emptyRecord;
};

#endif
