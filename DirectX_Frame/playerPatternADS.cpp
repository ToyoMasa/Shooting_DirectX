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

void CPlayerPatternADS::Init(CPlayer* player)
{
	player->GetModel()->StopMontage();
	player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

	// 照準の表示を遅延させるためのカウント
	m_Time = timeGetTime();

	player->SetADS(true);
}

void CPlayerPatternADS::Uninit(CPlayer* player)
{
	player->SetADS(false);
	player->SetWeaponADS(false);
}

void CPlayerPatternADS::Update(CPlayer* player)
{
	// アニメーションの整合性を取る
	player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

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

	player->GetModel()->ChangeAnim(PLAYER_ADS, 0.3f);

	// ADS解除
	if (!inputMouse->GetRightPress())
	{
		player->ChangePattern(new CPlayerPatternNormal());
		return;
	}

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
	dir *= PLAYER_ADS_MOVE_SPEED;

	player->Move(dir.x, dir.y);

	// 回転
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// ADSが完了したら照準の表示
	DWORD currentTime = timeGetTime();
	if (currentTime - m_Time < 1000 / 5.0f)
	{
		return;
	}
	player->SetWeaponADS(true);

	// 攻撃
	if (inputMouse->GetLeftPress())
	{
		player->Shoot();
	}
	if (inputMouse->GetLeftRelease())
	{
		player->TriggerRelease();
	}
}
