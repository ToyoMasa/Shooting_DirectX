//======================================================================
//	エネミー管理　（2018/11/22）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "game.h"
#include "waypoint.h"
#include "scene3D.h"
#include "field.h"
#include "player.h"
#include "enemyManager.h"
#include "enemyPatternWaypoints.h"
#include "enemyPatternWaypointsRun.h"
#include "zombie.h"
#include "spawnAIStateRise.h"
#include "spawnAIStateStop.h"
#include "tutorialShoot.h"

CEnemyManager::CEnemyManager(CField* field)
{
	m_CountFrame = 0;
	m_Field = field;
	m_ZombieSpawnRate = 100;
	m_PlayerTension = 0.0f;
	m_SpawnAIState = 0;
	m_isStart = false;

	//m_SpawnAI = new CSpawnAIStateStop(this);
	m_SpawnAI = new CSpawnAIStateRise(this);

	CEnemy::SetPatternState(CEnemy::PATTERN_STATE_NORMAL);
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

	if (m_SpawnAI != NULL)
	{
		delete m_SpawnAI;
		m_SpawnAI = NULL;
	}
}

void CEnemyManager::Update()
{
	if (!m_isStart)
	{
		return;
	}

	m_SpawnAI->Update();

	// フレームをインクリメント
	m_CountFrame++;

	if (CManager::GetDebug())
	{
		ImGui::Begin("Debug");
		ImGui::Text("ENEMY_NUM:%d", CModeGame::GetEnemyCount());
		ImGui::Text("PLAYER_TENSION:%.2f", m_PlayerTension);
		ImGui::Text("SPAWN_STATE:%d", m_SpawnAIState);
		ImGui::End();
	}
	//ImGui::SetNextWindowSize(ImVec2(80, 50), ImGuiSetCond_Once);
	//ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 120.0f, 20), ImGuiSetCond_Once);
	//ImGui::Begin("Heat");
	//ImGui::Text("%.2f", m_PlayerTension);
	//ImGui::End();
}

void CEnemyManager::CreateEnemy(ENEMY_TYPE type)
{
	CEnemy::PATTERN_STATE state = CEnemy::GetPatternState();

	switch (type)
	{
	case ENEMY_TYPE_ZOMBIE:
		for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
		{
			if (m_Zombies[i] != NULL)
			{
				continue;
			}

			D3DXVECTOR3 spawnPos;
			//int numWayPoint = CWayPoint::GetWayPoints().size();
			//spawnPos = CWayPoint::GetWayPointPos(rand() % numWayPoint);
			spawnPos = CWayPoint::GetSpawnPointPos(CModeGame::GetPlayer()->GetShortestPoint());

			if (state == CEnemy::PATTERN_STATE_HORDE)
			{
				m_Zombies[i] = CZombie::Create(
					(SKINMESH_MODEL_ID)(SM_ID_ZOMBIE_A + rand() % 2),
					spawnPos,
					new CEnemyPatternWaypointsRun(),
					m_Field);
				break;
			}
			else
			{
				m_Zombies[i] = CZombie::Create(
					(SKINMESH_MODEL_ID)(SM_ID_ZOMBIE_A + rand() % 2),
					spawnPos,
					new CEnemyPatternWaypoints(),
					m_Field);
				break;
			}
			break;
		}
		break;
	default:
		break;
	}
}

void CEnemyManager::ChangeSpawnAI(CSpawnAIStateBase* next)
{
	if (m_SpawnAI != NULL)
	{
		delete m_SpawnAI;
	}

	m_SpawnAI = next;
}

void CEnemyManager::DeleteZombie(CZombie* zombie)
{
	for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
	{
		if (m_Zombies[i] == zombie)
		{
			m_Zombies[i] = NULL;
			break;
		}
	}
}

void CEnemyManager::Start()
{
	for (int i = 0; i < ZOMBIE_NUM_MAX; i++)
	{
		if (m_Zombies[i] != NULL)
		{
			continue;
		}

		m_Zombies[i] = CZombie::Create(
			(SKINMESH_MODEL_ID)(SM_ID_ZOMBIE_A + rand() % 2),
			D3DXVECTOR3(5.0f, 0.0f, -84.0f),
			new CEnemyPatternWaypoints(),
			m_Field);

		m_isStart = true;

		CModeGame::GetPlayer()->ChangeTutorial(new CTutorialShoot());

		break;
	}
}
