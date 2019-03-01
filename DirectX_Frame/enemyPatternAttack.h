//======================================================================
//	�G�U���p�^�[���w�b�_[enemyPatternAttack.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_ATTACK_H_
#define _ENEMY_PATTERN_ATTACK_H_

#include "enemyPatternBase.h"

//*********************************************************************
//	�G���U�����Ă���Ԃ̏���
//*********************************************************************
class CEnemyPatternAttack : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_ATTACK_H_
