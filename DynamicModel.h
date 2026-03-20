#pragma once
#include <iostream>
#include "Model.h"
#include "Animator.h"

class DynamicModel
{
public:
	/// <summary>
	/// 带参的构造函数
	/// </summary>
	/// <param name="pos">模型位置</param>
	/// <param name="rot">模型旋转轴</param>
	/// <param name="rad">模型旋转角度</param>
	/// <param name="scale">模型缩放</param>
	/// <param name="shader">shader</param>
	DynamicModel(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader);
	DynamicModel();
	~DynamicModel();

	int DrawDynamicModel();//绘制动态模型
	int DynamicModelUpdata();//模型的每帧更新
	glm::vec3 ReturnPosition() { return position; }//返回模型位置

protected:
	Model* myModel;//模型
	Animator* animator;//动画
	unsigned int shader;
	glm::mat4 model;//位置旋转和缩放矩阵

	glm::vec3 position;
	glm::vec3 rotationAxis;
	float rotationAngle;
	glm::vec3 scale;
};

