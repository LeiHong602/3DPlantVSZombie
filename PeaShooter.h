#pragma once
#include "PeaShooterBullet.h"
#include "PlantsManager.h"
#include "Plant.h"
#include <GLFW/glfw3.h>
class PeaShooter:public Plant
{
public:
	PeaShooter();
	PeaShooter(const Vector3& pos, const Vector3& rot, const Vector3& sca);

	
	void PlantWork() override;

	const static int plantCost = 150; //植物的花费
private:
	float shootInterval = 2.0f; // 射击间隔时间
	double currentShooterTime;  // 当前射击时间
	double lastShootTime=0;       // 上次射击时间
};

