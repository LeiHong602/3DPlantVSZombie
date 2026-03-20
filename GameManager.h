#pragma once
#include"ZombiesManager.h"

class GameManager
{
public:
	static void Update();

	//判断游戏是否失败
	static bool IsGameFailed();

	//判断游戏是否成功
	static bool IsGameSuccess();

	static bool GetIsGameSuccess() { return isGameSuccess; };

private:
	static bool isFirstSuccess;
	static bool isGameSuccess;
};

