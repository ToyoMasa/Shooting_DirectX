//======================================================================
//	敵追跡(走り)パターンヘッダ[enemyPatternChaseRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_RUN_H_
#define _ENEMY_PATTERN_CHASE_RUN_H_

#include "enemyPatternBase.h"

//*********************************************************
//	プレイヤーに向かって、走って直線移動する
//*********************************************************
class CEnemyPatternChaseRun : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_RUN_H_
