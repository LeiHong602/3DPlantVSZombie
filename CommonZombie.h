#pragma once
#include "Zombie.h"

class CommonZombie :public Zombie
{
public:
	CommonZombie();
	CommonZombie(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader);

	void ChangeToDieAnimation() override; // ÇÐ»»ËÀÍö¶¯»­
	void ChangeToAttackAnimation() override; // ÇÐ»»µ½¹¥»÷¶¯»­
	void ChangeToMoveAnimation() override; // ÇÐ»»µ½ÒÆ¶¯¶¯»­
};

