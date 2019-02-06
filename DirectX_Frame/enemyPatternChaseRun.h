//======================================================================
//	敵追跡(走り)パターンヘッダ[enemyPatternChaseRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_RUN_H_
#define _ENEMY_PATTERN_CHASE_RUN_H_

#include "enemyPatternBase.h"

class CEnemyPatternChaseRun : public CEnemyPatternBase
{
public:
	CEnemyPatternChaseRun() {}
	~CEnemyPatternChaseRun() {}

	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_RUN_H_
