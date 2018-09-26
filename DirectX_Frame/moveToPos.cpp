//======================================================================
//	指定地点へ移動　（2018/07/18）
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
#include "moveToPos.h"
#include "root.h"
#include "sceneSkinMesh.h"
#include "EnemyAnim.h"

void CActionMoveToPos::Init(int rootId, float speed)
{
	m_RootID = rootId;
	m_Speed = speed;
	m_TargetPos = CRoot::GetRootPos(m_RootID);
}

void CActionMoveToPos::Update()
{
	D3DXVECTOR3 newPos = m_Character->GetPos();
	newPos.y = 0;
	D3DXVECTOR3 vec = m_TargetPos - newPos;

	m_Character->GetModel()->ChangeAnim(ENEMY_WALKING, 0.3f);

	if (D3DXVec3Length(&vec) > m_Speed)
	{
		D3DXVec3Normalize(&vec, &vec);

		newPos += m_Speed * vec;

		m_Character->Rotate(newPos - m_Character->GetPos());
		m_Character->SetPos(newPos);
	}
	else
	{
		D3DXVec3Normalize(&vec, &vec);

		newPos = m_TargetPos;
		m_Character->SetPos(newPos);

		// 次の目的地を設定
		m_RootID = CRoot::GetNextRoot(m_RootID);
		m_TargetPos = CRoot::GetRootPos(m_RootID);
	}

	D3DXVec3Normalize(&vec, &vec);
}

CActionMoveToPos* CActionMoveToPos::Create(CCharacter* chara, int rootId, float speed)
{
	CActionMoveToPos* moveToPos = new CActionMoveToPos(chara);
	moveToPos->Init(rootId, speed);

	return moveToPos;
}
