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

void CPlayerPatternDash::Init(CPlayer* player)
{
	player->GetModel()->StopMontage();
	player->GetModel()->ChangeAnim(PLAYER_SPRINT, 0.3f);

}

void CPlayerPatternDash::Uninit(CPlayer* player)
{

}

void CPlayerPatternDash::Update(CPlayer* player)
{
	// アニメーションの整合性を取る
	player->GetModel()->ChangeAnim(PLAYER_SPRINT, 0.3f);

	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	player->GetModel()->ChangeAnim(PLAYER_SPRINT, 0.3f);

	float moveX = 0.0f, moveZ = 0.0f;
	if (inputKeyboard->GetKeyPress(DIK_A))
	{
		moveX = -1.0f;
	}
	if (inputKeyboard->GetKeyPress(DIK_D))
	{
		moveX = 1.0f;
	}
	if (inputKeyboard->GetKeyPress(DIK_W))
	{
		moveZ = 1.0f;
	}
	if (inputKeyboard->GetKeyPress(DIK_S))
	{
		moveZ = -1.0f;
	}

	// 前方へ移動していなければ通常状態へ
	if (moveZ <= 0.8f)
	{
		player->ChangePattern(new CPlayerPatternNormal());
	}

	D3DXVECTOR2 dir = D3DXVECTOR2(moveX, moveZ);
	D3DXVec2Normalize(&dir, &dir);
	dir.x *= PLAYER_DASH_MOVE_SPEED_X;
	dir.y *= PLAYER_DASH_MOVE_SPEED_Z;

	player->Move(dir.x, dir.y);

	// 回転
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// ADS
	if (inputMouse->GetRightPress())
	{
		player->ChangePattern(new CPlayerPatternADS());
		return;
	}

	// リロード
	if (inputKeyboard->GetKeyTrigger(DIK_R))
	{
		if (player->GetUsingWeapon()->GetAmmo() < player->GetUsingWeapon()->GetMaxAmmo())
		{
			player->ChangePattern(new CPlayerPatternReload());
			return;
		}
	}

	// 武器チェンジ
	if (inputKeyboard->GetKeyTrigger(DIK_X))
	{
		player->ChangePattern(new CPlayerPatternWeaponChange());
		return;
	}

	// 攻撃したらダッシュ状態を解除
	if (inputMouse->GetLeftPress())
	{
		player->ChangePattern(new CPlayerPatternNormal());
		return;
	}

	if (inputMouse->GetLeftRelease())
	{
		player->TriggerRelease();
	}
}
