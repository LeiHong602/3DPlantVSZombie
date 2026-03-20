#include "GLQZombie.h"

GLQZombie::GLQZombie()
{
	HP = 200;
}

GLQZombie::GLQZombie(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader) :Zombie(pos, rot, rad, scale, shader)
{
	myModel = new Model("assets/GLQZombie/GLQZombie_Move.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
	//LoadModel("assets/Dance02.fbx");
	HP = 200;
}

void GLQZombie::ChangeToDieAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/GLQZombie/GLQZombie_Die.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}

void GLQZombie::ChangeToAttackAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/GLQZombie/GLQZombie_Attack.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}

void GLQZombie::ChangeToMoveAnimation()
{
	delete animator;
	delete myModel;

	myModel = new Model("assets/GLQZombie/GLQZombie_Move.fbx");//加载模型

	//加载动画
	if (myModel)
	{
		animator = new Animator(myModel->GetAnimation());
	}
}
