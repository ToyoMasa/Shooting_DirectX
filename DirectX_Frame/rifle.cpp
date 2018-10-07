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

void CRifle::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create("data/models/rifle.x");
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_CIRCLE, 32, 32);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
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
	if (m_Parent != NULL)
	{
		// 射撃間隔を減らす
		m_CoolDown--;

		// 位置の更新
		m_ParentMatrix = m_Parent->GetBoneMatrix("hand_r");

		//ImGui::Begin("Weapon", 0);

		//ImGui::SliderFloat("RotX", &m_Rot.x, 0.0f, 360.0f);
		//ImGui::SliderFloat("RotY", &m_Rot.y, 0.0f, 360.0f);
		//ImGui::SliderFloat("RotZ", &m_Rot.z, 0.0f, 360.0f);

		//ImGui::SliderInt("PosX1", &xx1, -50, 50);
		//ImGui::SliderInt("PosX2", &xx2, 0, 1000);
		//ImGui::SliderInt("PosY1", &yy1, -50, 50);
		//ImGui::SliderInt("PosY2", &yy2, 0, 1000);
		//ImGui::SliderInt("PosZ1", &zz1, -50, 50);
		//ImGui::SliderInt("PosZ2", &zz2, 0, 1000);

		//m_Pos.x = xx1;
		//m_Pos.x += 0.001f * xx2;
		//m_Pos.y = yy1;
		//m_Pos.y += 0.001f * yy2;
		//m_Pos.z = zz1;
		//m_Pos.z += 0.001f * zz2;

		//ImGui::SliderInt("Scale", &scale, 0, 100);
		//ImGui::End();

		m_Pos.x = -5.0f + 0.537f;
		m_Pos.y = -7.0f + 0.213f;
		m_Pos.z = 28.0f;

		m_Rot.x = 344.0f;
		m_Rot.y = 141.5f;
		m_Rot.z = 33.75f;

		D3DXMATRIX mtxMove, mtxX, mtxY,mtxZ, mtxRot;
		D3DXMatrixTranslation(&mtxMove, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixRotationX(&mtxX, D3DXToRadian(m_Rot.x));
		D3DXMatrixRotationY(&mtxY, D3DXToRadian(m_Rot.y));
		D3DXMatrixRotationZ(&mtxZ, D3DXToRadian(m_Rot.z));
		mtxRot = mtxX * mtxY * mtxZ;
		m_Model->SetWorld(mtxRot * m_ParentMatrix);
		m_Model->Move(m_Pos);
		m_Model->Scale(m_Scale);

	/*	ImGui::Begin("Weapon", 0);
		ImGui::SliderFloat("RotX", &m_MuzzlePos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("RotY", &m_MuzzlePos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("RotZ", &m_MuzzlePos.z, -100.0f, 100.0f);
		ImGui::End();

		D3DXMatrixTranslation(&m_MuzzleMatrix, m_MuzzlePos.x, m_MuzzlePos.y, m_MuzzlePos.z);
*/
		D3DXMATRIX mtxMuzzle;
		mtxMuzzle = m_MuzzleMatrix * (mtxRot * m_ParentMatrix);
		m_MuzzlePos = D3DXVECTOR3(mtxMuzzle._41, mtxMuzzle._42, mtxMuzzle._43);

		m_BulletDebug->Set(m_MuzzlePos);
	}
}

void CRifle::Shoot()
{
	if (m_CoolDown <= 0)
	{
		CBullet::Create(m_MuzzlePos, CModeGame::GetCamera()->GetFront(), 15.0f, 100.0f, 15);
		m_CoolDown = m_Rate;
		int i = 0;
	}
}

CRifle* CRifle::Create(CSceneSkinMesh *parent)
{
	CRifle* rifle = new CRifle();
	rifle->Init(parent);

	return rifle;
}
