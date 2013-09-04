#ifndef _DSACTORMANAGER_H
#define _DSACTORMANAGER_H

#include <map>
#include <string>
#include "dsSoldier.h"
#include "dsScript.h"
#include "dsTimer.h"

//定义每回合时间
#define ROUNDTIME 2000

typedef std::map<std::string, dsSoldier> SOLDIERS;

class DSActorManager
{
public:
	DSActorManager(void);
	~DSActorManager(void);
	
	//加载人物
	void initialize();
	//渲染
	void render();
	//每回合读脚本，更新人物信息，
	void update();

private:
	SOLDIERS list;
	DSScript script;
	dsTimer timer;
	int round;
};

#endif // !_DSACTORMANAGER_H