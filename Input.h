//======================================================================
/**
*  @file      Input.h
*
*  项目描述： DirectInput键盘演示
*   
*  文件描述:  输入系统类CInputSystem
*             其中,类CKeyboard、CMouse、CJoystick分别用来处理键盘、鼠标
*			  和游戏手柄，CInputSystem是输入设备的管理类。
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     WWBOSS
*  电子邮件:  wwboss123@gmail.com
*  创建日期： 2007-07-24	
*  修改日期： 2007-07-24
*/																			
//======================================================================

#ifndef __INPUT_H__
#define __INPUT_H__

#include "stdafx.h"
#include "Vector.h"

#define IS_USEKEYBOARD  1
#define IS_USEMOUSE     2
#define IS_USEJOYSTICK  4

/** 鼠标缓冲区大小 */
#define DINPUT_BUFFERSIZE  16

/** 键盘类 */
class CKeyboard
{
public:
	/** 构造函数和析构函数 */
	CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd);
	~CKeyboard();

	/** 判断某个键是否被按下 */
	bool KeyDown(char key);
	
	/** 判断某个键是否被释放 */
	bool KeyUp(char key);
	
	/** 对键盘数据清零 */
	void Clear();

	/** 更新键盘数据 */
	bool Update();	

	/** 取键盘数据 */
	bool Acquire();

	/** 停止取数据 */
	bool Unacquire();

private:
	/** 键盘设备 */
	LPDIRECTINPUTDEVICE8  m_pDIDev;

	/** 键盘数据区 */
	char    m_keys[256];
};


/** 输入系统类 */
class CInputSystem
{
public:
	/** 构造函数和析构函数 */
	CInputSystem()  {  m_pInput = this;	}

	~CInputSystem() {  ShutDown();     }  

	/** 初始化输入设备 */
	bool Init(HWND hwnd,HINSTANCE appInstance,
		      bool isExclusive,DWORD flags=0);

	/** 释放输入设备资源 */
	bool ShutDown();

	/** 取输入设备的数据 */
	void  AcquireAll();

	/** 停止取输入设备的数据 */
	void  UnacquireAll();

	/** 更新输入设备的参数 */
	void	Update();


	/** 获得键盘 */
	CKeyboard*  GetKeyboard(){  return  m_pKeyboard; }
	
	/** 得到当前的窗口句柄 */
	static HWND GetSysHWnd() {  return  m_hWnd; }
	
	/** 得到当前的输入设备 */
	static CInputSystem* GetSysInput(){  return  m_pInput; }

private:
	/** 输入系统 */
	static CInputSystem*  m_pInput;

	/** 窗口句柄 */
	static HWND  m_hWnd;

	/** 键盘 */
	CKeyboard*    m_pKeyboard;

	/** 输入设备 */
	LPDIRECTINPUT8 m_pDI;

};

//////////////////////////////////////////////////////////////////////////
/**  CKeybord类inline操作和实现 */ 

/** key键是否按下 */
inline bool CKeyboard::KeyDown(char key)
{
	return (m_keys[key] & 0x80) ? true : false;
}

/** key键是否按下 */
inline bool CKeyboard::KeyUp(char key)
{
	return (m_keys[key] & 0x80) ? false : true;
}

/** 对键盘数据清零 */
inline void CKeyboard::Clear()
{
	ZeroMemory(m_keys,256*sizeof(char));
}

/** 取键盘数据 */
inline bool CKeyboard::Acquire()
{
	Clear();
	return (!FAILED(m_pDIDev->Acquire()));
}

/** 停止取数据 */
inline bool CKeyboard::Unacquire()
{
	Clear();
	return (!FAILED(m_pDIDev->Unacquire()));
}


/////////////////////////////////////////////////////////////
/** CInputSystem类的inline操作 */

/** 取输入设备的数据 */
inline void CInputSystem::AcquireAll()
{
	if (m_pKeyboard)	m_pKeyboard->Acquire();
	
}

/** 停止取数据 */
inline void CInputSystem::UnacquireAll()
{
	if (m_pKeyboard)	m_pKeyboard->Unacquire();
	
}


#endif //__INPUT_H__
