//======================================================================
//	敵追跡パターンヘッダ[enemyPatternChase.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_H_
#define _ENEMY_PATTERN_CHASE_H_

#include "enemyPatternBase.h"

//*********************************************************
//	プレイヤーに向かって直線移動する
//*********************************************************
class CEnemyPatternChase : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_H_
