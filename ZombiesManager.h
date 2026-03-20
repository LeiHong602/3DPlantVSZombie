#pragma once
#include "Zombie.h"
#include"bits/stdc++.h"
//#include <GL/glut.h>
//#include"CommonZombie.h"

using namespace std;

class ZombiesManager
{
public:
	friend class PlantsManager;
	friend class GameManager;

	static unsigned int shader; // 着色器程序ID

	ZombiesManager();

	static void Update();// 更新所有僵尸的位置和状态,和生成新僵尸

	static void Draw();// 绘制所有僵尸
	
private:
	static vector<vector<class Zombie*>> allZombies;
	static double currentTime; // 当前时间
	static double lastTime;    // 上次生成时间
	static double spawnInterval; // 生成间隔（秒）

	static bool isGameOver; // 游戏是否结束
	static bool isHaveZomebies; // 是否有僵尸
	static int maxZombies; // 最大僵尸数量
	static int totalZombiesSpawned; // 已生成的僵尸总数


};

