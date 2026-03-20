#include "PeaShooter.h"


PeaShooter::PeaShooter()
{
	//加载豌豆射手
	if (!loadMTL("materials/Model/PeaShooter/PeaShooter.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/PeaShooter/PeaShooter.obj"))
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

PeaShooter::PeaShooter(const Vector3& pos, const Vector3& rot, const Vector3& sca) :Plant(pos, rot, sca)
{
	//加载豌豆射手
	if (!loadMTL("materials/Model/PeaShooter/PeaShooter.mtl"))
	{
		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
	}
	// 加载OBJ模型
	if (!load("materials/Model/PeaShooter/PeaShooter.obj"))
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

void PeaShooter::PlantWork()
{
	// 豌豆射手的工作逻辑，例如发射子弹
	currentShooterTime = glfwGetTime();
	if (lastShootTime == 0) lastShootTime = currentShooterTime;
	if (currentShooterTime - lastShootTime >= shootInterval) {
		// 发射子弹的代码
		PeaShooterBullet* newBullet = new PeaShooterBullet(Vector3(m_position.x+0.5,m_position.y+0.45,m_position.z));
		PlantsManager::allBullets.push_back(newBullet);
		lastShootTime = currentShooterTime;
	}
}
