#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Vector.h"


class PeaShooterBullet
{
public:
	friend class PlantsManager;
	PeaShooterBullet(const Vector3& pos) :position(pos) {}

	void Update();

	// 绘制子弹
	void Draw();

private:
	Vector3 position;
	float speed = 5.0f; // 子弹速度
	double currentTime; // 当前时间
	double lastTime = 0;    // 上次生成时间
	int damage = 20;      //伤害值
};

