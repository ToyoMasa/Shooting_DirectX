//======================================================================
//	ミッション目標カプセルクラス [targetCapsule.cpp]　（2018/11/24）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "texture.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "targetCapsule.h"
#include "debug.h"
#include "normalmapShader.h"
#include "emitter.h"
#include "bullet.h"
#include "Effekseer.h"
#include "game.h"
#include "normalmapSpotlightShader.h"

static const float CAPSULE_LIFE_MAX = 300.0f;

void CTargetCapsule::Init(D3DXVECTOR3 pos)
{
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_CAPSULE], LAYER_BACKGROUND);
	m_Model->SetShader(CShaderNormalmapSpotlight::GetShader());
	m_Model->SetNormalMapTexture("Capsule_Normal.png");

	m_Pos = pos;
	m_Model->Move(m_Pos);
	m_Model->Rotate(D3DXVECTOR3(0.0f, 180.0f, 0.0f));
	m_Model->Scale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));

	m_CollisionCapsule.Set(Point(m_Pos.x, m_Pos.y, m_Pos.z), Point(m_Pos.x, m_Pos.y + 2.5f, m_Pos.z), 1.0f);
	CDebugSphere::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y, m_Pos.z), 1.0f);
	CDebugSphere::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y + 2.5f, m_Pos.z), 1.0f);

	m_Life = CAPSULE_LIFE_MAX;
	m_isDestroyed = false;

	m_Flame = CEffekseer::Create(CEffekseer::EFFECT_FLAME);
	m_Flame->RepeatEffect(false);
	m_Flame->SetScale(0.5f, 0.5f, 0.5f);
	m_Flame->SetSpeed(1.0f);
	m_Flame->SetVisible(true);
	D3DXVECTOR3 flamepos = m_Pos;
	flamepos.y += 0.2f;
	flamepos.z += 1.0f;
	m_Flame->SetLocation(flamepos);
}

void CTargetCapsule::Uninit()
{
	if (m_Model)
	{
		m_Model->Release();
		m_Model = NULL;
	}
}

void CTargetCapsule::Update()
{
	if (CManager::GetDebug())
	{
		//ImGui::Begin("Target");
		//ImGui::Text("Pos  :X = %.2f Y = %.2f Z = %.2f", m_Pos.x, m_Pos.y, m_Pos.z);
		//ImGui::End();
	}

	// 銃弾との当たり判定
	if (!m_isDestroyed)
	{
		HitBullet();
	}
}

void CTargetCapsule::HitBullet()
{
	Capsule capsule;
	for (int i = 0; i < BULLET_MAX; i++)
	{
		CBullet* bullet = CBullet::GetBullet(i);
		if (bullet != NULL)
		{
			D3DXVECTOR3 bulletOldPos = bullet->GetOldPos();
			D3DXVECTOR3 bulletPos = bullet->GetPos();

			capsule.Set(Point(bulletOldPos.x, bulletOldPos.y, bulletOldPos.z),
				Point(bulletPos.x, bulletPos.y, bulletPos.z),
				BULLET_RADIUS);

			if (isCollisionCapsule(capsule, m_CollisionCapsule))
			{
				D3DXVECTOR3 pos1, pos2;

				D3DXVECTOR3 ray;
				ray = bullet->GetForward();
				D3DXVec3Normalize(&ray, &ray);

				D3DXVECTOR3 getPoint1, getPoint2;
				calcRayCapsule(
					bulletOldPos,
					ray,
					D3DXVECTOR3(m_Pos.x, m_Pos.y, m_Pos.z),
					D3DXVECTOR3(m_Pos.x, m_Pos.y + 2.5f, m_Pos.z),
					1.0f,
					getPoint1,
					getPoint2);

				CParticleEmitter::Create(TEX_ID_CIRCLE,
					5,
					1,
					5,
					0.4f,
					-0.05f,
					getPoint1,
					D3DXVECTOR3(-0.1f, 0.1f, -0.1f),
					D3DXVECTOR3(0.1f, 0.15f, 0.1f),
					D3DXVECTOR3(0.0f, -0.035f, 0.0f),
					false,
					D3DCOLOR_RGBA(192, 192, 16, 255));

				m_Life -= bullet->GetDamage();

				if (m_Life <= 0)
				{
					m_Model->SetVisible(false);
					//m_Model->Release();
					//m_Model = NULL;
					D3DXVECTOR3 flamepos = m_Pos;
					flamepos.y += 0.2f;
					flamepos.z += 1.0f;
					m_Flame->SetLocation(flamepos);
					m_Flame->Play();
					m_isDestroyed = true;
					CModeGame::GameEnd(GAME_CLEAR);
				}
			}
		}
	}
}

CTargetCapsule* CTargetCapsule::Create(D3DXVECTOR3 pos)
{
	CTargetCapsule* capsule = new CTargetCapsule();
	capsule->Init(pos);

	return capsule;
}
