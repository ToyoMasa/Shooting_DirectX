//======================================================================
//	エネミーマネージャー[enemyManager.h]
//
//======================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "enemy.h"
#include "zombie.h"

static const int ZOMBIE_NUM_MAX = 50;
static const float MAX_TENSION = 100.0f;

static const float DAMAGED_TENSION = 15.0f;

class CField;
class CSpawnAIStateBase;

class CEnemyManager
{
public:
	CEnemyManager(CField* field);
	~CEnemyManager();

	void Update();
	void CreateEnemy(ENEMY_TYPE type);
	int GetFrameCount() { return m_CountFrame; }
	void AddPlayerTension(float add) { m_PlayerTension += add; }
	void SetPlayerTension(float tension) { m_PlayerTension = tension; }
	float GetPlayerTension() { return m_PlayerTension; }
	void ChangeSpawnAI(CSpawnAIStateBase* next);

private:
	CZombie* m_Zombies[ZOMBIE_NUM_MAX];
	CField*				m_Field;
	CSpawnAIStateBase*	m_SpawnAI;
	float				m_PlayerTension;
	int					m_CountFrame;
	int					m_ZombieSpawnRate;
};


#endif // !_ENEMY_MANAGER_H_


