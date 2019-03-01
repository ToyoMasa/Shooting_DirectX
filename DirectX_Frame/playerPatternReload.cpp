//======================================================================
//	�v���C���[�����[�h�p�^�[�� [playerPatternReload.cpp]�@�i2018/11/24�j
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
#include "texture.h"
#include "billboard.h"
#include "character.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "enemy.h"
#include "game.h"
#include "PlayerAnim.h"
#include "bullet.h"
#include "weapon.h"
#include "rifle.h"
#include "shotgun.h"
#include "playerPatternNormal.h"
#include "playerPatternADS.h"
#include "playerPatternDash.h"
#include "playerPatternReload.h"
#include "enemyManager.h"

void CPlayerPatternReload::Init(CPlayer* player)
{
	m_Player = player;
	m_Player->GetModel()->ChangeAnim(PLAYER_NONE, 0.2f);
	m_Player->SetADS(false);
	m_Player->SetWeaponADS(false);
	m_CountFrame = 0;
	m_Player->PlayReload();
}

void CPlayerPatternReload::Update()
{
	// �t���[���ɉ���������
	if (m_CountFrame < 12)
	{
		// �A�j���[�V�����̐����������
		m_Player->GetModel()->ChangeAnim(PLAYER_NONE, 0.0f);
	}
	else if (m_CountFrame < 60)
	{
		// �A�j���[�V�����̐����������
		m_Player->GetModel()->ChangeAnim(PLAYER_NONE, 0.0f);
	}
	else if (m_CountFrame == 60)
	{
		m_Player->Reload();
		m_Player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.4f);
	}
	else if (m_CountFrame < 90)
	{
		// �A�j���[�V�����̐����������
		m_Player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.4f);
	}
	else if (m_CountFrame == 90)
	{
		m_Player->ChangePattern(new CPlayerPatternNormal());
		return;
	}

	m_CountFrame++;
}

void CPlayerPatternReload::Move(D3DXVECTOR2 move)
{
	D3DXVECTOR2 dir = move;
	D3DXVec2Normalize(&dir, &dir);

	// �����Ă���ԋْ��x�㏸
	if (move.x != 0.0f || move.y != 0.0f)
	{
		CModeGame::GetEnemyManager()->AddHeat(WALK_HEAT);
	}

	m_Player->Move(dir.x, dir.y);
}

void CPlayerPatternReload::Rotate(D3DXVECTOR2 rot)
{
	m_Player->Rotate(rot.x, rot.y);
}
