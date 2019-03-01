//======================================================================
//	�G�ǐ�(����)�p�^�[���w�b�_[enemyPatternChaseRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_CHASE_RUN_H_
#define _ENEMY_PATTERN_CHASE_RUN_H_

#include "enemyPatternBase.h"

//*********************************************************
//	�v���C���[�Ɍ������āA�����Ē����ړ�����
//*********************************************************
class CEnemyPatternChaseRun : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_CHASE_RUN_H_
