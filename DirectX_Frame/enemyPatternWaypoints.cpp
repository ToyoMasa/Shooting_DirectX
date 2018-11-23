//======================================================================
//	�G�ǐՃp�^�[�� [enemyPatternWaypoints.cpp]�@�i2018/11/19�j
//	Author : �L�� ���r
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

	// ��ԋ߂��E�F�C�|�C���g������
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
		// �v���C���[�Ƃ̋������߂���Βǔ�
		enemy->ChangePattern(new CEnemyPatternChase());
	}
	else if (m_TargetPoint == -1)
	{
		// �v���C���[�Ɠ����|�C���g�ɂ����
		enemy->ChangePattern(new CEnemyPatternChase());
	}
	else
	{
		// �ړI�n�����̃E�F�C�|�C���g�ɍX�V
		targetPos = m_TargetPos;
		vec = targetPos - newPos;

		D3DXVec3Normalize(&vec, &vec);

		newPos += MOVE_SPEED * vec;

		if (D3DXVec3Length(&(newPos - m_TargetPos)) < 1.0f)
		{
			// �ړI�n�ɓ��B�����玟�̖ړI�n��ݒ�
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

