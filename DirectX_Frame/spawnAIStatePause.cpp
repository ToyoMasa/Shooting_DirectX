//======================================================================
//	�G�l�~�[����AI�ْ��x�x�~��ԁ@�i2018/12/24�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "game.h"
#include "zombie.h"
#include "spawnAIStatePause.h"
#include "spawnAIStateRise.h"

void CSpawnAIStatePause::Update()
{
	m_Count++;

	if (m_Count >= 150)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStateRise(m_EnemyManager));
	}
}
