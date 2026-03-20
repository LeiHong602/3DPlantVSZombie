#include "ZombiesManager.h"
#include "CommonZombie.h"
#include "GLQZombie.h"

vector<vector<Zombie*>> ZombiesManager::allZombies = vector<vector<Zombie*>>(7, vector<Zombie*>());
double ZombiesManager::currentTime = 0.0; // 当前时间
double ZombiesManager::lastTime = 0.0;    // 上次生成时间
double ZombiesManager::spawnInterval = 10.0; // 生成间隔（秒）
unsigned int ZombiesManager::shader = 0; // 着色器程序ID
int ZombiesManager::totalZombiesSpawned = 0; // 已生成的僵尸数量
int ZombiesManager::maxZombies = 8; // 最大僵尸数量
bool ZombiesManager::isGameOver = false; // 游戏是否结束
bool ZombiesManager::isHaveZomebies = true; // 是否有僵尸

ZombiesManager::ZombiesManager()
{
}



void ZombiesManager::Update()
{
	if (totalZombiesSpawned < maxZombies)
	{
		currentTime = glfwGetTime(); // 使用std::time获取当前时间，单位为秒

		if (currentTime - lastTime >= spawnInterval) // 每隔spawnInterval秒生成一个僵尸
		{
			int row = rand() % 7; // 随机选择一行
			int n = rand() % 2;//随机一个数字，0或1
			Zombie* newZombie;
			if (n == 0)
			{
				newZombie = new GLQZombie(glm::vec3(20, 1.1, row * 2), glm::vec3(0, 1, 0), -90, glm::vec3(0.02f), shader);
			}
			else if (n == 1)
			{
				newZombie = new CommonZombie(glm::vec3(20, 1.1, row * 2), glm::vec3(0, 1, 0), -90, glm::vec3(0.02f), shader);
			}
			allZombies[row].push_back(newZombie);
			lastTime = currentTime;
			++totalZombiesSpawned;
		}
	}
   
   // 更新所有僵尸
	ZombiesManager::isHaveZomebies = false;
   for (int row = 0; row < allZombies.size(); ++row)
   {
	   for (int i = 0; i < allZombies[row].size(); ++i)
	   {
		   if (allZombies[row][i])
		   {
			   ZombiesManager::isHaveZomebies = true;
			   allZombies[row][i]->DynamicModelUpdata();
			   allZombies[row][i]->UpdateZombie();
			   if (allZombies[row][i]->status == DieDie) // 僵尸彻底死亡，移除
			   {
				   delete allZombies[row][i];
				   allZombies[row].erase(allZombies[row].begin() + i);
				   --i; // 调整索引以避免跳过下一个僵尸
			   }
		   }
	   }
   }

   // 判断游戏是否结束
   if (!ZombiesManager::isHaveZomebies && totalZombiesSpawned == maxZombies)
   {
	   ZombiesManager::isGameOver = true;
   }
}



void ZombiesManager::Draw()
{
	for (int row = 0; row < allZombies.size(); ++row)
	{
		for (int i = 0; i < allZombies[row].size(); ++i)
		{
			if (allZombies[row][i])
			{
				allZombies[row][i]->DrawDynamicModel();
			}
		}
	}
}
