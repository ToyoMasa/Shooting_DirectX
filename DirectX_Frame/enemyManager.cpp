//======================================================================
//	エネミー管理　（2018/11/22）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "waypoint.h"
#include "scene3D.h"
#include "field.h"
#include "enemyManager.h"
#include "enemyPatternWaypoints.h"
#include "zombie.h"

CEnemyManager::CEnemyManager(CField* field)
{
	m_CountFrame = 0;
	m_Field = field;
	m_ZombieSpawnRate = 100;

	CZombie::ZombieInit();

	for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
	{
		m_Zombies[i] = NULL;
	}
}

CEnemyManager::~CEnemyManager()
{
	for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
	{
		if (m_Zombies[i])
		{
			m_Zombies[i]->Release();
		}
		m_Zombies[i] = NULL;
	}
	CZombie::ZombieUninit();
}

void CEnemyManager::Update()
{
	if (m_CountFrame % m_ZombieSpawnRate == 0)
	{
		CreateEnemy(ENEMY_TYPE_ZOMBIE);
	}

	// フレームをインクリメント
	m_CountFrame++;
}

void CEnemyManager::CreateEnemy(ENEMY_TYPE type)
{
	switch (type)
	{
	case ENEMY_TYPE_ZOMBIE:
		for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
		{
			if (m_Zombies[i] == NULL)
			{
				D3DXVECTOR3 spawnPos;
				int numWayPoint = CWayPoint::GetWayPoints().size();
				spawnPos = CWayPoint::GetWayPointPos(rand() % numWayPoint);

				m_Zombies[i] = CZombie::Create(
					(SKINMESH_MODEL_ID)(SM_ID_ZOMBIE_A + rand() % 2),
					spawnPos,
					new CEnemyPatternWaypoints(),
					m_Field);
				break;
			}
		}
		break;
	default:
		break;
	}
}
