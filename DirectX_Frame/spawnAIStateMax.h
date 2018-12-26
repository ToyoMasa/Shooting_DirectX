//======================================================================
//	�G�l�~�[����AI�ْ��x�ő���[spawnAIStateMax.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_MAX_H_
#define _SPAWN_AI_STATE_MAX_H_

#include "spawnAIStateBase.h"

class CSpawnAIStateMax : public CSpawnAIStateBase
{
public:
	CSpawnAIStateMax(CEnemyManager* manager) : CSpawnAIStateBase(manager)
	{ 
		m_Count = 0; 
		manager->SetSpawnAIState(SPAWN_AI_MAX);
	}
	~CSpawnAIStateMax() {}

	void Update()override;
private:
	int m_Count;
};

#endif // !_SPAWN_AI_STATE_MAX_H_
