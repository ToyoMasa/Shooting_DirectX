//======================================================================
//	プレイヤーADSパターン [playerPatternADS.cpp]　（2018/11/10）
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
#include "PlayerAnim.h"
#include "wall.h"
#include "bullet.h"
#include "weapon.h"
#include "rifle.h"
#include "shotgun.h"
#include "playerPatternNormal.h"
#include "playerPatternJump.h"
#include "playerPatternADS.h"
#include "playerPatternDash.h"
#include "playerPatternReload.h"
#include "playerPatternWeaponChange.h"
#include "enemyManager.h"

static const float DASH_HEAT = 1.0f / 60.0f;

void CPlayerPatternDash::Init(CPlayer* player)
{
	m_Player = player;
	m_Player->GetModel()->ChangeAnim(PLAYER_SPRINT, 0.3f);
}

void CPlayerPatternDash::Update()
{
	// アニメーションの整合性を取る
	m_Player->GetModel()->ChangeAnim(PLAYER_SPRINT, 0.3f);
}

void CPlayerPatternDash::Move(D3DXVECTOR2 move)
{
	// 歩いている間緊張度上昇
	if (move.x != 0.0f || move.y != 0.0f)
	{
		CModeGame::GetEnemyManager()->AddPlayerTension(WALK_HEAT);
	}

	// 前方へ移動していなければ通常状態へ
	if (move.y <= 0.4f)
	{
		m_Player->ChangePattern(new CPlayerPatternNormal());
		return;
	}

	// 走っている間緊張度上昇
	CModeGame::GetEnemyManager()->AddPlayerTension(DASH_HEAT);

	D3DXVECTOR2 dir = move;
	D3DXVec2Normalize(&dir, &dir);

	dir.x *= PLAYER_DASH_MOVE_SPEED_X;
	dir.y *= PLAYER_DASH_MOVE_SPEED_Z;

	m_Player->Move(dir.x, dir.y);

}

void CPlayerPatternDash::Rotate(D3DXVECTOR2 rot)
{
	m_Player->Rotate(rot.x, rot.y);
}

void CPlayerPatternDash::ADS()
{
	m_Player->ChangePattern(new CPlayerPatternADS());
}

void CPlayerPatternDash::Shoot()
{
	m_Player->ChangePattern(new CPlayerPatternNormal());
}

void CPlayerPatternDash::Reload()
{
	if (m_Player->GetUsingWeapon()->GetAmmo() < m_Player->GetUsingWeapon()->GetMaxAmmo())
	{
		m_Player->ChangePattern(new CPlayerPatternReload());
	}
}

void CPlayerPatternDash::ChangeWeapon()
{
	m_Player->ChangePattern(new CPlayerPatternWeaponChange());
}
