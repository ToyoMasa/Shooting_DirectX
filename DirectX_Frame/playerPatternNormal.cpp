//======================================================================
//	プレイヤーアイドルパターン [playerPatternIdle.cpp]　（2018/10/8）
//	Author : 豊村 昌俊
//======================================================================
#include <Windows.h>
#include <Xinput.h>
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
#include "controller.h"
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
#include "playerPatternWeaponChange.h"
#include "enemyManager.h"

void CPlayerPatternNormal::Init(CPlayer* player)
{
	m_Player = player;
	m_Player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
	m_Player->SetADS(false);
	m_Player->SetWeaponADS(false);
}

void CPlayerPatternNormal::Update()
{
	// アニメーションの整合性を取る
	m_Player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
}

void CPlayerPatternNormal::Move(D3DXVECTOR2 move)
{
	D3DXVECTOR2 dir = move;
	D3DXVec2Normalize(&dir, &dir);

	// 歩いている間緊張度上昇
	if (move.x != 0.0f || move.y != 0.0f)
	{
		CModeGame::GetEnemyManager()->AddHeat(WALK_HEAT);
	}

	m_Player->Move(dir.x, dir.y);
}

void CPlayerPatternNormal::Rotate(D3DXVECTOR2 rot)
{
	m_Player->Rotate(rot.x, rot.y);
}

void CPlayerPatternNormal::Dash(float moveZ)
{
	if (moveZ >= 0.8f)
	{
		m_Player->ChangePattern(new CPlayerPatternDash());
	}
}

void CPlayerPatternNormal::ADS()
{
	m_Player->ChangePattern(new CPlayerPatternADS());
}

void CPlayerPatternNormal::Shoot()
{
	if (m_Player->GetUsingWeapon()->GetAmmo() <= 0)
	{
		m_Player->GetUsingWeapon()->ReleaseTrigger();
		m_Player->ChangePattern(new CPlayerPatternReload());
	}
	else
	{
		m_Player->Shoot();
	}
}

void CPlayerPatternNormal::Reload()
{
	if (m_Player->GetUsingWeapon()->GetAmmo() < m_Player->GetUsingWeapon()->GetMaxAmmo())
	{
		m_Player->ChangePattern(new CPlayerPatternReload());
	}
}

void CPlayerPatternNormal::ChangeWeapon()
{
	m_Player->ChangePattern(new CPlayerPatternWeaponChange());
}
