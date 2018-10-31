//======================================================================
//	敵攻撃パターンヘッダ[enemyPatternAttack.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_ATTACK_H_
#define _ENEMY_PATTERN_ATTACK_H_

#include "enemyPatternBase.h"

class CEnemyPatternAttack : public CEnemyPatternBase
{
public:
	CEnemyPatternAttack() {}
	~CEnemyPatternAttack() {}

	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_ATTACK_H_
