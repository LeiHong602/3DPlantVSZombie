#pragma once
#include "BMPLoader.h"
#include "Particle.h"

class SunParticle : public CParticle
{
public:
	SunParticle(Vector3& pos);
	~SunParticle() {};

	bool		Init(int num);  /**< 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */

	bool isActive = true;        //粒子是否还在活动

private:
	CBMPLoader  m_texture;       /**< 粒子的纹理 */
	const float ringRadius = 1.0f;  // 圆环半径
	const float ringThickness = 0.5f; // 圆环厚度（内外半径差）
	Vector3 currentPos = Vector3(0, 1.5, 0); //目前的位置

};

