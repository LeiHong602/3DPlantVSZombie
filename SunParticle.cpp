#include "SunParticle.h"
#include "stdafx.h"

SunParticle::SunParticle(Vector3& pos)
{
	currentPos.x = pos.x;
	currentPos.z = pos.z;
	Init(2000);
}

/** 金色圆环粒子的初始化 */
bool SunParticle::Init(int num)
{
	/** 初始化粒子数目为num个 */
	if (CParticle::Init(num))
	{

		for (int i = 0; i < m_iNum; i++)
		{
			/** 计算粒子在圆环上的位置 */
			float angle = 2 * 3.1415926f * (rand() % m_iNum) / m_iNum; // 等分圆周角度

			// 内半径和外半径之间的随机半径
			float radius = ringThickness * 0.45f + (rand() % 1000) / 1000.0f * ringThickness * 0.55f;
			radius += ringRadius - ringThickness * 0.75f;

			m_pList[i].radius = radius;

			float x = radius * cos(angle);
			float y = 0.03f * (rand() % 20 - 10) + rand() % 5 * 0.5; // Y轴方向的小范围波动，形成厚度
			float z = radius * sin(angle);

			m_pList[i].position = Vector3(x, y, z);

			/** 设置旋转速度 - 切线方向 */
			float speed = 0.008f + (rand() % 1000) / 1000000.0f; // 基础速度加随机变化
			float vx = -speed * sin(angle); // 切线方向的速度分量
			float vz = speed * cos(angle);   // 切线方向的速度分量
			float vy = 0.03;                 // Y轴方向

			m_pList[i].velocity = Vector3(vx, vy, vz);

			/** 向心力加速度，保持圆环形状 */
			float centripetalAccel = speed * speed / radius;
			float ax = -centripetalAccel * cos(angle);
			float az = -centripetalAccel * sin(angle);

			m_pList[i].acceleration = Vector3(ax, 0.0f, az);

			/** 初始化粒子生命时间*/
			m_pList[i].lifetime = 1500;

			/** 初始化粒子的尺寸 */
			m_pList[i].size = 0.025f;

			/** 初始化粒子的消失速度 */
			m_pList[i].dec = 1;

			/** 初始化粒子的颜色 - 金色渐变 */
			float goldenRatio = (float)(rand() % 1000) / 1000.0f;
			m_pList[i].color[0] = 255.0f * (0.95f + 0.05f * goldenRatio);
			m_pList[i].color[1] = 215.0f * (0.85f + 0.15f * goldenRatio);
			m_pList[i].color[2] = 0.0f;
		}

		/** 载入粒子纹理 */
		if (!m_texture.Load("Data/flare1.bmp"))
		{
			MessageBox(NULL, "载入粒子纹理失败!", "错误", MB_OK);
			return false;
		}
		else
			return true;
	}
	else
		return false;
}

/** 金色圆环粒子的渲染 */
void SunParticle::Render()
{
	glPushMatrix();
	/** 启用混合以实现透明效果 */
	glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_LIGHTING);
	/** 绑定纹理 */
	glBindTexture(GL_TEXTURE_2D, m_texture.ID);

	glTranslatef(currentPos.x, currentPos.y, currentPos.z);//移动到特定位置

	bool flag = false; //是否有粒子存活的标志
	/** 开始渲染圆环 */
	for (int i = 0; i < m_iNum; ++i)
	{
		if (m_pList[i].lifetime <= 0)
			continue; //跳过死亡的粒子

		flag = true; //有粒子存活

		float x = m_pList[i].position.x;
		float y = m_pList[i].position.y;
		float z = m_pList[i].position.z;
		float size = m_pList[i].size;

		glColor3fv(m_pList[i].color);
		glNormal3f(0.0f, 0.0f, 1.0f);     /**< 定义法线方向 */

		/** 画出粒子 */
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - size, y - size, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - size, y + size, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + size, y - size, z);
		glEnd();
	}
	if (flag == false) 
		isActive = false;//如果没有粒子存活了，设置粒子不活动

	/** 禁用混合 */
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	/** 更新粒子属性 */
	Update();
}

/** 金色圆环粒子的更新 */
void SunParticle::Update()
{
	for (int i = 0; i < m_iNum; ++i)
	{
		/** 更新位置 */
		m_pList[i].position.x += m_pList[i].velocity.x;
		m_pList[i].position.z += m_pList[i].velocity.z;
		m_pList[i].position.y += m_pList[i].velocity.y;

		/** 更新速度 */
		m_pList[i].velocity.x += m_pList[i].acceleration.x;
		m_pList[i].velocity.z += m_pList[i].acceleration.z;

		/** 更新生存时间 */
		m_pList[i].lifetime -= m_pList[i].dec;

	}
}
