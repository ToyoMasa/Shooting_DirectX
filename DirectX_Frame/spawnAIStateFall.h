//======================================================================
//	�G�l�~�[����AI�ْ��x���~���[spawnAIStateFall.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_FALL_H_
#define _SPAWN_AI_STATE_FALL_H_

#include "spawnAIStateBase.h"

class CSpawnAIStateFall : public CSpawnAIStateBase
{
public:
	CSpawnAIStateFall(CEnemyManager* manager) : CSpawnAIStateBase(manager)
	{
		manager->SetSpawnAIState(SPAWN_AI_FALL);
	}
	~CSpawnAIStateFall() {}

	void Update()override;
};

#endif // !_SPAWN_AI_STATE_FALL_H_