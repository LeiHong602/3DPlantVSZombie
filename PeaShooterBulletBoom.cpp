#include "PeaShooterBulletBoom.h"
#include "stdafx.h"

PeaShooterBulletBoom::PeaShooterBulletBoom(const Vector3& pos)
{
	currentPos = pos;
    Init(20);
}

/** 爆炸粒子的初始化 */
bool PeaShooterBulletBoom::Init(int num)
{
    /** 初始化粒子数目为num个 */
    if (CParticle::Init(num))
    {
        for (int i = 0; i < m_iNum; i++)
        {
            /** 所有粒子从中心位置开始 */
            m_pList[i].position = Vector3(0.0f, 0.0f, 0.0f);

            /** 生成随机的球面方向向量 */
            float theta = 2.0f * 3.1415926f * (rand() % 1000) / 1000.0f; // 方位角 0-2π
            float phi = acos(2.0f * (rand() % 1000) / 1000.0f - 1.0f);   // 极角，保证均匀分布在球面上

            /** 爆炸速度*/
            float baseSpeed = 0.04f; // 基础爆炸速度

            /** 计算球面坐标到直角坐标的转换 */
            float vx = baseSpeed * sin(phi) * cos(theta);
            float vy = baseSpeed * sin(phi) * sin(theta);
            float vz = baseSpeed * cos(phi);

            m_pList[i].velocity = Vector3(vx, vy, vz);

            /** 添加轻微的随机扰动 */
            float randomFactor = 0.01f;
            m_pList[i].velocity.x += randomFactor * (rand() % 200 - 100) / 100.0f;
            m_pList[i].velocity.y += randomFactor * (rand() % 200 - 100) / 100.0f;
            m_pList[i].velocity.z += randomFactor * (rand() % 200 - 100) / 100.0f;

            /** 设置阻力加速度，模拟空气阻力导致减速 */
            float dragForce = 0.002f + (rand() % 500) / 100000.0f;
            m_pList[i].acceleration = Vector3(
                -dragForce * vx,
                -dragForce * vy,
                -dragForce * vz
            );

            /** 设置粒子生命周期 - 比圆环短很多，模拟爆炸的短暂性 */
            m_pList[i].lifetime = 5 + rand() % 40; // 30-70帧的生命周期


            /** 设置粒子初始尺寸 - 稍大一些，更符合爆炸效果 */
            m_pList[i].size = 0.1f + (rand() % 1000) / 50000.0f;

            /** 设置粒子的消失速度 */
            m_pList[i].dec = 1.0f;

        }

        /** 载入粒子纹理 */
        if (!m_texture.Load("Data/flare2.bmp"))
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

/** 爆炸粒子的渲染 */
void PeaShooterBulletBoom::Render()
{
    glPushMatrix();
    /** 启用混合以实现透明效果 */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_LIGHTING);
    /** 绑定纹理 */
    glBindTexture(GL_TEXTURE_2D, m_texture.ID);

    glTranslatef(currentPos.x, currentPos.y, currentPos.z); // 移动到爆炸位置

	bool flag = true;//是否有粒子存活的标志

    /** 开始渲染所有粒子 */
    for (int i = 0; i < m_iNum; ++i)
    {
		if (m_pList[i].lifetime <= 0)
			continue; /**< 如果粒子生命结束则跳过 */
		flag = false;
        float x = m_pList[i].position.x;
        float y = m_pList[i].position.y;
        float z = m_pList[i].position.z;

        glColor3f(1, 1, 1);
        glNormal3f(0.0f, 0.0f, 1.0f); /**< 定义法线方向 */

        /** 画出粒子 */
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - m_pList[i].size, y - m_pList[i].size, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x - m_pList[i].size, y + m_pList[i].size, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + m_pList[i].size, y + m_pList[i].size, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + m_pList[i].size, y - m_pList[i].size, z);
        glEnd();
    }

	if (flag) isActive = false;//如果所有粒子都死亡了，设置粒子不活动

    /** 禁用混合 */
    glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
    glPopMatrix();

    /** 更新粒子属性 */
    Update();
}

/** 爆炸粒子的更新 */
void PeaShooterBulletBoom::Update()
{
    for (int i = 0; i < m_iNum; ++i)
    {
        /** 更新位置 */
        m_pList[i].position.x += m_pList[i].velocity.x;
        m_pList[i].position.y += m_pList[i].velocity.y;
        m_pList[i].position.z += m_pList[i].velocity.z;

        /** 更新速度（受阻力影响） */
        m_pList[i].velocity.x += m_pList[i].acceleration.x;
        m_pList[i].velocity.y += m_pList[i].acceleration.y;
        m_pList[i].velocity.z += m_pList[i].acceleration.z;

        /** 更新生存时间 */
        m_pList[i].lifetime -= m_pList[i].dec;

        /** 粒子逐渐缩小 */
        m_pList[i].size *= 1.0f;

        ///** 如果粒子生命结束 */
        //if (m_pList[i].lifetime <= 0)
        //{
        //    /** 重置粒子到中心位置，准备下一次爆炸 */
        //    m_pList[i].position = Vector3(0.0f, 0.0f, 0.0f);

        //    /** 重新生成随机的球面方向向量 */
        //    float theta = 2.0f * 3.1415926f * (rand() % 1000) / 1000.0f;
        //    float phi = acos(2.0f * (rand() % 1000) / 1000.0f - 1.0f);

        //    float baseSpeed = 0.04f;

        //    float vx = baseSpeed * sin(phi) * cos(theta);
        //    float vy = baseSpeed * sin(phi) * sin(theta);
        //    float vz = baseSpeed * cos(phi);

        //    m_pList[i].velocity = Vector3(vx, vy, vz);

        //    float randomFactor = 0.01f;
        //    m_pList[i].velocity.x += randomFactor * (rand() % 200 - 100) / 100.0f;
        //    m_pList[i].velocity.y += randomFactor * (rand() % 200 - 100) / 100.0f;
        //    m_pList[i].velocity.z += randomFactor * (rand() % 200 - 100) / 100.0f;

        //    float dragForce = 0.002f + (rand() % 500) / 100000.0f;
        //    m_pList[i].acceleration = Vector3(
        //        -dragForce * vx,
        //        -dragForce * vy,
        //        -dragForce * vz
        //    );

        //    m_pList[i].lifetime = 5 + rand() % 40;
        //    m_pList[i].size = 0.1f + (rand() % 1000) / 50000.0f;
        //    m_pList[i].dec = 1.0f;

        //}
    }
}

///** 启动爆炸效果（可选方法，用于手动触发爆炸） */
//void PeaShooterBulletBoom::TriggerExplosion()
//{
//    for (int i = 0; i < m_iNum; ++i)
//    {
//        /** 重置所有粒子到中心，开始新的爆炸 */
//        m_pList[i].position = Vector3(0.0f, 0.0f, 0.0f);
//
//        float theta = 2.0f * 3.1415926f * (rand() % 1000) / 1000.0f;
//        float phi = acos(2.0f * (rand() % 1000) / 1000.0f - 1.0f);
//
//        float baseSpeed = 0.12f + (rand() % 1000) / 4000.0f; // 触发时速度更快
//
//        float vx = baseSpeed * sin(phi) * cos(theta);
//        float vy = baseSpeed * sin(phi) * sin(theta);
//        float vz = baseSpeed * cos(phi);
//
//        m_pList[i].velocity = Vector3(vx, vy, vz);
//
//        m_pList[i].lifetime = 25 + rand() % 35; // 生命周期稍短
//    }
//}