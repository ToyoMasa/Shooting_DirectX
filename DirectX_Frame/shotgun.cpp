//======================================================================
//	ショットガンクラス [shotgun.cpp]　（2018/10/04）
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
#include "shotgun.h"
#include "game.h"
#include "debug.h"

static const int DIFFUSSION = 200;

CSceneModel *model;

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

	m_BulletDebug = CDebugSphere::Create(m_MuzzlePos, 0.03f);

	model = CSceneModel::Create("data/models/shotgun.x");
	model->Move(D3DXVECTOR3(0.0f, 1.7f, 0.0f));
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
	}
}

void CShotgun::Shoot()
{
	if (m_CoolDown <= 0)
	{
		for (int i = 0; i < 12; i++)
		{
			D3DXVECTOR3 bulletVec = CModeGame::GetCamera()->GetFront();
			bulletVec = bulletVec + CModeGame::GetCamera()->GetUp() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;
			bulletVec = bulletVec + CModeGame::GetCamera()->GetRight() * (-DIFFUSSION / 2.0f + rand() % DIFFUSSION) / 1000.0f;

			CBullet::Create(m_MuzzlePos, bulletVec, 10.0f, 20.0f, 15);
		}
		m_CoolDown = m_Rate;
		int i = 0;
	}
}

CShotgun* CShotgun::Create(CSceneSkinMesh *parent)
{
	CShotgun* Shotgun = new CShotgun();
	Shotgun->Init(parent);

	return Shotgun;
}
