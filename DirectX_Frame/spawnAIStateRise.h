//======================================================================
//	�G�l�~�[����AI�ْ��x�㏸���[spawnAIStateRise.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_RISE_H_
#define _SPAWN_AI_STATE_RISE_H_

#include "spawnAIStateBase.h"

class CSpawnAIStateRise : public CSpawnAIStateBase
{
public:
	CSpawnAIStateRise(CEnemyManager* manager) : CSpawnAIStateBase(manager) {}
	~CSpawnAIStateRise() {}

	void Update()override;
};

#endif // !_SPAWN_AI_STATE_RISE_H_
