//======================================================================
//	プレイヤーアイドルパターン [playerPatternIdle.cpp]　（2018/10/8）
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
#include "playerPatternIdle.h"
#include "playerPatternJump.h"
#include "playerPatternADS.h"
#include "playerPatternDash.h"
#include "playerPatternReload.h"

void CPlayerPatternIdle::Init(CPlayer* player)
{
	player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
	player->SetADS(false);
	player->SetWeaponADS(false);
}

void CPlayerPatternIdle::Update(CPlayer* player)
{
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

	D3DXVECTOR2 dir = D3DXVECTOR2(moveX, moveZ);
	D3DXVec2Normalize(&dir, &dir);

	player->Move(dir.x, dir.y);

	// ADS
	if (inputMouse->GetRightPress())
	{
		player->ChangePattern(new CPlayerPatternADS());
		return;
	}

	// 攻撃
	if (inputMouse->GetLeftPress())
	{
		player->Shoot();
	}
	if (inputMouse->GetLeftRelease())
	{
		player->TriggerRelease();
	}

	// ジャンプ
	if (inputKeyboard->GetKeyRelease(DIK_SPACE))
	{
		player->ChangePattern(new CPlayerPatternJump(dir));

	}

	// ダッシュ
	if (inputKeyboard->GetKeyTrigger(DIK_LSHIFT))
	{
		player->ChangePattern(new CPlayerPatternDash());
	}

	// リロード
	if (inputKeyboard->GetKeyTrigger(DIK_R))
	{
		if (player->GetUsingWeapon()->GetAmmo() < player->GetUsingWeapon()->GetMaxAmmo())
		{
			player->ChangePattern(new CPlayerPatternReload());
		}
	}

	// 回転
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// 武器チェンジ
	if (inputKeyboard->GetKeyTrigger(DIK_1))
	{
		player->ChangeWeapon(0);
	}

	if (inputKeyboard->GetKeyTrigger(DIK_2))
	{
		player->ChangeWeapon(1);
	}
}
