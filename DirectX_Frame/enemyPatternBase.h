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
	enum ENEMY_PATTERN_TYPE
	{
		ENEMY_PATTERN_NONE,
		ENEMY_PATTERN_ATTACK,
		ENEMY_PATTERN_ATTACK_RUN,
		ENEMY_PATTERN_CHASE,
		ENEMY_PATTERN_CHASE_RUN,
		ENEMY_PATTERN_DEATH,
		ENEMY_PATTERN_WAYPOINTS,
		ENEMY_PATTERN_WAYPOINTS_RUN
	};

	CEnemyPatternBase() {}
	~CEnemyPatternBase() {}

	virtual void Init(CEnemy* enemy) = 0;
	virtual void Update(CEnemy* enemy) = 0;
};

#endif // !_ENEMY_PATTERN_BASE_H_

