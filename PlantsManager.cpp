#include "PlantsManager.h"

GoldRing PlantsManager::goldRing = GoldRing();
Plant* PlantsManager::allPlant[7][10] = { 0 };
vector<PeaShooterBullet*> PlantsManager::allBullets = vector<PeaShooterBullet*>();
vector<PeaShooterBulletBoom*> PlantsManager::allBooms = vector<PeaShooterBulletBoom*>();

bool PlantsManager::Init()
{
	/** 初始化金环实例 */
	if (!goldRing.Init(1500))
	{
		MessageBox(NULL, "金环系统初始化失败!", "错误", MB_OK);
		return FALSE;
	}

	return true;
}

bool PlantsManager::CheckPeaShooterBullets(PeaShooterBullet* peaShooterBullet)
{
	int z = peaShooterBullet->position.z / 2;

	for (auto it = ZombiesManager::allZombies[z].begin(); it < ZombiesManager::allZombies[z].end(); it++)
	{
		Zombie* zombie = *it;
		if (zombie->status != Die && (zombie->position.x - peaShooterBullet->position.x) < 0.5f && (zombie->position.x - peaShooterBullet->position.x)>0)
		{
			zombie->BeAttacked(peaShooterBullet->damage);     // 僵尸受到攻击，减少生命值
			//创建一个爆炸粒子效果并添加到粒子效果列表中
			PeaShooterBulletBoom* boom = new PeaShooterBulletBoom(peaShooterBullet->position);
			allBooms.push_back(boom);
			return true; // 子弹击中僵尸
		}
	}
	return false;
}

void PlantsManager::UpdatePeaShooterBullets()
{
	for (auto it = allBullets.begin(); it != allBullets.end(); )
	{
		PeaShooterBullet* bullet = *it;
		bullet->Update();
		// 如果子弹超出范围或碰撞到僵尸，则删除子弹
		if (bullet->position.x > 25 || CheckPeaShooterBullets(bullet))
		{
			it = allBullets.erase(it);
			delete bullet;
		}
		else
		{
			++it;
		}
	}
}



void PlantsManager::UpdatePlant()
{
	//按1生成向日葵到列表
	if (Keys::TriggerKey(VK_NUMPAD1))
	{
		if (SunManager::ReduceSun(Sunflower::plantCost) == false)
		{
			MessageBox(NULL, "阳光不足，无法种植!", "提示", MB_OK);
			return;
		}
		int cellX = goldRing.currentPos.x / 2;
		int cellZ = goldRing.currentPos.z / 2;
		allPlant[cellZ][cellX] = new Sunflower(Vector3(cellX * 2, 2, cellZ * 2), Vector3(0, 90, 0), Vector3(0.01, 0.01, 0.01));

	}

	//按2生成豌豆射手到列表
	else if (Keys::TriggerKey(VK_NUMPAD2))
	{
		if (SunManager::ReduceSun(PeaShooter::plantCost) == false)
		{
			MessageBox(NULL, "阳光不足，无法种植!", "提示", MB_OK);
			return;
		}
		int cellX = goldRing.currentPos.x / 2;
		int cellZ = goldRing.currentPos.z / 2;
		allPlant[cellZ][cellX] = new PeaShooter(Vector3(cellX * 2, 2, cellZ * 2), Vector3(0, 0, 0), Vector3(0.01, 0.01, 0.01));
	}

	//按3生成土豆
	else if (Keys::TriggerKey(VK_NUMPAD3))
	{
		if (SunManager::ReduceSun(Potato::plantCost) == false)
		{
			MessageBox(NULL, "阳光不足，无法种植!", "提示", MB_OK);
			return;
		}
		int cellX = goldRing.currentPos.x / 2;
		int cellZ = goldRing.currentPos.z / 2;
		allPlant[cellZ][cellX] = new Potato(Vector3(cellX * 2, 2, cellZ * 2), Vector3(0, 90, 0), Vector3(0.01, 0.01, 0.01));
	}
}

void PlantsManager::UpdatePlantDie()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (allPlant[i][j] && allPlant[i][j]->GetHP() <= 0)
			{
				delete allPlant[i][j];
				allPlant[i][j] = nullptr;
			}
		}
	}
}

void PlantsManager::UpdatePlantsWork()
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (allPlant[i][j])
			{
				allPlant[i][j]->UpdatePlant();
			}
		}
	}
}

void PlantsManager::UpdateBoom()
{
	for (auto it = allBooms.begin(); it != allBooms.end();)
	{
		PeaShooterBulletBoom* boom = *it;
		boom->Update();
		// 如果粒子效果的生命周期结束，则删除粒子效果
		if (boom->isActive == false)
		{
			it = allBooms.erase(it);
			delete boom;
		}
		else
		{
			++it;
		}
	}
}

void PlantsManager::Update()
{
	if (Keys::TriggerKey(VK_UP))
	{
		goldRing.MoveGoldRing(0, -1);
	}

	if (Keys::TriggerKey(VK_DOWN))
	{
		goldRing.MoveGoldRing(0, 1);
	}

	if (Keys::TriggerKey(VK_LEFT))
	{
		goldRing.MoveGoldRing(-1, 0);
	}

	if (Keys::TriggerKey(VK_RIGHT))
	{
		goldRing.MoveGoldRing(1, 0);
	}

	UpdatePlant();
	UpdatePlantDie();
	UpdatePlantsWork();
	UpdatePeaShooterBullets();
	UpdateBoom();
}

void PlantsManager::Draw()
{
	/** 渲染金环 */
	goldRing.Render();

	//渲染所有植物
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (allPlant[i][j])
			{
				allPlant[i][j]->draw();
			}
		}
	}

	//渲染所有子弹
	for (auto it = allBullets.begin(); it != allBullets.end(); it++)
	{
		PeaShooterBullet* bullet = *it;
		bullet->Draw();
	}

	//渲染所有爆炸粒子效果
	for (auto it = allBooms.begin(); it != allBooms.end(); it++)
	{
		PeaShooterBulletBoom* boom = *it;
		boom->Render();
	}
}
