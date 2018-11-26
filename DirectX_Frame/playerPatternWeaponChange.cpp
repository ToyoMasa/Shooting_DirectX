//======================================================================
//	プレイヤーリロードパターン [playerPatternWeaponChange.cpp]　（2018/11/26）
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
#include "playerPatternWeaponChange.h"

void CPlayerPatternWeaponChange::Init(CPlayer* player)
{
	player->GetModel()->ChangeAnim(PLAYER_NONE, 0.2f);
	player->SetADS(false);
	player->SetWeaponADS(false);
	m_CountFrame = 0;
}

void CPlayerPatternWeaponChange::Update(CPlayer* player)
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

	// 回転
	player->Rotate(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);

	// フレームに応じた処理
	if (m_CountFrame < 12)
	{
		player->GetModel()->ChangeAnim(PLAYER_NONE, 0.0f);
	}
	else if (m_CountFrame == 60)
	{
		player->ChangeWeapon();
		player->GetModel()->ChangeAnim(PLAYER_IDLE, 0.5f);
	}
	else if (m_CountFrame == 90)
	{
		player->ChangePattern(new CPlayerPatternNormal());
	}

	m_CountFrame++;
}
