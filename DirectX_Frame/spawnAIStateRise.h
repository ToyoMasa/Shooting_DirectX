//======================================================================
//	�G�l�~�[����AI�ْ��x�㏸���[spawnAIStateRise.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_RISE_H_
#define _SPAWN_AI_STATE_RISE_H_

#include "spawnAIStateBase.h"

//*********************************************************
//	�G����莞�Ԓu���ɐ���
//*********************************************************
class CSpawnAIStateRise : public CSpawnAIStateBase
{
public:
	CSpawnAIStateRise(CEnemyManager* manager) : CSpawnAIStateBase(manager)
	{
		m_Count = 0;
		manager->SetSpawnAIState(SPAWN_AI_RISE);
	}

	void Update()override;
private:
	int m_Count;
};

#endif // !_SPAWN_AI_STATE_RISE_H_
