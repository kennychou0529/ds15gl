#include "dsActorManager.h"


DSActorManager::DSActorManager(void)
{
}


DSActorManager::~DSActorManager(void)
{
}


//¼ÓÔØÈËÎï
void DSActorManager::initialize(){

	list.insert(std::make_pair("sword_man", dsSoldier()));
	list["sword_man"].load("data/sword_man/tris.md2",
		"data/sword_man/tris.bmp",
		"data/sword_man/weapon.md2",
		"data/sword_man/weapon.bmp");

	list["sword_man"].setPosition(6, 6);
	list["sword_man"].setTarget(8, 9);
	list["sword_man"].enterStatus(dsSoldier::Status::running);

	list.insert(std::make_pair("mage", dsSoldier()));
	list["mage"].load("data/mage/tris.md2",
		"data/mage/tris.bmp",
		"data/mage/weapon.md2",
		"data/mage/weapon.bmp");

	list["mage"].setPosition(1, 1);
	list["mage"].enterStatus(dsSoldier::Status::idle);
}
//äÖÈ¾
void DSActorManager::render(){
	if (list.empty()) return;
	
	glPushMatrix();
	{
		SOLDIERS::iterator it = list.begin();
		while(it!=list.end()){
			it->second.animate();
			it++;
		}
	}
	glPopMatrix();
}

void DSActorManager::update(){

}