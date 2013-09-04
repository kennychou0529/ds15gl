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
	
	//��������
	void initialize();
	//��Ⱦ
	void render();
	//���ű�������������Ϣ��ÿ�غ�����
	void update();

private:
	SOLDIERS list;
};

#endif // !_DSACTORMANAGER_H