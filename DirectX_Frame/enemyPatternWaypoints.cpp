//======================================================================
//	敵追跡パターン [enemyPatternWaypoints.cpp]　（2018/11/19）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "sceneShadow.h"
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "wall.h"
#include "bullet.h"
#include "EnemyAnim.h"
#include "waypoint.h"
#include "enemyPatternWaypoints.h"
#include "enemyPatternChase.h"
#include "enemyPatternAttack.h"

static float MOVE_SPEED = 0.015f;

void CEnemyPatternWaypoints::Init(CEnemy* enemy)
{
	enemy->GetModel()->ChangeAnim(ENEMY_WALKING, 0.3f);
	enemy->GetModel()->SetAnimPlaySpeed(1.2f);

	// 一番近いウェイポイントを検索
	m_NowPoint = CWayPoint::SearchShortestPoint(enemy->GetPos());
	int playerPoint = CWayPoint::SearchShortestPoint(CModeGame::GetPlayer()->GetPos());

	m_TargetPoint = CWayPoint::GetNextPoint(m_NowPoint, playerPoint);
	m_TargetPos = CWayPoint::GetWayPointPos(m_TargetPoint);
}

void CEnemyPatternWaypoints::Update(CEnemy* enemy)
{
	D3DXVECTOR3 newPos = enemy->GetPos();
	newPos.y = 0;
	D3DXVECTOR3 targetPos = CModeGame::GetPlayer()->GetPos();
	targetPos.y = 0;
	D3DXVECTOR3 vec = targetPos - newPos;

	if (D3DXVec3Length(&vec) < 5.0f)
	{
		// プレイヤーとの距離が近ければ追尾
		enemy->ChangePattern(new CEnemyPatternChase());
	}
	else if (m_TargetPoint == -1)
	{
		// プレイヤーと同じポイントにいれば
		enemy->ChangePattern(new CEnemyPatternChase());
	}
	else
	{
		// 目的地を次のウェイポイントに更新
		targetPos = m_TargetPos;
		vec = targetPos - newPos;

		D3DXVec3Normalize(&vec, &vec);

		newPos += MOVE_SPEED * vec;

		if (D3DXVec3Length(&(newPos - m_TargetPos)) < 1.0f)
		{
			// 目的地に到達したら次の目的地を設定
			m_NowPoint = m_TargetPoint;
			int playerPoint = CWayPoint::SearchShortestPoint(CModeGame::GetPlayer()->GetPos());

			m_TargetPoint = CWayPoint::GetNextPoint(m_NowPoint, playerPoint);
			m_TargetPos = CWayPoint::GetWayPointPos(m_TargetPoint);
		}
	}

	D3DXVec3Normalize(&vec, &vec);
	enemy->Rotate(vec);
	enemy->Move(newPos);
}

