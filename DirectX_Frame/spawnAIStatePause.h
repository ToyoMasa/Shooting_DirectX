//======================================================================
//	エネミー生成AI緊張度休止状態[spawnAIStatePause.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_PAUSE_H_
#define _SPAWN_AI_STATE_PAUSE_H_

#include "spawnAIStateBase.h"

class CSpawnAIStatePause : public CSpawnAIStateBase
{
public:
	CSpawnAIStatePause(CEnemyManager* manager) : CSpawnAIStateBase(manager)
	{
		m_Count = 0;
		manager->SetSpawnAIState(SPAWN_AI_PAUSE);
	}
	~CSpawnAIStatePause() {}

	void Update()override;
private:
	int m_Count;
};

#endif // !_SPAWN_AI_STATE_PAUSE_H_
