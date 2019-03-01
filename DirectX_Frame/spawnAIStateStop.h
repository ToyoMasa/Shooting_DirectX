//======================================================================
//	�G�l�~�[����AI��~���[spawnAIStateStop.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_STOP_H_
#define _SPAWN_AI_STATE_STOP_H_

#include "spawnAIStateBase.h"

//*********************************************************
//	�G�̐������~
//*********************************************************
class CSpawnAIStateStop : public CSpawnAIStateBase
{
public:
	CSpawnAIStateStop(CEnemyManager* manager) : CSpawnAIStateBase(manager) 
	{
		manager->SetSpawnAIState(SPAWN_AI_STOP);
	}

	void Update()override {};
};

#endif // !_SPAWN_AI_STATE_STOP_H_
