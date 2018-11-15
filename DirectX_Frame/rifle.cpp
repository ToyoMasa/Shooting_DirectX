//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "player.h"
#include "bullet.h"
#include "weapon.h"
#include "rifle.h"
#include "game.h"
#include "debug.h"
#include "emitter.h"
#include "billboard.h"
#include "PlayerAnim.h"
#include "metalShader.h"

#define RECOILE_PATTERN_X ((0.1 * m_CountFire * (-350 + rand() % 1000) * 0.001))
#define RECOILE_PATTERN_Y ((0.1 * m_CountFire * (rand() % 1000) * 0.001))

static const int DIFFUSSION = 160;
CShader* metal = NULL;

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
	m_Damage = 28.0f;

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);

	// マズルフラッシュの初期化
	m_Flash = CBillBoard::Create(TEX_ID_FLASH);
	m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, NORMAL, D3DCOLOR_RGBA(255, 255, 0, 255));
	m_FlashAlpha = 0;
	m_isFlash = false;
	m_Flash->SetVisible(m_isFlash);

	m_Pos.x = -5.0f + 0.537f;
	m_Pos.y = -7.0f + 0.213f;
	m_Pos.z = 28.0f;

	m_Rot.x = 344.0f;
	m_Rot.y = 141.5f;
	m_Rot.z = 33.75f;

	//metal = new CShaderMetal();
	m_Model->SetShader(CShaderMetal::GetShader());
}

void CRifle::Uninit()
{
	if (m_Model != NULL)
	{
		m_Model->Release();
		m_Model = NULL;
	}

	//delete metal;
}

void CRifle::Update()
{
	if (m_Parent != NULL)
	{
		// 射撃間隔を減らす
		m_CoolDown--;

		// 位置の更新
		m_ParentMatrix = m_Parent->GetBoneMatrix("hand_r");

		// ポジションの更新
		D3DXMATRIX mtxX, mtxY, mtxZ, mtxRot;
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
		if (m_isADS)
		{
			CBullet::Create(m_MuzzlePos, CModeGame::GetCamera()->GetFront(), 15.0f, 100.0f, m_Damage);
		}
		else
		{
			D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
			bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;
			bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;

			CBullet::Create(m_MuzzlePos, bulletVec, 15.0f, 100.0f, m_Damage);

			m_Parent->PlayMontage(PLAYER_FIRE, 0.0f, 0.13f, PLAYER_IDLE);
		}

		m_CoolDown = m_Rate;
		int i = 0;

		// マズルフラッシュの描画を有効化
		m_FlashAlpha = 200;
		m_isFlash = true;
		m_Flash->SetVisible(m_isFlash);

		// リコイル
		Recoil(0.03f, 0.07f);
	}
}

CRifle* CRifle::Create(CSceneSkinMesh *parent)
{
	CRifle* rifle = new CRifle();
	rifle->Init(parent);

	return rifle;
}

void CRifle::SetADS(bool ads)
{
	m_Crosshair->SetVisible(ads);
	m_isADS = ads;
}

void CRifle::ChangeCrosshair(int nextTex)
{
	if (m_Crosshair != NULL)
	{
		m_Crosshair->Release();
		m_Crosshair = CScene2D::Create(nextTex, 32, 32);
	}
}

void CRifle::Recoil(float recoilX, float recoilY)
{
	if (m_CountFire < 10)
	{
		m_CountFire++;

		CModeGame::GetPlayer()->Rotate(recoilX * (float)RECOILE_PATTERN_X, -recoilY * (float)RECOILE_PATTERN_Y);
		m_TotalRecoilX += recoilX * (float)RECOILE_PATTERN_X;
		m_TotalRecoilY += recoilY * (float)RECOILE_PATTERN_Y;
	}
	else
	{
		CModeGame::GetPlayer()->Rotate(recoilX * (float)RECOILE_PATTERN_X, -recoilY * 0.1f);
		m_TotalRecoilX += recoilX * (float)RECOILE_PATTERN_X;
		m_TotalRecoilY += recoilY * 0.1f;
	}
}

void CRifle::RecoilUpdate()
{
	if (m_CountFire == 0)
	{
		if (m_RecoilCount > 0)
		{
			CModeGame::GetPlayer()->Rotate(-m_RecoilX, m_RecoilY);
			m_RecoilCount--;
		}
		else
		{
			m_RecoilX = 0.0f;
			m_RecoilY = 0.0f;
			m_TotalRecoilX = 0.0f;
			m_TotalRecoilY = 0.0f;
		}
	}

	ImGui::Begin("FireCount", 0);
	ImGui::Text("%d", m_CountFire);
	ImGui::End();
}

void CRifle::ReleaseTrigger()
{
	if (m_CountFire < 3)
	{
		m_CountFire = 0;
		m_RecoilCount = 1;
		m_RecoilX = m_TotalRecoilX;
		m_RecoilY = m_TotalRecoilY;
	}
	else
	{
		m_CountFire = 0;
		m_RecoilCount = 5;
		m_RecoilX = m_TotalRecoilX / 5.0f;
		m_RecoilY = m_TotalRecoilY / 5.0f;
	}
}
