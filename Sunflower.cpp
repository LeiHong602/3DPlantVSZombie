#include "Sunflower.h"


Sunflower::Sunflower()
{
	//加载向日葵
	if (!loadMTL("materials/Model/Sunflower/Sunflower.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/Sunflower/Sunflower.obj"))
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

Sunflower::Sunflower(const Vector3& pos, const Vector3& rot, const Vector3& sca):Plant(pos, rot, sca)
{
	//加载向日葵
	if (!loadMTL("materials/Model/Sunflower/Sunflower.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/Sunflower/Sunflower.obj"))
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

void Sunflower::PlantWork()
{
	// 向日葵的工作逻辑，例如生产阳光
	currentProduceTime = glfwGetTime();
	if (lastProduceTime == 0) lastProduceTime = currentProduceTime;
	if (currentProduceTime - lastProduceTime >= produceInterval) {
		// 生产阳光的代码
		SunManager::AddSun(produceSunValue); // 添加阳光数量
		SunParticle* newSun = new SunParticle(m_position);
		SunManager::AddSunParticle(newSun);
		lastProduceTime = currentProduceTime;
	}
}
