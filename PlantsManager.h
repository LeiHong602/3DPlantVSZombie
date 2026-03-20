#pragma once
#include "PeaShooter.h"
#include "GoldRing.h"
#include "Keys.h"
#include "Sunflower.h"
#include "Potato.h"
#include "bits/stdc++.h"
#include "ZombiesManager.h"
#include "PeaShooterBulletBoom.h"
using namespace std;

class PlantsManager
{
public:
	friend class PeaShooter;
	friend class Zombie;
	/// <summary>
	/// 初始化函数
	/// </summary>
	/// <returns>初始化结果</returns>
	static bool Init();

	/// <summary>
	/// 检测豌豆射手子弹是否击中僵尸
	/// </summary>
	static bool CheckPeaShooterBullets(PeaShooterBullet* peaShooterBullet);

	/// <summary>
	/// 子弹数组的更新
	/// </summary>
	static void UpdatePeaShooterBullets();

	/// <summary>
	/// 有关种植行为的更新
	/// </summary>
	static void UpdatePlant();

	/// <summary>
	/// 有关植物死亡的更新
	/// </summary>
	static void UpdatePlantDie();

	static void UpdatePlantsWork();

	static void UpdateBoom();

	/// <summary>
	/// 更新函数
	/// </summary>
	static void Update();

	/// <summary>
	/// 绘制函数
	/// </summary>
	static void Draw();

private:
	static GoldRing goldRing;                     /**< 金环实例 */
	static Plant* allPlant[7][10];                //所有植物数组
	static vector<PeaShooterBullet*> allBullets;  //所有子弹数组
	static vector<PeaShooterBulletBoom*> allBooms; //所有爆炸粒子数组
};

