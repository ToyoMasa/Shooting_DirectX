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
#include "sceneShadow.h"
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

static const float WALK_HEAT = 0.4f / 60.0f;

void CPlayerPatternNormal::Init(CPlayer* player)
{
	player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
	player->SetADS(false);
	player->SetWeaponADS(false);
}

void CPlayerPatternNormal::Update(CPlayer* player)
{
	// アニメーションの整合性を取る
	player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);

	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	CController* Controller;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// コントローラーの取得
	Controller = CManager::GetController();

	float moveX = 0.0f, moveZ = 0.0f;

	moveX = Controller->GetStickLX();
	moveZ = Controller->GetStickLY();

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

	// 歩いている間緊張度上昇
	if (moveX != 0.0f || moveZ != 0.0f)
	{
		CModeGame::GetEnemyManager()->AddPlayerTension(WALK_HEAT);
	}

	player->Move(dir.x, dir.y);

	// ADS
	if (inputMouse->GetRightPress())
	{
		player->ChangePattern(new CPlayerPatternADS());
		return;
	}

	if (inputMouse->GetLeftRelease())
	{
		player->TriggerRelease();
	}

	// 攻撃
	if (inputMouse->GetLeftPress())
	{
		if (player->GetUsingWeapon()->GetAmmo() <= 0)
		{
			player->GetUsingWeapon()->ReleaseTrigger();
			player->ChangePattern(new CPlayerPatternReload());
			return;
		}
		else
		{
			player->Shoot();
		}
	}

	// ダッシュ
	else if (inputKeyboard->GetKeyPress(DIK_LSHIFT) && moveZ > 0.8f)
	{
		player->ChangePattern(new CPlayerPatternDash());
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

	// 回転
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// 武器チェンジ
	if (inputKeyboard->GetKeyTrigger(DIK_X) || mouseZ != 0)
	{
		player->ChangePattern(new CPlayerPatternWeaponChange());
		return;
	}
}
