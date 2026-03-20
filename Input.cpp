//======================================================================
/**
*  @file      Input.cpp
*
*  项目描述： DirectInput输入系统
*   
*  文件描述:  输入系统类CInputSystem
*             其中,类CKeyboard、CMouse和CJoystick分别用来处理键盘、鼠标
*             和游戏手柄，CInputSystem是输入设备的管理类。
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     WWBOSS
*  电子邮件:  wwboss123@gmail.com
*  创建日期： 2007-07-24	
*  修改日期： 2007-07-24
*/																			
//======================================================================

#include "Input.h"


//************** 键盘类CKeyboard的实现 **************************/
/** 构造函数 */
CKeyboard::CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd)
{
	/** 创建设备对象 */
	if (FAILED(pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL)))
	{
		MessageBox(NULL,"创建键盘设备对象失败！","错误",MB_OK);
	}

    /** 设置数据格式 */
	if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL,"设置键盘数据格式失败！","错误",MB_OK);
	}

    /** 设置协作层次 */ 
	if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		MessageBox(NULL,"设置键盘协作层次失败！","错误",MB_OK);
	}

	/** 捕获设置 */ 
	if (FAILED(m_pDIDev->Acquire()))
	{
		MessageBox(NULL,"捕获键盘设备失败！","错误",MB_OK);
	}

	/** 键盘数据清零 */
	Clear();
}

/** 析构函数 */
CKeyboard::~CKeyboard()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}

/** 更新键盘数据 */
bool CKeyboard::Update()
{
  if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
  {
		if (FAILED(m_pDIDev->Acquire()))
			return false;

		if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys),(LPVOID)m_keys)))
			return false;
	}

	return TRUE;
}



//************** 输入系统类CInputSystem的实现 **************************/

CInputSystem* CInputSystem::m_pInput = NULL;
HWND CInputSystem::m_hWnd = NULL;

/** 初始化设备 */
bool CInputSystem::Init(HWND hwnd,HINSTANCE appInstance,
						bool isExclusive,DWORD flags)
{
	m_pKeyboard = NULL; 

	m_hWnd = hwnd;

	/** 创建DirectInput接口对象 */
	if (FAILED(DirectInput8Create(appInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void **)&m_pDI,NULL)))
	{
		MessageBox(NULL,"创建DirectInput接口对象失败!","错误",MB_OK);
		return false;
	}
    
	/** 创建键盘 */
	if (flags & IS_USEKEYBOARD)
	{
		m_pKeyboard = new CKeyboard(m_pDI, hwnd);
	}


	return true;

}

/** 更新输入设备的参数 */
void CInputSystem::Update()
{
	/** 键盘更新 */
	if (m_pKeyboard)	
		m_pKeyboard->Update();

}

/** 释放输入设备资源 */
bool CInputSystem::ShutDown()
{
	UnacquireAll();

    if (m_pKeyboard)
	{  
		delete m_pKeyboard;
		m_pKeyboard  = NULL;
	}

    
	/** 释放接口对象 */
	if (FAILED(m_pDI->Release()))
		return false;

     return true;
}


