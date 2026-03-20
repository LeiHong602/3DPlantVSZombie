#include "FireworksParticle.h"
#include "stdafx.h"
#include <cmath>
FireworksParticle::FireworksParticle(Vector3& pos)
{
	currentPos.x = pos.x;
	currentPos.y = pos.y;
	currentPos.z = pos.z;
	Init(2000);
}

// 烟花粒子类的实现
bool FireworksParticle::Init(int num)
{
    // 初始化粒子数目
    if (CParticle::Init(num))
    {
        // 初始化升空粒子
        for (int i = 0; i < ROCKET_COUNT; i++) {
            InitRocketParticle(i);
        }

        // 初始化爆炸粒子为休眠状态
        for (int i = ROCKET_COUNT; i < m_iNum; i++) {
            m_pList[i].lifetime = 0; // 休眠状态
        }

        // 载入粒子纹理
        if (!m_texture.Load("Data/flare1.bmp"))
        {
            MessageBox(NULL, "载入粒子纹理失败!", "错误", MB_OK);
            return false;
        }
        return true;
    }
    return false;
}

// 初始化升空粒子
void FireworksParticle::InitRocketParticle(int index)
{
    // 初始位置在地面附近
    m_pList[index].position = Vector3(
        (rand() % 200 - 100) / 100.0f, // X方向随机偏移
        -1.5f,                         // 从底部开始
        (rand() % 200 - 100) / 100.0f  // Z方向随机偏移
    );

    // 升空速度 - 主要向上
    float speed = 0.08f + (rand() % 1000) / 5000.0f;
    m_pList[index].velocity = Vector3(
        (rand() % 200 - 100) / 500.0f, // 轻微的水平随机速度
        speed,
        (rand() % 200 - 100) / 500.0f
    );

    // 加速度 - 模拟重力
    m_pList[index].acceleration = Vector3(0.0f, -0.002f, 0.0f);

    // 生命周期 - 足够升到高处
    m_pList[index].lifetime = 100 + rand() % 50;
    m_pList[index].dec = 0.5f;

    // 升空粒子尺寸
    m_pList[index].size = 0.4f;

    //// 升空粒子颜色 - 明亮色调
    //float r = 0.8f + (rand() % 200) / 1000.0f;
    //float g = 0.8f + (rand() % 200) / 1000.0f;
    //float b = 0.2f + (rand() % 300) / 1000.0f;
    //m_pList[index].color[0] = r * 255;
    //m_pList[index].color[1] = g * 255;
    //m_pList[index].color[2] = b * 255;

    // 设置粒子颜色 - 烟花颜色
    int colorScheme = rand() % 4;
    switch (colorScheme) {
    case 0: // 红色系
        m_pList[index].color[0] = 255;
        m_pList[index].color[1] = 50 + rand() % 100;
        m_pList[index].color[2] = rand() % 50;
        break;
    case 1: // 蓝色系
        m_pList[index].color[0] = rand() % 50;
        m_pList[index].color[1] = 100 + rand() % 100;
        m_pList[index].color[2] = 255;
        break;
    case 2: // 绿色系
        m_pList[index].color[0] = rand() % 50;
        m_pList[index].color[1] = 255;
        m_pList[index].color[2] = 100 + rand() % 100;
        break;
    case 3: // 紫色系
        m_pList[index].color[0] = 180 + rand() % 75;
        m_pList[index].color[1] = rand() % 100;
        m_pList[index].color[2] = 255;
        break;
    }

    // 标记为升空粒子
    m_pList[index].type = ROCKET;
}

// 初始化爆炸粒子
void FireworksParticle::InitExplosionParticle(int index, const Vector3& position)
{
    // 从爆炸中心开始
    m_pList[index].position = position;

    // 生成随机的球面方向向量
    float theta = 2.0f * 3.1415926f * (rand() % 1000) / 1000.0f;
    float phi = acos(2.0f * (rand() % 1000) / 1000.0f - 1.0f);

    // 爆炸速度
    float baseSpeed = 0.08f + (rand() % 1000) / 5000.0f;
    float vx = baseSpeed * sin(phi) * cos(theta);
    float vy = baseSpeed * sin(phi) * sin(theta);
    float vz = baseSpeed * cos(phi);

    m_pList[index].velocity = Vector3(vx, vy, vz);

    // 添加随机扰动
    float randomFactor = 0.02f;
    m_pList[index].velocity.x += randomFactor * (rand() % 200 - 100) / 100.0f;
    m_pList[index].velocity.y += randomFactor * (rand() % 200 - 100) / 100.0f;
    m_pList[index].velocity.z += randomFactor * (rand() % 200 - 100) / 100.0f;

    // 设置阻力加速度
    float dragForce = 0.003f + (rand() % 500) / 100000.0f;
    m_pList[index].acceleration = Vector3(
        -dragForce * vx,
        -dragForce * vy,
        -dragForce * vz
    );

    // 设置粒子生命周期
    m_pList[index].lifetime = 30 + rand() % 40;
    m_pList[index].dec = 1.0f;

    // 设置粒子初始尺寸
    m_pList[index].size = 0.5f + (rand() % 1000) / 50000.0f;

    // 设置粒子颜色 - 烟花颜色
    int colorScheme = rand() % 4;
    switch (colorScheme) {
    case 0: // 红色系
        m_pList[index].color[0] = 255;
        m_pList[index].color[1] = 50 + rand() % 100;
        m_pList[index].color[2] = rand() % 50;
        break;
    case 1: // 蓝色系
        m_pList[index].color[0] = rand() % 50;
        m_pList[index].color[1] = 100 + rand() % 100;
        m_pList[index].color[2] = 255;
        break;
    case 2: // 绿色系
        m_pList[index].color[0] = rand() % 50;
        m_pList[index].color[1] = 255;
        m_pList[index].color[2] = 100 + rand() % 100;
        break;
    case 3: // 紫色系
        m_pList[index].color[0] = 180 + rand() % 75;
        m_pList[index].color[1] = rand() % 100;
        m_pList[index].color[2] = 255;
        break;
    }

    // 标记为爆炸粒子
    m_pList[index].type = EXPLOSION;
}

// 渲染烟花粒子
void FireworksParticle::Render()
{
    glPushMatrix();
    // 启用混合以实现透明效果
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_LIGHTING);
    // 不绑定纹理
    //glBindTexture(GL_TEXTURE_2D, 0);
    //不启用纹理坐标
    glDisable(GL_TEXTURE_2D);



    // 移动烟花到特定位置
    glTranslatef(currentPos.x, currentPos.y, currentPos.z);

    // 渲染所有活跃粒子
    for (int i = 0; i < m_iNum; ++i)
    {
        // 跳过休眠粒子
        if (m_pList[i].lifetime <= 0) continue;

        // 计算透明度
        float alpha = m_pList[i].lifetime / 100.0f;
        if (alpha < 0.1f) alpha = 0.1f;

        float x = m_pList[i].position.x;
        float y = m_pList[i].position.y;
        float z = m_pList[i].position.z;
        float size = m_pList[i].size;

        // 爆炸粒子透明度随生命周期变化
        if (m_pList[i].type == EXPLOSION) {
            size *= (0.5f + 0.5f * alpha); // 尺寸随生命周期减小
        }

        // 设置粒子颜色和透明度
        glColor4f(m_pList[i].color[0] / 255.0f,
            m_pList[i].color[1] / 255.0f,
            m_pList[i].color[2] / 255.0f,
            alpha);
        //glColor3f(1.0f, 0, 0);

        glNormal3f(0.0f, 0.0f, 1.0f); // 定义法线方向

        // 渲染粒子
        glBegin(GL_QUADS);
        glVertex3f(x - size, y - size, z);
        glVertex3f(x - size, y + size, z);
        glVertex3f(x + size, y + size, z);
        glVertex3f(x + size, y - size, z);
        glEnd();
    }

    // 禁用混合
    glDisable(GL_BLEND);
    glPopMatrix();

    // 更新粒子属性
    Update();
}

// 更新烟花粒子
void FireworksParticle::Update()
{
    for (int i = 0; i < m_iNum; ++i)
    {
        // 跳过休眠粒子
        if (m_pList[i].lifetime <= 0) continue;

        // 更新位置
        m_pList[i].position.x += m_pList[i].velocity.x;
        m_pList[i].position.y += m_pList[i].velocity.y;
        m_pList[i].position.z += m_pList[i].velocity.z;

        // 更新速度
        m_pList[i].velocity.x += m_pList[i].acceleration.x;
        m_pList[i].velocity.y += m_pList[i].acceleration.y;
        m_pList[i].velocity.z += m_pList[i].acceleration.z;

        // 更新生存时间
        m_pList[i].lifetime -= m_pList[i].dec;

        // 对于升空粒子
        if (m_pList[i].type == ROCKET) {
            // 粒子逐渐缩小
            m_pList[i].size *= 0.99f;

            // 当升空粒子到达顶点或生命结束时
            if (m_pList[i].position.y > 30.0f || m_pList[i].lifetime <= 0) {
                // 触发爆炸
                TriggerExplosion(i);
            }
        }
        // 对于爆炸粒子
        else if (m_pList[i].type == EXPLOSION) {
            // 粒子逐渐缩小
            m_pList[i].size *= 0.97f;

            // 如果粒子生命结束
            if (m_pList[i].lifetime <= 0) {
                // 休眠粒子
                m_pList[i].lifetime = 0;
            }
        }
    }
}

// 触发爆炸效果
void FireworksParticle::TriggerExplosion(int rocketIndex)
{
    // 记录爆炸位置
    Vector3 explosionPos = m_pList[rocketIndex].position;

    // 重置升空粒子
    InitRocketParticle(rocketIndex);

    // 激活爆炸粒子
    int particlesPerExplosion = (m_iNum - ROCKET_COUNT) / ROCKET_COUNT;
    int startIndex = ROCKET_COUNT + (rocketIndex * particlesPerExplosion);
    int endIndex = startIndex + particlesPerExplosion;

    // 确保不超过粒子总数
    if (endIndex > m_iNum) endIndex = m_iNum;

    for (int i = startIndex; i < endIndex; i++) {
        InitExplosionParticle(i, explosionPos);
    }
}
