//======================================================================
//	プレイヤージャンプパターン [playerPatternJump.cpp]　（2018/10/29）
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
#include "playerPatternJump.h"
#include "playerPatternAir.h"

static const float PLAYER_JUMP_MOVE = 0.18f;

void CPlayerPatternJump::Init(CPlayer* player)
{
	player->SetJumpPower(PLAYER_JUMP_POWER);
	player->SetUpValue(PLAYER_JUMP_MOVE);
}

void CPlayerPatternJump::Update(CPlayer* player)
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	{
		// キーボード取得
		inputKeyboard = CManager::GetInputKeyboard();

		// マウス取得
		inputMouse = CManager::GetInputMouse();
		mouseX = (float)inputMouse->GetAxisX();
		mouseY = (float)inputMouse->GetAxisY();
		mouseZ = (float)inputMouse->GetAxisZ();

		//float moveX = 0.0f, moveZ = 0.0f;
		//if (inputKeyboard->GetKeyPress(DIK_A))
		//{
		//	moveX = -PLAYER_AIR_MOVE_SPEED;
		//}
		//if (inputKeyboard->GetKeyPress(DIK_D))
		//{
		//	moveX = PLAYER_AIR_MOVE_SPEED;
		//}
		//if (inputKeyboard->GetKeyPress(DIK_W))
		//{
		//	moveZ = PLAYER_AIR_MOVE_SPEED;
		//}
		//if (inputKeyboard->GetKeyPress(DIK_S))
		//{
		//	moveZ = -PLAYER_AIR_MOVE_SPEED;
		//}

		m_MoveVec *= 0.99f;

		player->SetJumpPower(player->GetJumpPower() - player->GetUpValue());
		if (player->GetJumpPower() > 0)
		{
			player->MoveAir(m_MoveVec.x, player->GetUpValue(), m_MoveVec.y);
			player->SetUpValue(player->GetUpValue() * 0.92f);
		}
		else
		{
			player->ChangePattern(new CPlayerPatternAir(m_MoveVec));
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
