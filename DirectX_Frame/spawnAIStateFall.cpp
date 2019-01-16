//======================================================================
//	Gl~[¶¬AIÙ£xº~óÔ@i2018/12/24j
//	Author : Lº ¹r
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

	if (CModeGame::GetEnemyCount() <= 15)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStatePause(m_EnemyManager));
	}
}
