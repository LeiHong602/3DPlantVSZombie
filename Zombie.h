#pragma once
#include "DynamicModel.h"
#include <GLFW/glfw3.h> // 添加此头文件以解决未定义标识符 "glfwGetTime" 的问题
#include "Plant.h"
#include "PlantsManager.h"

enum Status
{
	Move,
	Attack,
	Die,
	DieDie
};

class Zombie :public DynamicModel
{
public:
	friend class PlantsManager;
	friend class ZombiesManager;
	Zombie();
	Zombie(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader);


	void MoveUpdate(); // 更新僵尸位置
	void AttackUpdate(); // 更新僵尸攻击
	void DieUpdate(); // 更新僵尸死亡

	void UpdateZombie();

	void BeAttacked(int damage); // 受到攻击，减少生命值

	virtual void ChangeToDieAnimation() = 0; // 切换死亡动画

	virtual void ChangeToAttackAnimation() = 0; // 切换到攻击动画

	virtual void ChangeToMoveAnimation() = 0; // 切换到移动动画

protected:
	double currentTime; // 当前时间
	double lastTime;    // 上次生成时间
	float speed = 1.0f; // 僵尸移动速度
	int HP;             // 僵尸生命值
	Status status = Move; // 僵尸当前状态

	int attackDamage = 10; // 僵尸攻击力
	float attackInterval = 1.0f; // 攻击间隔（秒）
	double lastAttackTime = 0.0; // 上次攻击时间
	Plant* targetPlant = nullptr; // 目标植物
};

