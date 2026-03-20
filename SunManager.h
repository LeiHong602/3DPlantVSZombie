#pragma once
#include "SunParticle.h"
#include "bits/stdc++.h"
using namespace std;

class SunManager
{
public:
	static int GetSunCount() { return sunCount; } // 获取当前阳光数量
	static void AddSun(int amount) { sunCount += amount; } // 增加阳光数量
	static bool ReduceSun(int amount);                 // 减少阳光数量，返回是否成功
	static void AddSunParticle(SunParticle* sun) { allSunParticles.push_back(sun); } // 增加阳光粒子

	static void Update(); // 更新所有阳光粒子

	static void Draw(); // 绘制所有阳光粒子
private:
	static int sunCount; // 当前阳光数量
	static vector<SunParticle*> allSunParticles; // 所有的阳光粒子
};

