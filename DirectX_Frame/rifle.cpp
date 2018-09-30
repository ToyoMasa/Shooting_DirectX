//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "weapon.h"
#include "rifle.h"

void CRifle::Init(CSceneSkinMesh *parent)
{
	m_Parent = parent;
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_RIFLE]);
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(70.0f, 70.0f, 70.0f);

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
		m_ParentMatrix = m_Parent->GetBoneMatrix("root_hand_l");
		m_Model->SetWorld(m_ParentMatrix);
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
