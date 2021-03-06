//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
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
#include "normalmapShader.h"
#include "modelSpotlightShader.h"
#include "spotlightShader.h"
#include "enemyManager.h"

#define RECOILE_PATTERN_X ((0.1 * m_CountFire * (-350 + rand() % 1000) * 0.001))
#define RECOILE_PATTERN_Y ((0.1 * m_CountFire * (rand() % 1000) * 0.001))
#define RIFLE_RATE (60.0f / (460.0f / 60.0f))

static const int RIFLE_MAX_AMMO = 40;
static const int DIFFUSSION = 160;

void CRifle::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_RIFLE], LAYER_OBJECT3D);
	m_Model->SetUseShadow(false);
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_RIFLE, CROSSHAIR_RIFLE_SIZE, CROSSHAIR_RIFLE_SIZE);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Crosshair->SetVisible(true);
	m_Reticle = CScene2D::Create(TEX_ID_CROSSHAIR_CIRCLE, 32, 32);
	m_Reticle->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f)); 
	m_Reticle->SetVisible(false);
	m_Reticle->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(90.0f, 90.0f, 90.0f);

	m_MuzzlePos = D3DXVECTOR3(-4.6f, 6.1f, -24.6f);
	D3DXMatrixTranslation(&m_MuzzleMatrix, m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);

	m_Model->Move(m_Pos);
	m_Model->Rotate(m_Rot);
	m_Model->Scale(m_Scale);

	m_Ammo = RIFLE_MAX_AMMO;
	m_MaxAmmo = RIFLE_MAX_AMMO;

	m_Rate = RIFLE_RATE;
	m_CoolDown = 0.0f;
	m_Damage = 28.0f;

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);

	// マズルフラッシュの初期化
	m_Flash = CBillBoard::Create(TEX_ID_FLASH);
	m_Flash->Set(TEX_ID_FLASH, m_MuzzlePos, 0.3f, BILLBOARDTYPE_ADD, D3DCOLOR_RGBA(192, 192, 32, 255));
	m_FlashAlpha = 0;
	m_isFlash = false;
	m_Flash->SetVisible(m_isFlash);

	m_Pos.x = -5.0f + 0.537f;
	m_Pos.y = -7.0f + 0.213f;
	m_Pos.z = 28.0f;

	m_Rot.x = 344.0f;
	m_Rot.y = 141.5f;
	m_Rot.z = 33.75f;

	m_Model->SetShader(CShaderModelSpotlight::GetShader());
	m_Model->SetNormalMapTexture("WPN_ASLc_Norm.png");

	if (m_FlashEffect == NULL)
	{
		m_FlashEffect = CEffekseer::Create(CEffekseer::EFFECT_MUZZLEFLASH);
		m_FlashEffect->RepeatEffect(false);
		m_FlashEffect->SetScale(0.025f, 0.025f, 0.025f);
		m_FlashEffect->SetSpeed(5.0f);
		m_FlashEffect->SetVisible(true);
	}

	// 銃声の準備
	for (int i = 0; i < RIFLE_SHOT_SE_NUM; i++)
	{
		m_ShotSE[i] = CSound::Create((SOUND_LABEL)(SOUND_LABEL_SE_RIFLE_SHOT1 + i));
	}
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
		m_FlashEffect->SetMtxRotate(m_Parent->GetMtxRotate());
		m_FlashEffect->SetLocation(m_MuzzlePos);

		m_BulletDebug->Set(m_MuzzlePos);

		if (m_isFlash)
		{
			// マズルフラッシュの更新
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

void CRifle::Shoot()
{
	if (m_CoolDown <= 0 && m_Ammo > 0)
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

		// 残弾を減らす
		m_Ammo--;

		// リコイル
		Recoil(0.03f, 0.12f);

		// マズルフラッシュ
		m_FlashEffect->SetMtxRotate(m_Parent->GetMtxRotate());
		m_FlashEffect->SetLocation(m_MuzzlePos);
		//m_FlashAlpha = 255;
		//m_isFlash = true;
		//m_Flash->SetVisible(m_isFlash);
		m_FlashEffect->Play();

		// 銃声
		SoundShot(); 
		
		CModeGame::GetEnemyManager()->AddHeat(0.5f);
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
	m_Reticle->SetVisible(ads);
	m_Crosshair->SetVisible(!ads);
	m_isADS = ads;
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

void CRifle::SoundShot()
{
	m_ShotSE[rand() % 6]->Play(SHOT_SE_VOLUME);
}
