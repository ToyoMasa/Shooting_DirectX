//======================================================================
//	Gl~[¶¬AIÙ£xã¸óÔ@i2018/12/24j
//	Author : Lº ¹r
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "game.h"
#include "zombie.h"
#include "spawnAIStateRise.h"
#include "spawnAIStateMax.h"

void CSpawnAIStateRise::Update()
{
	if (m_EnemyManager->GetFrameCount() % 120 == 0)
	{
		m_EnemyManager->CreateEnemy(ENEMY_TYPE_ZOMBIE);
	}

	if (m_EnemyManager->GetPlayerTension() >= MAX_TENSION)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStateMax(m_EnemyManager));
	}
}
