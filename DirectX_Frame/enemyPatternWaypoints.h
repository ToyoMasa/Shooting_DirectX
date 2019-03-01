//======================================================================
//	敵ウェイポイントによる移動パターンヘッダ[enemyPatternWaypoints.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_WAYPOINTS_H_
#define _ENEMY_PATTERN_WAYPOINTS_H_

#include "enemyPatternBase.h"

//*********************************************************
//	ウェイポイント間の移動処理
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
