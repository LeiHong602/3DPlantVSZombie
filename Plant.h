#pragma once
#include "LoadOBJ.h"

class Plant: public ObjLoader
{
public:
	Plant();
	Plant(const Vector3& pos, const Vector3& rot, const Vector3& sca);

	void UpdatePlant();

	virtual void PlantWork();

	void BeAttacked(int damage); // 受到攻击，减少生命值

	//获取植物生命值
	int GetHP() { return HP; }

protected:
	
	int HP = 100; //植物生命值
};

