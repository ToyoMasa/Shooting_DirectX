//======================================================================
//	�G���S�p�^�[���w�b�_[enemyPatternIdle.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_DEATH_H_
#define _ENEMY_PATTERN_DEATH_H_

#include "enemyPatternBase.h"

//*********************************************************
//	�G���S���̏���
//*********************************************************
class CEnemyPatternDeath : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_DEATH_H_
