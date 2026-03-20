//======================================================================
/**
*  @file      Protechny.h
*
*  项目描述： 粒子系统
*  文件描述:  喷泉类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     WWBOSS
*  电子邮件:  wwboss123@gmail.com
*  创建日期： 2006-12-18	
*  修改日期： 2006-12-19
*	
*/																			
//======================================================================

#ifndef  __PROTECHNY_H__
#define  __PROTECHNY_H__


#include "BMPLoader.h"
#include "Particle.h"


/** 喷泉类 */
class GoldRing : public CParticle
{
public:
	friend class PlantsManager;
	GoldRing()  {};
	~GoldRing() {};
	
	bool		Init(int num );  /**< 初始化过程 */
	void		Render();        /**< 渲染过程 */
	void		Update();        /**< 更新过程 */

	/// <summary>
	/// 移动金环，输入移动方向
	/// </summary>
	/// <param name="x">x轴方向</param>
	/// <param name="z">z轴方向</param>
	void MoveGoldRing(int x,int z);


private:
	CBMPLoader  m_texture;       /**< 粒子的纹理 */
	const float ringRadius = 1.0f;  // 圆环半径
	const float ringThickness = 0.5f; // 圆环厚度（内外半径差）
	Vector3 currentPos = Vector3(0,1.5,0); //金环目前的位置

}; 

#endif //__PROTECHNY_H__
