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

class DSActorManager
{
public:
	DSActorManager(void);
	~DSActorManager(void);
	
	//��������
	void initialize();
	//��Ⱦ
	void render();
	//ÿ�غ϶��ű�������������Ϣ��
	void update();

private:
	SOLDIERS list;
	DSScript script;
	dsTimer timer;
	int round;
};

#endif // !_DSACTORMANAGER_H