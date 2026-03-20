#include "SunManager.h"

int SunManager::sunCount = 2000; // 初始阳光数量
vector<SunParticle*> SunManager::allSunParticles = vector<SunParticle*>();

bool SunManager::ReduceSun(int amount)
{
	if (sunCount >= amount) {
		sunCount -= amount;
		return true;
	}
	return false;
}

void SunManager::Update()
{
	for (auto it = allSunParticles.begin(); it != allSunParticles.end();)
	{
		SunParticle* sun = *it;
		sun->Update();
		//如果粒子生命结束，则删除粒子
		if (sun->isActive == false)
		{
			it = allSunParticles.erase(it);
			delete sun;
		}
		else
		{
			++it;
		}
	}
}

void SunManager::Draw()
{
	for (auto it = allSunParticles.begin(); it != allSunParticles.end(); it++)
	{
		SunParticle* sun = *it;
		sun->Render();
	}
}
