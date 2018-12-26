//======================================================================
//	エネミー生成AIベース[spawnAIStateBase.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_BASE_H_
#define _SPAWN_AI_STATE_BASE_H_

#include "enemyManager.h"

enum ENEMY_SPAWN_AI_STATE
{
	SPAWN_AI_NONE,
	SPAWN_AI_RISE,
	SPAWN_AI_MAX,
	SPAWN_AI_FALL,
	SPAWN_AI_STOP,
	SPAWN_AI_PAUSE
};

class CSpawnAIStateBase
{
public:
	CSpawnAIStateBase(CEnemyManager* manager) : m_EnemyManager(manager) {}
	~CSpawnAIStateBase() {}

	virtual void Update() = 0;

protected:
	CEnemyManager*	m_EnemyManager;
};

#endif // !_SPAWN_AI_STATE_BASE_H_
