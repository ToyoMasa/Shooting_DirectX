//======================================================================
//	Gl~[¶¬AIÙ£xx~óÔ@i2018/12/24j
//	Author : Lº ¹r
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
	m_EnemyManager->SetHeat(0.0f);

	if (m_Count >= 420)
	{
		m_EnemyManager->ChangeSpawnAI(new CSpawnAIStateRise(m_EnemyManager));
	}
}
