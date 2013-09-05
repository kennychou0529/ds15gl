#ifndef _DSACTORMANAGER_H
#define _DSACTORMANAGER_H

#include <map>
#include <string>
#include "dsSoldier.h"
#include "dsScript.h"
#include "dsTimer.h"

//����ÿ�غ�ʱ��
#define ROUNDTIME 2000

typedef std::map<std::string, dsSoldier> SOLDIERS;

class DSActorManager {
public:
    DSActorManager();
    ~DSActorManager();

    //��������
    void initialize();

    //��Ⱦ
    void render();

    //ÿ�غ϶��ű�������������Ϣ��
    void update();

    bool script_finished;

private:
    SOLDIERS list;
    DSScript script;
    //dsTimer timer;
    int round;

    bool round_finished;
};

#endif // !_DSACTORMANAGER_H