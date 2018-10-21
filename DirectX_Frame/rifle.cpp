//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "camera.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "bullet.h"
#include "weapon.h"
#include "rifle.h"
#include "game.h"
#include "debug.h"
#include "emitter.h"
#include "billboard.h"
#include "PlayerAnim.h"

static const int DIFFUSSION = 160;
bool once = false;

void CRifle::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create("data/models/rifle.x");
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_CIRCLE, 32, 32);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Crosshair->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(90.0f, 90.0f, 90.0f);

	m_MuzzlePos = D3DXVECTOR3(-4.6f, 6.1f, -24.6f);
	D3DXMatrixTranslation(&m_MuzzleMatrix, m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);

	m_Model->Move(m_Pos);
	m_Model->Rotate(m_Rot);
	m_Model->Scale(m_Scale);

	m_Rate = 60.0f / (600.0f / 60.0f);
	m_CoolDown = 0.0f;

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);
}

void CRifle::Uninit()
{
	if (m_Model != NULL)
	{
		m_Model->Release();
		m_Model = NULL;
	}
}

void CRifle::Update()
{
	// 最初の一度だけ実行　※initではカメラを取得できない場合があるため
	if (!once)
	{
		m_Flash = CBillBoard::Create(TEX_ID_FLASH);

		D3DXVECTOR3 flashPos = m_MuzzlePos;
		flashPos += CModeGame::GetCamera()->GetUp() * ((-500 + (rand() % 1000)) / 1000.0f);
		flashPos += CModeGame::GetCamera()->GetRight() * ((-500 + (rand() % 1000)) / 1000.0f);

		m_Flash->Set(TEX_ID_FLASH, flashPos, 0.3f, NORMAL, D3DCOLOR_RGBA(255, 255, 0, 255));

		m_FlashAlpha = 0;
		m_isFlash = false;
		m_Flash->SetVisible(m_isFlash);

		once = true;
	}

	if (m_Parent != NULL)
	{
		// 射撃間隔を減らす
		m_CoolDown--;

		// 位置の更新
		m_ParentMatrix = m_Parent->GetBoneMatrix("hand_r");

		m_Pos.x = -5.0f + 0.537f;
		m_Pos.y = -7.0f + 0.213f;
		m_Pos.z = 28.0f;

		m_Rot.x = 344.0f;
		m_Rot.y = 141.5f;
		m_Rot.z = 33.75f;

		// ポジションの更新
		D3DXMATRIX mtxMove, mtxX, mtxY, mtxZ, mtxRot;
		D3DXMatrixTranslation(&mtxMove, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixRotationX(&mtxX, D3DXToRadian(m_Rot.x));
		D3DXMatrixRotationY(&mtxY, D3DXToRadian(m_Rot.y));
		D3DXMatrixRotationZ(&mtxZ, D3DXToRadian(m_Rot.z));
		mtxRot = mtxX * mtxY * mtxZ;
		m_Model->SetWorld(mtxRot * m_ParentMatrix);
		m_Model->Move(m_Pos);
		m_Model->Scale(m_Scale);

		// マズルのポイントを更新
		D3DXMATRIX mtxMuzzle;
		mtxMuzzle = m_MuzzleMatrix * (mtxRot * m_ParentMatrix);
		m_MuzzlePos = D3DXVECTOR3(mtxMuzzle._41, mtxMuzzle._42, mtxMuzzle._43);

		m_BulletDebug->Set(m_MuzzlePos);

		if (m_isFlash)
		{
			// マズルフラッシュの更新
			D3DXVECTOR3 flashPos = m_MuzzlePos;
			flashPos += CModeGame::GetCamera()->GetUp() * ((-500 + (rand() % 1000)) / 1000000.0f);
			flashPos += CModeGame::GetCamera()->GetRight() * ((-500 + (rand() % 1000)) / 5000.0f);
			m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, NORMAL, D3DCOLOR_RGBA(255, 255, 0, m_FlashAlpha));

			if (m_FlashAlpha > 0)
			{
				m_FlashAlpha -= 20;
			}
			else
			{
				m_FlashAlpha = 0;
				m_isFlash = false;
				m_Flash->SetVisible(m_isFlash);
			}
		}
	}
}

void CRifle::Shoot()
{
	if (m_CoolDown <= 0)
	{
		D3DXVECTOR3 startVec = CModeGame::GetCamera()->GetFront();
		startVec = startVec + CModeGame::GetCamera()->GetUp() * -0.5f;
		startVec = startVec + CModeGame::GetCamera()->GetRight() * -0.5f;
		startVec *= 0.05f;

		D3DXVECTOR3 endVec = CModeGame::GetCamera()->GetFront();
		endVec = endVec + CModeGame::GetCamera()->GetUp() * 0.5f;
		endVec = endVec + CModeGame::GetCamera()->GetRight() * 0.5f;
		endVec *= 0.05f;

		if (m_isADS)
		{
			CBullet::Create(m_MuzzlePos, CModeGame::GetCamera()->GetFront(), 15.0f, 100.0f, 15);
		}
		else
		{
			D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
			bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;
			bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;

			CBullet::Create(m_MuzzlePos, bulletVec, 10.0f, 20.0f, 15);

			m_Parent->PlayMontage(PLAYER_FIRE, 0.0f, 0.13f, PLAYER_IDLE);
		}

		m_CoolDown = m_Rate;
		int i = 0;

		// マズルフラッシュの描画を有効化
		m_FlashAlpha = 200;
		m_isFlash = true;
		m_Flash->SetVisible(m_isFlash);
	}
}

CRifle* CRifle::Create(CSceneSkinMesh *parent)
{
	CRifle* rifle = new CRifle();
	rifle->Init(parent);

	return rifle;
}
