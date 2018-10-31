//======================================================================
//	敵パターンヘッダ[enemyPatternBase.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_BASE_H_
#define _ENEMY_PATTERN_BASE_H_

class CEnemy;

class CEnemyPatternBase
{
public:
	CEnemyPatternBase() {}
	~CEnemyPatternBase() {}

	virtual void Init(CEnemy* enemy) = 0;
	virtual void Update(CEnemy* enemy) = 0;
};

#endif // !_ENEMY_PATTERN_BASE_H_

