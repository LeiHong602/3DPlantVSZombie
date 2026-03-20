#include "CommonZombie.h"

CommonZombie::CommonZombie()
{
	HP = 100;
}

CommonZombie::CommonZombie(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader):Zombie(pos, rot, rad, scale, shader)
{
	myModel = new Model("assets/CommonZombie/CommonZombie_Move.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
	HP = 100;
}

void CommonZombie::ChangeToDieAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/CommonZombie/CommonZombie_Die.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}

void CommonZombie::ChangeToAttackAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/CommonZombie/CommonZombie_Attack.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}

void CommonZombie::ChangeToMoveAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/CommonZombie/CommonZombie_Move.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}

