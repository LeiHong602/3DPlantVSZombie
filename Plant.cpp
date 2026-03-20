#include "Plant.h"

Plant::Plant()
{
}

Plant::Plant(const Vector3& pos, const Vector3& rot, const Vector3& sca):ObjLoader(pos,rot,sca)
{
}

void Plant::UpdatePlant()
{
	PlantWork();//执行植物工作
}

void Plant::PlantWork()
{
}

void Plant::BeAttacked(int damage)
{
	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;
	}
}
