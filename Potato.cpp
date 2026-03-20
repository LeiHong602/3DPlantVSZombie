#include "Potato.h"

Potato::Potato()
{
	HP = 250;
	//加载土豆
	if (!loadMTL("materials/Model/Potato/Potato.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/Potato/Potato.obj"))
	{
		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
	}
	// 加载纹理
	for (auto& mat : materials)
	{
		if (!mat.map_Kd.empty()) {
			mat.textureID = loadTexture(mat.map_Kd.c_str());
		}
	}
}

Potato::Potato(const Vector3& pos, const Vector3& rot, const Vector3& sca) :Plant(pos, rot, sca)
{
	HP = 250;
	//加载土豆
	if (!loadMTL("materials/Model/Potato/Potato.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/Potato/Potato.obj"))
	{
		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
	}
	// 加载纹理
	for (auto& mat : materials)
	{
		if (!mat.map_Kd.empty()) {
			mat.textureID = loadTexture(mat.map_Kd.c_str());
		}
	}
}

void Potato::PlantWork()
{
	return;
}
