#pragma once
#include "BMPLoader.h"
#include "Particle.h"

class FireworksParticle : public CParticle
{
public:
	FireworksParticle(Vector3& pos);
	~FireworksParticle() {};

	bool		Init(int num);  /**< 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */
	// 初始化升空粒子
	void InitRocketParticle(int index);

	// 初始化爆炸粒子
	void InitExplosionParticle(int index, const Vector3& position);

	// 触发爆炸效果
	void TriggerExplosion(int rocketIndex);

	bool isActive = true;        //粒子是否还在活动

private:
	int ROCKET_COUNT = 50;        // 升空粒子数量
	CBMPLoader  m_texture;       /**< 粒子的纹理 */
	const float ringRadius = 1.0f;  // 圆环半径
	const float ringThickness = 0.5f; // 圆环厚度（内外半径差）
	Vector3 currentPos = Vector3(0, 1.5, 0); //目前的位置
};

