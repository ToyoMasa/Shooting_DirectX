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
#include "playerPatternNormal.h"
#include "playerPatternJump.h"
#include "playerPatternADS.h"
#include "playerPatternDash.h"
#include "playerPatternReload.h"
#include "playerPatternWeaponChange.h"

void CPlayerPatternNormal::Init(CPlayer* player)
{
	player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.2f);
	player->SetADS(false);
	player->SetWeaponADS(false);
}

void CPlayerPatternNormal::Update(CPlayer* player)
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

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

	D3DXVECTOR2 dir = D3DXVECTOR2(moveX, moveZ);
	D3DXVec2Normalize(&dir, &dir);

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

	// �U��
	if (inputMouse->GetLeftPress())
	{
		player->Shoot();
	}
	// �W�����v
	//else if (inputKeyboard->GetKeyRelease(DIK_SPACE))
	//{
	//	player->ChangePattern(new CPlayerPatternJump(dir));

	//}
	// �_�b�V��
	else if (inputKeyboard->GetKeyTrigger(DIK_LSHIFT))
	{
		player->ChangePattern(new CPlayerPatternDash());
	}

	// �����[�h
	if (inputKeyboard->GetKeyTrigger(DIK_R))
	{
		if (player->GetUsingWeapon()->GetAmmo() < player->GetUsingWeapon()->GetMaxAmmo())
		{
			player->ChangePattern(new CPlayerPatternReload());
		}
	}

	// ��]
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// ����`�F���W
	if (inputKeyboard->GetKeyTrigger(DIK_X) || mouseZ != 0)
	{
		player->ChangePattern(new CPlayerPatternWeaponChange());
	}
}
