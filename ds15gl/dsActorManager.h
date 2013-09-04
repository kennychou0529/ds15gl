#ifndef _DSACTORMANAGER_H
#define _DSACTORMANAGER_H

#include <map>
#include <string>
#include "dsSoldier.h"

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
	//读脚本，更新人物信息，每回合运行
	void update();

private:
	SOLDIERS list;
};

#endif // !_DSACTORMANAGER_H