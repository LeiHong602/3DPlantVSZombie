#include "Zombie.h"

Zombie::Zombie()
{
	currentTime = glfwGetTime(); // 使用std::time获取当前时间，单位为秒
	lastTime = currentTime;
}

Zombie::Zombie(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader):DynamicModel( pos, rot, rad, scale, shader)
{
	currentTime = glfwGetTime(); // 使用std::time获取当前时间，单位为秒
	lastTime = currentTime;
}

void Zombie::MoveUpdate()
{
	currentTime = glfwGetTime(); // 使用std::time获取当前时间，单位为秒
	position.x -= speed * (currentTime - lastTime); // 根据时间差更新位置
	lastTime = currentTime;
	model = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis) * glm::scale(glm::mat4(1.0f), scale);

	for (int i = 0; i < 10; ++i)
	{
		int z = (int)(position.z / 2); // 计算僵尸所在的行
		if (PlantsManager::allPlant[z][i] && PlantsManager::allPlant[z][i]->GetHP() > 0 && (position.x - PlantsManager::allPlant[z][i]->GetPosition().x) < 1.0f && (position.x - PlantsManager::allPlant[z][i]->GetPosition().x) > 0)
		{
			status = Attack; // 切换到攻击状态
			targetPlant = PlantsManager::allPlant[z][i]; // 设置目标植物
			currentTime = glfwGetTime(); // 更新当前时间
			lastAttackTime = currentTime; // 重置攻击时间
			ChangeToAttackAnimation(); // 切换到攻击动画
			break;
		}
	}
}

void Zombie::AttackUpdate()
{
	currentTime = glfwGetTime(); // 使用std::time获取当前时间，单位为秒
	if (targetPlant == nullptr) // 如果没有目标植物，切换回移动状态
	{
		status = Move;
		return;
	}
	if (currentTime - lastAttackTime >= attackInterval) // 每隔attackInterval秒攻击一次
	{
		targetPlant->BeAttacked(attackDamage); // 对目标植物造成伤害
		lastAttackTime = currentTime; // 更新上次攻击时间
		if (targetPlant->GetHP() <= 0) // 如果目标植物被摧毁，切换回移动状态
		{
			status = Move;
			ChangeToMoveAnimation(); // 切换回移动动画
			targetPlant = nullptr;
			lastTime = currentTime; // 重置移动时间
		}
	}
}

void Zombie::DieUpdate()
{
	if (animator->isOver)
		status = DieDie; // 切换到最终死亡状态
}

void Zombie::UpdateZombie()
{

	if (status == Move)  
	{
		MoveUpdate();
	}
	else if (status == Attack)
	{
		AttackUpdate();
	}
	else if (status == Die)
	{
		DieUpdate();
	}
}

void Zombie::BeAttacked(int damage)
{
	HP -= damage;
	if (HP < 0) HP = 0; // 确保生命值不为负

	if (HP == 0) 
	{
		status = Die; // 切换到死亡状态
		ChangeToDieAnimation(); // 切换到死亡动画
	}
}
