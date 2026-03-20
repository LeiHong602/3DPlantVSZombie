#pragma once
#include "BMPLoader.h"
#include "Particle.h"

class PeaShooterBulletBoom : public CParticle
{
public:
	PeaShooterBulletBoom(const Vector3& pos);
	~PeaShooterBulletBoom() {};

	bool		Init(int num);  /**< 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */

	bool isActive = true; //粒子是否还在活动



private:
	CBMPLoader  m_texture;       /**< 粒子的纹理 */
	Vector3 currentPos = Vector3(0, 2, 0); //爆炸的位置

};

