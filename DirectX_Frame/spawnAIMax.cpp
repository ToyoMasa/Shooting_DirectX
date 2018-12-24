//======================================================================
//	エネミー生成AI緊張度最大状態　（2018/12/24）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "game.h"
#include "zombie.h"
#include "spawnAIStateMax.h"
#include "spawnAIStateFall.h"

void CSpawnAIStateMax::Update()
{
	m_Count++;

	if (CModeGame::GetEnemyCount() < ZOMBIE_NUM_MAX)
	{
		m_EnemyManager->CreateEnemy(ENEMY_TYPE_ZOMBIE);
	}

	if (m_Count >= 300)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStateFall(m_EnemyManager));
	}
}
