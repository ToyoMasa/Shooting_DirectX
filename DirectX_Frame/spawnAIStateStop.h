//======================================================================
//	エネミー生成AI停止状態[spawnAIStateStop.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_STOP_H_
#define _SPAWN_AI_STATE_STOP_H_

#include "spawnAIStateBase.h"

class CSpawnAIStateStop : public CSpawnAIStateBase
{
public:
	CSpawnAIStateStop(CEnemyManager* manager) : CSpawnAIStateBase(manager) 
	{
		manager->SetSpawnAIState(SPAWN_AI_STOP);
	}
	~CSpawnAIStateStop() {}

	void Update()override {};
};

#endif // !_SPAWN_AI_STATE_STOP_H_
