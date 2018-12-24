//======================================================================
//	�G�l�~�[����AI�ْ��x�ő��ԁ@�i2018/12/24�j
//	Author : �L�� ���r
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
