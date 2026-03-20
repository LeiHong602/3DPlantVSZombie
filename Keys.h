#pragma once

class Keys
{
public:

	/** 构造函数 */
	Keys() { Clear(); }

	/** 清空所有的按键信息 */
	void Clear() 
	{ 
		for (int i = 0; i < sizeof(m_KeyDown);i++)
		{
			m_KeyDown[i] = 0;
		}
		for (int i = 0; i < sizeof(m_KeyTrigger); i++)
		{
			m_KeyTrigger[i] = 0;
		}
	}

	/** 判断某个键是否按下 */
	bool IsPressed(unsigned int key) { return (key < MAX_KEYS) ? (m_KeyDown[key] == true) : false; }

	//触发某个按键
	static bool TriggerKey(unsigned int key)
	{
		if (key < MAX_KEYS)
		{
			if (m_KeyTrigger[key] == true)
			{
				m_KeyTrigger[key] = false;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/** 设置某个键被按下 */
	void SetPressed(unsigned int key)
	{
		if (key < MAX_KEYS)
		{
			m_KeyDown[key] = true;
			m_KeyTrigger[key] = true;
		}
	}

	/** 设置某个键被释放 */
	void SetReleased(unsigned int key)
	{
		if (key < MAX_KEYS)
		{
			m_KeyDown[key] = false;
			m_KeyTrigger[key] = false;
		}
	}

private:
	static const unsigned int MAX_KEYS = 256;
	static bool m_KeyDown[MAX_KEYS];									/**< 保存256个按键的状态 */
	static bool m_KeyTrigger[MAX_KEYS];                         //按键触发器
};

