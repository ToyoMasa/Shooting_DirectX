//======================================================================
//	エネミー生成AI緊張度上昇状態[spawnAIStateRise.h]
//
//======================================================================
#ifndef _SPAWN_AI_STATE_RISE_H_
#define _SPAWN_AI_STATE_RISE_H_

#include "spawnAIStateBase.h"

//*********************************************************
//	敵を一定時間置きに生成
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
