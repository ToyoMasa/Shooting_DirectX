//======================================================================
//	敵攻撃(走り)パターンヘッダ[enemyPatternAttackRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_ATTACK_RUN_H_
#define _ENEMY_PATTERN_ATTACK_RUN_H_

#include "enemyPatternBase.h"

class CEnemyPatternAttackRun : public CEnemyPatternBase
{
public:
	CEnemyPatternAttackRun() {}
	~CEnemyPatternAttackRun() {}

	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_ATTACK_RUN_H_
