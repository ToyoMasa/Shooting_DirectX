//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "weapon.h"
#include "rifle.h"

int scale = 0;

void CRifle::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create("data/models/rifle.x");
	m_Crosshair = CScene2D::Create(TEX_ID_CROSSHAIR_CIRCLE, 32, 32);
	m_Crosshair->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Model->Move(m_Pos);
	m_Model->Rotate(m_Rot);
	m_Model->Scale(m_Scale);
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
		m_ParentMatrix = m_Parent->GetBoneMatrix("hand_r");

		ImGui::Begin("Weapon", 0);

		ImGui::SliderFloat("RotX", &m_Rot.x, 0.0f, 360.0f);
		ImGui::SliderFloat("RotY", &m_Rot.y, 0.0f, 360.0f);
		ImGui::SliderFloat("RotZ", &m_Rot.z, 0.0f, 360.0f);

		ImGui::SliderFloat("PosX", &m_Pos.x, -50.0f, 50.0f);
		ImGui::SliderFloat("PosY", &m_Pos.y, -50.0f, 50.0f);
		ImGui::SliderFloat("PosZ", &m_Pos.z, -50.0f, 50.0f);

		ImGui::SliderInt("Scale", &scale, 0, 100);
		ImGui::End();
/*
		m_Rot = D3DXVECTOR3(342.0f, 141.0f, 34.364f);
		m_Pos = D3DXVECTOR3(5.909f, 22.273f, -14.545);*/
		scale = 75;

		m_Scale = D3DXVECTOR3(scale, scale, scale);

		D3DXMATRIX mtxMove, mtxX, mtxY,mtxZ, mtxRot;
		D3DXMatrixTranslation(&mtxMove, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixRotationX(&mtxX, D3DXToRadian(m_Rot.x));
		D3DXMatrixRotationY(&mtxY, D3DXToRadian(m_Rot.y));
		D3DXMatrixRotationZ(&mtxZ, D3DXToRadian(m_Rot.z));
		mtxRot = mtxX * mtxY * mtxZ;
		m_LocalMatrix = mtxRot * mtxMove;
		//m_Model->SetWorld(m_LocalMatrix * m_ParentMatrix);
		m_Model->SetWorld(mtxRot * m_ParentMatrix);
		m_Model->Move(m_Pos);
		m_Model->Scale(m_Scale);
	}
}

void CRifle::Shoot()
{

}

CRifle* CRifle::Create(CSceneSkinMesh *parent)
{
	CRifle* rifle = new CRifle();
	rifle->Init(parent);

	return rifle;
}
