//======================================================================
//	�G�l�~�[����AI�ْ��x�㏸��ԁ@�i2018/12/24�j
//	Author : �L�� ���r
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
	if (m_Count % 180 == 0)
	{
		m_EnemyManager->CreateEnemy(ENEMY_TYPE_ZOMBIE);
	}

	if (m_EnemyManager->GetHeat() >= MAX_HEAT)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStateMax(m_EnemyManager));
	}

	m_Count++;
}
