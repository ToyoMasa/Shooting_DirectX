//======================================================================
//	プレイヤーアイドルパターン [playerPatternIdle.cpp]　（2019/2/15）
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

void CPlayerPatternBase::Move(D3DXVECTOR2 move)
{

}

void CPlayerPatternBase::Rotate(D3DXVECTOR2 rot)
{

}

void CPlayerPatternBase::Dash(float moveZ)
{

}

void CPlayerPatternBase::ADS()
{

}

void CPlayerPatternBase::Shoot()
{

}

void CPlayerPatternBase::Reload()
{

}

void CPlayerPatternBase::ChangeWeapon()
{

}
