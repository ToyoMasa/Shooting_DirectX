//======================================================================
//	�v���C���[ADS�p�^�[�� [playerPatternADS.cpp]�@�i2018/11/10�j
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
#include "playerPatternDash.h"

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

	// �O���ֈړ����Ă��Ȃ���Βʏ��Ԃ�
	if (moveZ <= 0.8f)
	{
		player->ChangePattern(new CPlayerPatternIdle());
	}

	D3DXVECTOR2 dir = D3DXVECTOR2(moveX, moveZ);
	D3DXVec2Normalize(&dir, &dir);
	dir.x *= PLAYER_DASH_MOVE_SPEED_X;
	dir.y *= PLAYER_DASH_MOVE_SPEED_Z;

	player->Move(dir.x, dir.y);

	// ��]
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// ADS
	if (inputMouse->GetRightPress())
	{
		player->ChangePattern(new CPlayerPatternADS());
		return;
	}

	// �_�b�V��
	if (inputKeyboard->GetKeyTrigger(DIK_LSHIFT))
	{
		player->ChangePattern(new CPlayerPatternIdle());
		return;
	}

	// �W�����v
	if (inputKeyboard->GetKeyRelease(DIK_SPACE))
	{
		player->ChangePattern(new CPlayerPatternJump(dir));
		return;

	}

	// �U��������_�b�V����Ԃ�����
	if (inputMouse->GetLeftPress())
	{
		player->ChangePattern(new CPlayerPatternIdle());
		return;
	}
	if (inputMouse->GetLeftRelease())
	{
		player->TriggerRelease();
	}
}