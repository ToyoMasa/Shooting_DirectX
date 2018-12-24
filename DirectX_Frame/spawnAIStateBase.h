//======================================================================
//	エネミー生成AIベース[spawnAIStateBase.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_BASE_H_
#define _SPAWN_AI_STATE_BASE_H_

#include "enemyManager.h"

class CSpawnAIStateBase
{
public:
	CSpawnAIStateBase(CEnemyManager* manager) : m_EnemyManager(manager) {}
	~CSpawnAIStateBase() {}

	virtual void Update() = 0;
protected:
	CEnemyManager* m_EnemyManager;
};

#endif // !_SPAWN_AI_STATE_BASE_H_
