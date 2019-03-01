//======================================================================
//	ƒGƒlƒ~[¶¬AI‹Ù’£“x‹xŽ~ó‘Ô[spawnAIStatePause.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_PAUSE_H_
#define _SPAWN_AI_STATE_PAUSE_H_

#include "spawnAIStateBase.h"

//*********************************************************
//	“G‚Ì¶¬’†’f
//*********************************************************
class CSpawnAIStatePause : public CSpawnAIStateBase
{
public:
	CSpawnAIStatePause(CEnemyManager* manager) : CSpawnAIStateBase(manager)
	{
		m_Count = 0;
		manager->SetSpawnAIState(SPAWN_AI_PAUSE);
		CEnemy::StopHorde();
	}

	void Update()override;
private:
	int m_Count;
};

#endif // !_SPAWN_AI_STATE_PAUSE_H_
