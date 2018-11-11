//======================================================================
//	�v���C���[�A�C�h���p�^�[�� [playerPatternIdle.cpp]�@�i2018/10/8�j
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

void CPlayerPatternIdle::Init(CPlayer* player)
{
	player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
}

void CPlayerPatternIdle::Update(CPlayer* player)
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	{
		// �L�[�{�[�h�擾
		inputKeyboard = CManager::GetInputKeyboard();

		// �}�E�X�擾
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

		player->Move(moveX, moveZ);

		// ADS
		if (inputMouse->GetRightPress())
		{
			player->ChangePattern(new CPlayerPatternADS());
		}
		//player->ADS(inputMouse->GetRightPress());

		// �U��
		if (inputMouse->GetLeftPress())
		{
			player->Shoot();
		}
		if (inputMouse->GetLeftRelease())
		{
			player->TriggerRelease();
		}

		// �W�����v
		if (inputKeyboard->GetKeyRelease(DIK_SPACE))
		{
			player->ChangePattern(new CPlayerPatternJump());

		}

		// ��]
		player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

		// ����`�F���W
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
