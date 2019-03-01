//======================================================================
//	敵ウェイポイントによる移動パターンヘッダ[enemyPatternWaypointsRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_WAYPOINTS_RUN__H_
#define _ENEMY_PATTERN_WAYPOINTS_RUN__H_

#include "enemyPatternBase.h"

//*********************************************************
//	走っている敵のウェイポイント間の移動処理
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
