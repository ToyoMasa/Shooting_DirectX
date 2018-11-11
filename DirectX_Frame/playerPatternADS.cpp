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
#include "playerPatternIdle.h"
#include "playerPatternJump.h"
#include "playerPatternADS.h"

void CPlayerPatternADS::Init(CPlayer* player)
{
	player->GetModel()->StopMontage();
	player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);
	m_Time = timeGetTime();
}

void CPlayerPatternADS::Uninit(CPlayer* player)
{
	player->SetADS(false);
}

void CPlayerPatternADS::Update(CPlayer* player)
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

	if (timeGetTime() - m_Time < (DWORD)(1000 / 20))
	{
		return;
	}

	player->SetADS(true);

	{
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
			moveX = -0.5f;
		}
		if (inputKeyboard->GetKeyPress(DIK_D))
		{
			moveX = 0.5f;
		}
		if (inputKeyboard->GetKeyPress(DIK_W))
		{
			moveZ = 0.5f;
		}
		if (inputKeyboard->GetKeyPress(DIK_S))
		{
			moveZ = -0.5f;
		}

		player->Move(moveX, moveZ);

		// ADS
		if (!inputMouse->GetRightPress())
		{
			player->ChangePattern(new CPlayerPatternIdle());
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
			player->ChangePattern(new CPlayerPatternJump());

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
}
