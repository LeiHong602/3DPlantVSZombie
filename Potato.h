#pragma once
#include "Plant.h"
class Potato :public Plant
{
public:
	Potato();
	Potato(const Vector3& pos, const Vector3& rot, const Vector3& sca);
	void PlantWork() override;

	const static int plantCost = 75; //植物的花费
};

