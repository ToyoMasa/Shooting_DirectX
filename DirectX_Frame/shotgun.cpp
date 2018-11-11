//======================================================================
//	�V���b�g�K���N���X [shotgun.cpp]�@�i2018/10/04�j
//	Author : �L�� ���r
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "camera.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "player.h"
#include "bullet.h"
#include "weapon.h"
#include "shotgun.h"
#include "game.h"
#include "debug.h"
#include "PlayerAnim.h"

#define RECOILE_PATTERN_X ((0.1 * m_CountFire * (-350 + rand() % 1000) * 0.001))
#define RECOILE_PATTERN_Y ((0.1 * m_CountFire * (rand() % 1000) * 0.001))

static const int DIFFUSSION = 200;
static const int ADS_DIFFUSSION = 80;

void CShotgun::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create("data/models/shotgun.x");
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_CIRCLE, 32, 32);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_MuzzlePos = D3DXVECTOR3(-4.5f, 11.5f, -22.0f);
	D3DXMatrixTranslation(&m_MuzzleMatrix, m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);

	m_Model->Move(m_Pos);
	m_Model->Rotate(m_Rot);
	m_Model->Scale(m_Scale);

	m_Rate = 60.0f / (200.0f / 60.0f);
	m_CoolDown = 0.0f;
	m_Damage = 8.0f;

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);

	// �}�Y���t���b�V���̏�����
	m_Flash = CBillBoard::Create(TEX_ID_FLASH);
	m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, NORMAL, D3DCOLOR_RGBA(255, 255, 0, 255));
	m_FlashAlpha = 0;
	m_isFlash = false;
	m_Flash->SetVisible(m_isFlash);

	m_isReleaseTrigger = true;
}

void CShotgun::Uninit()
{
	if (m_Model != NULL)
	{
		m_Model->Release();
		m_Model = NULL;
	}
}

void CShotgun::Update()
{
	if (m_Parent != NULL)
	{
		// �ˌ��Ԋu�����炷
		m_CoolDown--;

		// �ʒu�̍X�V
		m_ParentMatrix = m_Parent->GetBoneMatrix("hand_r");

		m_Pos = D3DXVECTOR3(-5 + 0.72f, 15 + 0.424f, -2.0f);
		m_Rot = D3DXVECTOR3(344.0f, 141 + 0.68f, 33.0f);

		m_Scale = D3DXVECTOR3(125.0f, 125.0f, 125.0f);

		D3DXMATRIX mtxMove, mtxX, mtxY, mtxZ, mtxRot;
		D3DXMatrixRotationX(&mtxX, D3DXToRadian(m_Rot.x));
		D3DXMatrixRotationY(&mtxY, D3DXToRadian(m_Rot.y));
		D3DXMatrixRotationZ(&mtxZ, D3DXToRadian(m_Rot.z));
		mtxRot = mtxX * mtxY * mtxZ;
		m_Model->SetWorld(mtxRot * m_ParentMatrix);

		D3DXMatrixTranslation(&mtxMove, m_Pos.x, m_Pos.y, m_Pos.z);
		m_Model->Move(m_Pos);
		m_Model->Scale(m_Scale);

		D3DXMATRIX mtxMuzzle;
		mtxMuzzle = m_MuzzleMatrix * (mtxRot * m_ParentMatrix);
		m_MuzzlePos = D3DXVECTOR3(mtxMuzzle._41, mtxMuzzle._42, mtxMuzzle._43);
		m_BulletDebug->Set(m_MuzzlePos);

		if (m_isFlash)
		{
			// �}�Y���t���b�V���̍X�V
			//D3DXVECTOR3 flashPos = m_MuzzlePos;
			//flashPos += CModeGame::GetCamera()->GetUp() * ((-500 + (rand() % 1000)) / 1000000.0f);
			//flashPos += CModeGame::GetCamera()->GetRight() * ((-500 + (rand() % 1000)) / 5000.0f);
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

void CShotgun::Shoot()
{
	if (m_CoolDown <= 0 && m_isReleaseTrigger)
	{
		if (m_isADS)
		{
			for (int i = 0; i < 12; i++)
			{
				D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
				bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-ADS_DIFFUSSION / 2.0f + rand() % ADS_DIFFUSSION) / 1000.0f;
				bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-ADS_DIFFUSSION / 2.0f + rand() % ADS_DIFFUSSION) / 1000.0f;

				CBullet::Create(m_MuzzlePos, bulletVec, 10.0f, 20.0f, 15);
			}
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
				bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;
				bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;

				CBullet::Create(m_MuzzlePos, bulletVec, 10.0f, 20.0f, m_Damage);
			}

			m_Parent->PlayMontage(PLAYER_FIRE, 0.0f, 0.13f, PLAYER_IDLE);
		}

		m_CoolDown = m_Rate;
		int i = 0;

		// �}�Y���t���b�V���̕`���L����
		m_FlashAlpha = 200;
		m_isFlash = true;
		m_Flash->SetVisible(m_isFlash);

		// �g���K�[���������܂܂ɂ���
		m_isReleaseTrigger = false;

		// ���R�C��
		Recoil(0.1f, 0.25f);
		m_CountFire = 0;
		m_RecoilCount = 5;
		m_RecoilX = m_TotalRecoilX / 5.0f;
		m_RecoilY = m_TotalRecoilY / 5.0f;
	}
}

CShotgun* CShotgun::Create(CSceneSkinMesh *parent)
{
	CShotgun* Shotgun = new CShotgun();
	Shotgun->Init(parent);

	return Shotgun;
}

void CShotgun::SetADS(bool ads)
{
	m_Crosshair->SetVisible(false);
	m_isADS = ads;
}

void CShotgun::ChangeCrosshair(int nextTex)
{

}

void CShotgun::Recoil(float recoilX, float recoilY)
{
	CModeGame::GetPlayer()->Rotate(recoilX * (float)RECOILE_PATTERN_X, -recoilY);
	m_TotalRecoilX = recoilX * (float)RECOILE_PATTERN_X;
	m_TotalRecoilY = recoilY;
}

void CShotgun::RecoilUpdate()
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
	
	ImGui::Begin("FireCount", 0);
	ImGui::Text("%d", m_CountFire);
	ImGui::End();
}

void CShotgun::ReleaseTrigger()
{
	// �g���K�[�𗣂�������
	m_isReleaseTrigger = true;
}
