//======================================================================
//	エネミー生成AI緊張度下降状態　（2018/12/24）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "game.h"
#include "zombie.h"
#include "spawnAIStateFall.h"
#include "spawnAIStatePause.h"

void CSpawnAIStateFall::Update()
{
	m_EnemyManager->SetPlayerTension(0.0f);

	if (CModeGame::GetEnemyCount() <= 0)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStatePause(m_EnemyManager));
	}
}
