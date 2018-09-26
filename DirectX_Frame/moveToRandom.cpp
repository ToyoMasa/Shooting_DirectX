//======================================================================
//	ランダムな地点へ移動　（2018/07/18）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "input.h"
#include "field.h"
#include "action.h"
#include "root.h"
#include "sceneSkinMesh.h"
#include "EnemyAnim.h"

void CActionMoveToRandom::Init(float min, float max, float speed)
{
	m_Speed = speed;
	m_MinMove = min;
	m_MaxMove = max;

	float move;
	move = m_MinMove + rand() % (int)((m_MaxMove - m_MinMove) * 10000) / 10000.0f;

	D3DXVECTOR3 dir = D3DXVECTOR3(1.0f - rand() % 20000 / 10000.0f, 0.0f, 1.0f - rand() % 20000 / 10000.0f);
	D3DXVec3Normalize(&dir, &dir);

	m_TargetPos = m_Character->GetPos() + dir * move;
}

void CActionMoveToRandom::Update()
{
	D3DXVECTOR3 newPos = m_Character->GetPos();
	newPos.y = 0;
	D3DXVECTOR3 vec = m_TargetPos - newPos;

	m_Character->GetModel()->ChangeAnim(ENEMY_WALKING, 0.3f);

	if (D3DXVec3Length(&vec) > m_Speed)
	{
		D3DXVec3Normalize(&vec, &vec);
		newPos += m_Speed * vec;

		// 壁との当たり判定
		newPos = m_Character->HitWall(newPos);

		m_Character->SetPos(newPos);

		D3DXVec3Normalize(&vec, &vec);
		m_Character->Rotate(vec);
	}
	else
	{
		newPos = m_TargetPos;

		// 壁との当たり判定
		newPos = m_Character->HitWall(newPos);

		m_Character->SetPos(newPos);

		D3DXVec3Normalize(&vec, &vec);
		m_Character->Rotate(vec);

		// 次の目的地を設定
/*
		float move;
		move = m_MinMove + rand() % (int)((m_MaxMove - m_MinMove) * 10000) / 10000.0f;

		D3DXVECTOR3 dir = D3DXVECTOR3(1.0f - rand() % 20000 / 10000.0f, 0.0f, 1.0f - rand() % 20000 / 10000.0f);
		D3DXVec3Normalize(&dir, &dir);

		m_TargetPos = m_Character->GetPos() + dir * move;*/
		CEnemy* enemy = (CEnemy*)m_Character;
		enemy->SetAction(CActionWaitTime::Create(enemy, 120, CActionMoveToRandom::Create(enemy, m_MinMove, m_MaxMove, m_Speed)));
	}
}

CActionMoveToRandom* CActionMoveToRandom::Create(CCharacter* chara, float min, float max, float speed)
{
	CActionMoveToRandom* moveToRandom = new CActionMoveToRandom(chara);
	moveToRandom->Init(min, max, speed);

	return moveToRandom;
}
