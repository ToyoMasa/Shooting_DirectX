//======================================================================
//	�G�E�F�C�|�C���g�ɂ��ړ��p�^�[���w�b�_[enemyPatternWaypointsRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_WAYPOINTS_RUN__H_
#define _ENEMY_PATTERN_WAYPOINTS_RUN__H_

#include "enemyPatternBase.h"

//*********************************************************
//	�����Ă���G�̃E�F�C�|�C���g�Ԃ̈ړ�����
//*********************************************************
class CEnemyPatternWaypointsRun : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;

private:
	int				m_NowPoint;
	int				m_TargetPoint;
	D3DXVECTOR3		m_TargetPos;
};

#endif // !_ENEMY_PATTERN_WAYPOINTS_RUN__H_
