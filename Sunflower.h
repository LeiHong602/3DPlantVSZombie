#pragma once
#include <GLFW/glfw3.h>
#include "Plant.h"
#include "SunParticle.h"
#include "SunManager.h"

class Sunflower : public Plant
{
public:
	Sunflower();
	Sunflower(const Vector3& pos, const Vector3& rot, const Vector3& sca);
	void PlantWork() override;

	const static int plantCost = 50; //植物的花费
private:
	float produceInterval = 7.0f; // 生产阳光间隔时间
	int produceSunValue = 25;    // 每次生产阳光数量
	double currentProduceTime;  // 当前生产时间
	double lastProduceTime = 0;       // 上次生产时间
};

