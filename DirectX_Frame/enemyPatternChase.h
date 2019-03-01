//======================================================================
//	�G�ǐՃp�^�[���w�b�_[enemyPatternChase.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_H_
#define _ENEMY_PATTERN_CHASE_H_

#include "enemyPatternBase.h"

//*********************************************************
//	�v���C���[�Ɍ������Ē����ړ�����
//*********************************************************
class CEnemyPatternChase : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_H_
