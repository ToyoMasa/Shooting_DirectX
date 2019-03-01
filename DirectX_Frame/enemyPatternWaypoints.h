//======================================================================
//	�G�E�F�C�|�C���g�ɂ��ړ��p�^�[���w�b�_[enemyPatternWaypoints.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_WAYPOINTS_H_
#define _ENEMY_PATTERN_WAYPOINTS_H_

#include "enemyPatternBase.h"

//*********************************************************
//	�E�F�C�|�C���g�Ԃ̈ړ�����
//*********************************************************
class CEnemyPatternWaypoints : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;

private:
	int				m_NowPoint;
	int				m_TargetPoint;
	D3DXVECTOR3		m_TargetPos;
};

#endif // !_ENEMY_PATTERN_WAYPOINTS_H_
