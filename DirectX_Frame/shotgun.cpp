//======================================================================
//	ショットガンクラス [shotgun.cpp]　（2018/10/04）
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
#include "shotgun.h"
#include "game.h"
#include "debug.h"
#include "PlayerAnim.h"
#include "metalShader.h"
#include "normalmapShader.h"
#include "normalmapSpotlightShader.h"
#include "enemyManager.h"

#define RECOILE_PATTERN_X ((0.1 * m_CountFire * (-350 + rand() % 1000) * 0.001))
#define RECOILE_PATTERN_Y ((0.1 * m_CountFire * (rand() % 1000) * 0.001))
#define SHOTGUN_RATE (60.0f / (360.0f / 60.0f))
static const int SHOTGUN_MAX_AMMO = 8;

static const int DIFFUSSION = 200;
static const int ADS_DIFFUSSION = 80;

void CShotgun::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_SHOTGUN]);
	m_Model->SetUseShadow(false);
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_SHOTGUN, CROSSHAIR_SHOTGUN_SIZE, CROSSHAIR_SHOTGUN_SIZE);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Crosshair->SetVisible(true);
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_MuzzlePos = D3DXVECTOR3(-4.5f, 11.5f, -22.0f);
	D3DXMatrixTranslation(&m_MuzzleMatrix, m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);

	m_Model->Move(m_Pos);
	m_Model->Rotate(m_Rot);
	m_Model->Scale(m_Scale);

	m_Ammo = SHOTGUN_MAX_AMMO;
	m_MaxAmmo = SHOTGUN_MAX_AMMO;

	m_Rate = SHOTGUN_RATE;
	m_CoolDown = 0.0f;
	m_Damage = 15.0f;

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);

	// マズルフラッシュの初期化
	m_Flash = CBillBoard::Create(TEX_ID_FLASH);
	m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, BILLBOARDTYPE_ADD, D3DCOLOR_RGBA(255, 255, 0, 255));
	m_FlashAlpha = 0;
	m_isFlash = false;
	m_Flash->SetVisible(m_isFlash);

	m_isReleaseTrigger = true;

	m_Model->SetShader(CShaderNormalmapSpotlight::GetShader());
	m_Model->SetNormalMapTexture("shotgun_normal.png");

	if (m_FlashEffect == NULL)
	{
		m_FlashEffect = CEffekseer::Create(CEffekseer::EFFECT_MUZZLEFLASH);
		m_FlashEffect->RepeatEffect(false);
		m_FlashEffect->SetScale(0.025f, 0.025f, 0.025f);
		m_FlashEffect->SetSpeed(30.0f);
		m_FlashEffect->SetVisible(true);
	}

	// 銃声の準備
	m_ShotSE = CSound::Create(SOUND_LABEL_SE_SHOTGUN_SHOT);
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
		// 射撃間隔を減らす
		m_CoolDown--;

		// 位置の更新
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
			 //マズルフラッシュの更新
			D3DXVECTOR3 flashPos = m_MuzzlePos;
			flashPos += CModeGame::GetCamera()->GetUp() * ((-500 + (rand() % 1000)) / 1000000.0f);
			flashPos += CModeGame::GetCamera()->GetRight() * ((-500 + (rand() % 1000)) / 5000.0f);
			m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, BILLBOARDTYPE_ADD, D3DCOLOR_RGBA(255, 255, 0, m_FlashAlpha));

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

	if (m_isActive)
	{
		if (m_isADS)
		{
			m_Crosshair->SetVisible(false);
		}
		else
		{
			m_Crosshair->SetVisible(true);
		}
	}
	else
	{
		m_Crosshair->SetVisible(false);
	}
}

void CShotgun::Shoot()
{
	if (m_CoolDown <= 0 && m_isReleaseTrigger && m_Ammo > 0)
	{
		if (m_isADS)
		{
			for (int i = 0; i < 8; i++)
			{
				D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
				bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-ADS_DIFFUSSION / 2.0f + rand() % ADS_DIFFUSSION) / 1000.0f;
				bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-ADS_DIFFUSSION / 2.0f + rand() % ADS_DIFFUSSION) / 1000.0f;

				CBullet::Create(m_MuzzlePos, bulletVec, 10.0f, 20.0f, 15);
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
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

		// 残弾を減らす
		m_Ammo--;

		// トリガーを押したままにする
		m_isReleaseTrigger = false;

		// リコイル
		Recoil(0.1f, 0.25f);
		m_CountFire = 0;
		m_RecoilCount = 10;
		m_RecoilX = m_TotalRecoilX / 10.0f;
		m_RecoilY = m_TotalRecoilY / 10.0f;

		m_FlashEffect->SetRotate(m_Rot);
		m_FlashEffect->SetLocation(m_MuzzlePos);
		m_FlashEffect->Play();

		// 銃声
		SoundShot();

		CModeGame::GetEnemyManager()->AddPlayerTension(1.0f);
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
	m_Crosshair->SetVisible(!ads);
	m_isADS = ads;
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
}

void CShotgun::ReleaseTrigger()
{
	// トリガーを離した判定
	m_isReleaseTrigger = true;
}

void CShotgun::SoundShot()
{
	m_ShotSE->Play(0.1f);
}
