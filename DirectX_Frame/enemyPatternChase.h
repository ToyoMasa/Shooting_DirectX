//======================================================================
//	敵追跡パターンヘッダ[enemyPatternChase.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_H_
#define _ENEMY_PATTERN_CHASE_H_

#include "enemyPatternBase.h"

class CEnemyPatternChase : public CEnemyPatternBase
{
public:
	CEnemyPatternChase() {}
	~CEnemyPatternChase() {}

	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_H_
