//======================================================================
//	�v���C���[ADS�p�^�[�� [playerPatternADS.cpp]�@�i2018/11/10�j
//	Author : �L�� ���r
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
#include "playerPatternReload.h"
#include "playerPatternADS.h"
#include "enemyManager.h"

static const float ADS_CAMERA_MAG = 0.65f;

void CPlayerPatternADS::Init(CPlayer* player)
{
	m_Player = player;
	m_Player->GetModel()->StopMontage();
	m_Player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

	// �Ə��̕\����x�������邽�߂̃J�E���g
	m_Time = timeGetTime();

	m_Player->SetADS(true);
}

void CPlayerPatternADS::Uninit()
{
	m_Player->SetADS(false);
	m_Player->SetWeaponADS(false);
}

void CPlayerPatternADS::Update()
{
	// �A�j���[�V�����̐����������
	m_Player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

	CInputKeyboard* inputKeyboard;
	CInputMouse* inputMouse;
	CController* controller;
	float mouseX, mouseY, mouseZ;

	// �L�[�{�[�h�擾
	inputKeyboard = CManager::GetInputKeyboard();

	// �}�E�X�擾
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	// �R���g���[���[�̎擾
	controller = CManager::GetController();

	// ADS����
	if (!inputMouse->GetRightPress() && 
		!controller->ButtonPress(XINPUT_GAMEPAD_LEFT_SHOULDER) &&
		!controller->LeftPress())
	{
		m_Player->ChangePattern(new CPlayerPatternNormal());
		return;
	}

	// ADS������������Ə��̕\��
	DWORD currentTime = timeGetTime();
	if (currentTime - m_Time < 1000 / 5.0f)
	{
		return;
	}
	m_Player->SetWeaponADS(true);
}

void CPlayerPatternADS::Move(D3DXVECTOR2 move)
{
	// �����Ă���ԋْ��x�㏸
	if (move.x != 0.0f || move.y != 0.0f)
	{
		CModeGame::GetEnemyManager()->AddHeat(WALK_HEAT);
	}

	D3DXVECTOR2 dir = move;
	D3DXVec2Normalize(&dir, &dir);
	dir *= PLAYER_ADS_MOVE_SPEED;

	m_Player->Move(dir.x, dir.y);
}

void CPlayerPatternADS::Rotate(D3DXVECTOR2 rot)
{
	CController *controller;

	// �R���g���[���[�̎擾
	controller = CManager::GetController();

	if (controller->GetIsAble())
	{
		m_Player->Rotate(rot.x * ADS_CAMERA_MAG, rot.y * ADS_CAMERA_MAG);
	}
	else
	{
		m_Player->Rotate(rot.x, rot.y);
	}
}

void CPlayerPatternADS::Shoot()
{
	if (m_Player->GetUsingWeapon()->GetAmmo() <= 0)
	{
		m_Player->GetUsingWeapon()->ReleaseTrigger();
		m_Player->ChangePattern(new CPlayerPatternReload());
	}
	else
	{
		m_Player->SetWeaponADS(true);
		m_Player->Shoot();
	}
}

void CPlayerPatternADS::Reload()
{
	if (m_Player->GetUsingWeapon()->GetAmmo() < m_Player->GetUsingWeapon()->GetMaxAmmo())
	{
		m_Player->ChangePattern(new CPlayerPatternReload());
	}
}
