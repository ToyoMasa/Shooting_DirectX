//======================================================================
//	�G�l�~�[����AI�ْ��x���~��ԁ@�i2018/12/24�j
//	Author : �L�� ���r
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
