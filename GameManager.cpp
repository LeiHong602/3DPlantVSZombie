#include "GameManager.h"
bool GameManager::isFirstSuccess = true;
bool GameManager::isGameSuccess = false;
void GameManager::Update()
{
	//如果游戏失败则弹出对话框结束游戏
	if (IsGameFailed())
	{
		//弹出失败对话框
		MessageBox(NULL, "僵尸吃掉了你的脑子", "Game Over!", MB_OK);
		//结束游戏
		exit(0);
	}
	else if (IsGameSuccess())
	{
		//弹出成功对话框
		if (isFirstSuccess)
		{
			MessageBox(NULL, "你成功地消灭了所有僵尸", "Congratulations!", MB_OK);
			isFirstSuccess = false;
			isGameSuccess = true;
		}
		
	}
}

bool GameManager::IsGameFailed()
{
	for (int row = 0; row < ZombiesManager::allZombies.size(); ++row)
	{
		for (int i = 0; i < ZombiesManager::allZombies[row].size(); ++i)
		{
			if (ZombiesManager::allZombies[row][i]->ReturnPosition().x < -3)
			{
				return true;
			}
		}
	}
	return false;
}

bool GameManager::IsGameSuccess()
{
	if (ZombiesManager::isGameOver)
	{
		return true;
	}

	return false;
}


